/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// "liveMedia"
// Copyright (c) 1996-2015 Live Networks, Inc.  All rights reserved.
// A sink that generates an AVI file from a composite media session
// Implementation

#include "SMFAVIFileSink.hh"

#define fourChar(x,y,z,w) ( ((w)<<24)|((z)<<16)|((y)<<8)|(x) )/*little-endian*/

///////// AVIIndexRecord definition & implementation //////////

class AVIIndexRecord {
public:
  AVIIndexRecord(unsigned chunkId, unsigned flags, unsigned offset, unsigned size)
    : fNext(NULL), fChunkId(chunkId), fFlags(flags), fOffset(offset), fSize(size) {
  }

  AVIIndexRecord*& next() { return fNext; }
  unsigned chunkId() const { return fChunkId; }
  unsigned flags() const { return fFlags; }
  unsigned offset() const { return fOffset; }
  unsigned size() const { return fSize; }

private:
  AVIIndexRecord* fNext;
  unsigned fChunkId;
  unsigned fFlags;
  unsigned fOffset;
  unsigned fSize;
};


////////// SMFAVIFileSink implementation //////////

SMFAVIFileSink::SMFAVIFileSink(UsageEnvironment& env,
			 MediaSession& inputSession,
			 char const* outputFileName,
			 unsigned bufferSize,
			 unsigned short movieWidth, unsigned short movieHeight,
			 unsigned movieFPS, Boolean packetLossCompensate)
  : Medium(env), fInputSession(inputSession),
    fIndexRecordsHead(NULL), fIndexRecordsTail(NULL), fNumIndexRecords(0),
    fBufferSize(bufferSize), fPacketLossCompensate(packetLossCompensate),
    fAreCurrentlyBeingPlayed(False), fNumSubsessions(0), fNumBytesWritten(0),
    fHaveCompletedOutputFile(False),
    fMovieWidth(movieWidth), fMovieHeight(movieHeight), fMovieFPS(movieFPS) {
  fOutFid = OpenOutputFile(env, outputFileName);
  if (fOutFid == NULL) return;

  // Set up I/O state for each input subsession:
  MediaSubsessionIterator iter(fInputSession);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    // Ignore subsessions without a data source:
    FramedSource* subsessionSource = subsession->readSource();
    if (subsessionSource == NULL) continue;

    // If "subsession's" SDP description specified screen dimension
    // or frame rate parameters, then use these.
    if (subsession->videoWidth() != 0) {
      fMovieWidth = subsession->videoWidth();
    }
    if (subsession->videoHeight() != 0) {
      fMovieHeight = subsession->videoHeight();
    }
    if (subsession->videoFPS() != 0) {
      fMovieFPS = subsession->videoFPS();
    }

    SMFAVISubsessionIOState* ioState
      = new SMFAVISubsessionIOState(*this, *subsession);
    subsession->miscPtr = (void*)ioState;

    // Also set a 'BYE' handler for this subsession's RTCP instance:
    if (subsession->rtcpInstance() != NULL) {
      subsession->rtcpInstance()->setByeHandler(onRTCPBye, ioState);
    }

    ++fNumSubsessions;
  }

  // Begin by writing an AVI header:
  addFileHeader_AVI();
}

SMFAVIFileSink::~SMFAVIFileSink() {
  completeOutputFile();

  // Then, stop streaming and delete each active "SMFAVISubsessionIOState":
  MediaSubsessionIterator iter(fInputSession);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    if (subsession->readSource() != NULL) subsession->readSource()->stopGettingFrames();

    SMFAVISubsessionIOState* ioState
      = (SMFAVISubsessionIOState*)(subsession->miscPtr);
    if (ioState == NULL) continue;

    delete ioState;
  }

  // Then, delete the index records:
  AVIIndexRecord* cur = fIndexRecordsHead;
  while (cur != NULL) {
    AVIIndexRecord* next = cur->next();
    delete cur;
    cur = next;
  }

  // Finally, close our output file:
  CloseOutputFile(fOutFid);
}

SMFAVIFileSink* SMFAVIFileSink
::createNew(UsageEnvironment& env, MediaSession& inputSession,
	    char const* outputFileName,
	    unsigned bufferSize,
	    unsigned short movieWidth, unsigned short movieHeight,
	    unsigned movieFPS, Boolean packetLossCompensate) {
  SMFAVIFileSink* newSink =
    new SMFAVIFileSink(env, inputSession, outputFileName, bufferSize,
		    movieWidth, movieHeight, movieFPS, packetLossCompensate);
  if (newSink == NULL || newSink->fOutFid == NULL) {
    Medium::close(newSink);
    return NULL;
  }

  return newSink;
}

Boolean SMFAVIFileSink::startPlaying(afterPlayingFunc* afterFunc,
				  void* afterClientData) {
  // Make sure we're not already being played:
  if (fAreCurrentlyBeingPlayed) {
    envir().setResultMsg("This sink has already been played");
    return False;
  }

  fAreCurrentlyBeingPlayed = True;
  fAfterFunc = afterFunc;
  fAfterClientData = afterClientData;

  return continuePlaying();
}

Boolean SMFAVIFileSink::continuePlaying() {
  // Run through each of our input session's 'subsessions',
  // asking for a frame from each one:
  Boolean haveActiveSubsessions = False;
  MediaSubsessionIterator iter(fInputSession);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    FramedSource* subsessionSource = subsession->readSource();
    if (subsessionSource == NULL) continue;

    if (subsessionSource->isCurrentlyAwaitingData()) continue;

    SMFAVISubsessionIOState* ioState
      = (SMFAVISubsessionIOState*)(subsession->miscPtr);
    if (ioState == NULL) continue;

    haveActiveSubsessions = True;
    unsigned char* toPtr = ioState->fBuffer->dataEnd();
    unsigned toSize = ioState->fBuffer->bytesAvailable();
    subsessionSource->getNextFrame(toPtr, toSize,
				   afterGettingFrame, ioState,
				   onSourceClosure, ioState);
  }
  if (!haveActiveSubsessions) {
    envir().setResultMsg("No subsessions are currently active");
    return False;
  }

  return True;
}

void SMFAVIFileSink
::afterGettingFrame(void* clientData, unsigned packetDataSize,
		    unsigned numTruncatedBytes,
		    struct timeval presentationTime,
		    unsigned /*durationInMicroseconds*/) {
  SMFAVISubsessionIOState* ioState = (SMFAVISubsessionIOState*)clientData;
  if (numTruncatedBytes > 0) {
    ioState->envir() << "SMFAVIFileSink::afterGettingFrame(): The input frame data was too large for our buffer.  "
		     << numTruncatedBytes
		     << " bytes of trailing data was dropped!  Correct this by increasing the \"bufferSize\" parameter in the \"createNew()\" call.\n";
  }
  ioState->afterGettingFrame(packetDataSize, presentationTime);
}

void SMFAVIFileSink::onSourceClosure(void* clientData) {
  SMFAVISubsessionIOState* ioState = (SMFAVISubsessionIOState*)clientData;
  ioState->onSourceClosure();
}

void SMFAVIFileSink::onSourceClosure1() {
  // Check whether *all* of the subsession sources have closed.
  // If not, do nothing for now:
  MediaSubsessionIterator iter(fInputSession);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    SMFAVISubsessionIOState* ioState
      = (SMFAVISubsessionIOState*)(subsession->miscPtr);
    if (ioState == NULL) continue;

    if (ioState->fOurSourceIsActive) return; // this source hasn't closed
  }

  completeOutputFile();

  // Call our specified 'after' function:
  if (fAfterFunc != NULL) {
    (*fAfterFunc)(fAfterClientData);
  }
}

void SMFAVIFileSink::onRTCPBye(void* clientData) {
  SMFAVISubsessionIOState* ioState = (SMFAVISubsessionIOState*)clientData;

  struct timeval timeNow;
  gettimeofday(&timeNow, NULL);
  unsigned secsDiff
    = timeNow.tv_sec - ioState->fOurSink.fStartTime.tv_sec;

  MediaSubsession& subsession = ioState->fOurSubsession;
  ioState->envir() << "Received RTCP \"BYE\" on \""
		   << subsession.mediumName()
		   << "/" << subsession.codecName()
		   << "\" subsession (after "
		   << secsDiff << " seconds)\n";

  // Handle the reception of a RTCP "BYE" as if the source had closed:
  ioState->onSourceClosure();
}

void SMFAVIFileSink::addIndexRecord(AVIIndexRecord* newIndexRecord) {
  if (fIndexRecordsHead == NULL) {
    fIndexRecordsHead = newIndexRecord;
  } else {
    fIndexRecordsTail->next() = newIndexRecord;
  }
  fIndexRecordsTail = newIndexRecord;
  ++fNumIndexRecords;
}

void SMFAVIFileSink::completeOutputFile() {
  if (fHaveCompletedOutputFile || fOutFid == NULL) return;

  // Update various AVI 'size' fields to take account of the codec data that
  // we've now written to the file:
  unsigned maxBytesPerSecond = 0;
  unsigned numVideoFrames = 0;
  unsigned numAudioFrames = 0;

  //// Subsession-specific fields:
  MediaSubsessionIterator iter(fInputSession);
  MediaSubsession* subsession;
  while ((subsession = iter.next()) != NULL) {
    SMFAVISubsessionIOState* ioState
      = (SMFAVISubsessionIOState*)(subsession->miscPtr);
    if (ioState == NULL) continue;

    maxBytesPerSecond += ioState->fMaxBytesPerSecond;

    setWord(ioState->fSTRHFrameCountPosition, ioState->fNumFrames);
    if (ioState->fIsVideo) numVideoFrames = ioState->fNumFrames;
    else if (ioState->fIsAudio) numAudioFrames = ioState->fNumFrames;
  }

  //// Global fields:
  add4ByteString("idx1");
  addWord(fNumIndexRecords*4*4); // the size of all of the index records, which come next:
  for (AVIIndexRecord* indexRecord = fIndexRecordsHead; indexRecord != NULL; indexRecord = indexRecord->next()) {
    addWord(indexRecord->chunkId());
    addWord(indexRecord->flags());
    addWord(indexRecord->offset());
    addWord(indexRecord->size());
  }

  fRIFFSizeValue += fNumBytesWritten;
  setWord(fRIFFSizePosition, fRIFFSizeValue);

  setWord(fAVIHMaxBytesPerSecondPosition, maxBytesPerSecond);
  setWord(fAVIHFrameCountPosition,
	  numVideoFrames > 0 ? numVideoFrames : numAudioFrames);

  fMoviSizeValue += fNumBytesWritten;
  setWord(fMoviSizePosition, fMoviSizeValue);

  // We're done:
  fHaveCompletedOutputFile = True;
}


////////// SMFAVISubsessionIOState implementation ///////////

SMFAVISubsessionIOState::SMFAVISubsessionIOState(SMFAVIFileSink& sink,
				     MediaSubsession& subsession)
  : fOurSink(sink), fOurSubsession(subsession),
    fMaxBytesPerSecond(0), fIsVideo(False), fIsAudio(False), fIsByteSwappedAudio(False), fNumFrames(0) {
  fBuffer = new SubsessionBuffer(fOurSink.fBufferSize);
  fPrevBuffer = sink.fPacketLossCompensate
    ? new SubsessionBuffer(fOurSink.fBufferSize) : NULL;

  FramedSource* subsessionSource = subsession.readSource();
  fOurSourceIsActive = subsessionSource != NULL;

  fPrevPresentationTime.tv_sec = 0;
  fPrevPresentationTime.tv_usec = 0;
}

SMFAVISubsessionIOState::~SMFAVISubsessionIOState() {
  delete fBuffer; delete fPrevBuffer;
}

void SMFAVISubsessionIOState::setAVIstate(unsigned subsessionIndex) {
  fIsVideo = strcmp(fOurSubsession.mediumName(), "video") == 0;
  fIsAudio = strcmp(fOurSubsession.mediumName(), "audio") == 0;

  if (fIsVideo) {
    fAVISubsessionTag
      = fourChar('0'+subsessionIndex/10,'0'+subsessionIndex%10,'d','c');
    if (strcmp(fOurSubsession.codecName(), "JPEG") == 0) {
      fAVICodecHandlerType = fourChar('m','j','p','g');
    } else if (strcmp(fOurSubsession.codecName(), "MP4V-ES") == 0) {
      fAVICodecHandlerType = fourChar('D','I','V','X');
    } else if (strcmp(fOurSubsession.codecName(), "MPV") == 0) {
      fAVICodecHandlerType = fourChar('m','p','g','1'); // what about MPEG-2?
    } else if (strcmp(fOurSubsession.codecName(), "H263-1998") == 0 ||
	       strcmp(fOurSubsession.codecName(), "H263-2000") == 0) {
      fAVICodecHandlerType = fourChar('H','2','6','3');
    } else if (strcmp(fOurSubsession.codecName(), "H264") == 0) {
      fAVICodecHandlerType = fourChar('H','2','6','4');
    } else {
      fAVICodecHandlerType = fourChar('?','?','?','?');
    }
    fAVIScale = 1; // ??? #####
    fAVIRate = fOurSink.fMovieFPS; // ??? #####
    fAVISize = fOurSink.fMovieWidth*fOurSink.fMovieHeight*3; // ??? #####
  } else if (fIsAudio) {
    fIsByteSwappedAudio = False; // by default
    fAVISubsessionTag
      = fourChar('0'+subsessionIndex/10,'0'+subsessionIndex%10,'w','b');
    fAVICodecHandlerType = 1; // ??? ####
    unsigned numChannels = fOurSubsession.numChannels();
    fAVISamplingFrequency = fOurSubsession.rtpTimestampFrequency(); // default
    if (strcmp(fOurSubsession.codecName(), "L16") == 0) {
      fIsByteSwappedAudio = True; // need to byte-swap data before writing it
      fWAVCodecTag = 0x0001;
      fAVIScale = fAVISize = 2*numChannels; // 2 bytes/sample
      fAVIRate = fAVISize*fAVISamplingFrequency;
    } else if (strcmp(fOurSubsession.codecName(), "L8") == 0) {
      fWAVCodecTag = 0x0001;
      fAVIScale = fAVISize = numChannels; // 1 byte/sample
      fAVIRate = fAVISize*fAVISamplingFrequency;
    } else if (strcmp(fOurSubsession.codecName(), "PCMA") == 0) {
      fWAVCodecTag = 0x0006;
      fAVIScale = fAVISize = numChannels; // 1 byte/sample
      fAVIRate = fAVISize*fAVISamplingFrequency;
    } else if (strcmp(fOurSubsession.codecName(), "PCMU") == 0) {
      fWAVCodecTag = 0x0007;
      fAVIScale = fAVISize = numChannels; // 1 byte/sample
      fAVIRate = fAVISize*fAVISamplingFrequency;
    } else if (strcmp(fOurSubsession.codecName(), "MPA") == 0) {
      fWAVCodecTag = 0x0050;
      fAVIScale = fAVISize = 1;
      fAVIRate = 0; // ??? #####
    } else {
      fWAVCodecTag = 0x0001; // ??? #####
      fAVIScale = fAVISize = 1;
      fAVIRate = 0; // ??? #####
    }
  } else { // unknown medium
    fAVISubsessionTag
      = fourChar('0'+subsessionIndex/10,'0'+subsessionIndex%10,'?','?');
    fAVICodecHandlerType = 0;
    fAVIScale = fAVISize = 1;
    fAVIRate = 0; // ??? #####
  }
}

void SMFAVISubsessionIOState::afterGettingFrame(unsigned packetDataSize,
					  struct timeval presentationTime) {
  // Begin by checking whether there was a gap in the RTP stream.
  // If so, try to compensate for this (if desired):
  unsigned short rtpSeqNum
    = fOurSubsession.rtpSource()->curPacketRTPSeqNum();
  if (fOurSink.fPacketLossCompensate && fPrevBuffer->bytesInUse() > 0) {
    short seqNumGap = rtpSeqNum - fLastPacketRTPSeqNum;
    for (short i = 1; i < seqNumGap; ++i) {
      // Insert a copy of the previous frame, to compensate for the loss:
      useFrame(*fPrevBuffer);
    }
  }
  fLastPacketRTPSeqNum = rtpSeqNum;

  envir() << "rtpSeqNum = " << rtpSeqNum << "\n";

  // Now, continue working with the frame that we just got
  if (fBuffer->bytesInUse() == 0) {
    fBuffer->setPresentationTime(presentationTime);
  }
  fBuffer->addBytes(packetDataSize);

  useFrame(*fBuffer);
  if (fOurSink.fPacketLossCompensate) {
    // Save this frame, in case we need it for recovery:
    SubsessionBuffer* tmp = fPrevBuffer; // assert: != NULL
    fPrevBuffer = fBuffer;
    fBuffer = tmp;
  }
  fBuffer->reset(); // for the next input

  // Now, try getting more frames:
  fOurSink.continuePlaying();
}

void SMFAVISubsessionIOState::useFrame(SubsessionBuffer& buffer) {
  unsigned char* const frameSource = buffer.dataStart();
  unsigned const frameSize = buffer.bytesInUse();
  struct timeval const& presentationTime = buffer.presentationTime();
  if (fPrevPresentationTime.tv_usec != 0||fPrevPresentationTime.tv_sec != 0) {
    int uSecondsDiff
      = (presentationTime.tv_sec - fPrevPresentationTime.tv_sec)*1000000
      + (presentationTime.tv_usec - fPrevPresentationTime.tv_usec);
    if (uSecondsDiff > 0) {
      unsigned bytesPerSecond = (unsigned)((frameSize*1000000.0)/uSecondsDiff);
      if (bytesPerSecond > fMaxBytesPerSecond) {
	fMaxBytesPerSecond = bytesPerSecond;
      }
    }
  }
  fPrevPresentationTime = presentationTime;

  if (fIsByteSwappedAudio) {
    // We need to swap the 16-bit audio samples from big-endian
    // to little-endian order, before writing them to a file:
    for (unsigned i = 0; i < frameSize; i += 2) {
      unsigned char tmp = frameSource[i];
      frameSource[i] = frameSource[i+1];
      frameSource[i+1] = tmp;
    }
  }

  // Add an index record for this frame:
  AVIIndexRecord* newIndexRecord
    = new AVIIndexRecord(fAVISubsessionTag, // chunk id
			 frameSource[0] == 0x67 ? 0x10 : 0, // flags
			 fOurSink.fMoviSizePosition + 8 + fOurSink.fNumBytesWritten, // offset (note: 8 == size + 'movi')
			 frameSize + 4); // size
  fOurSink.addIndexRecord(newIndexRecord);

  // Write the data into the file:
  fOurSink.fNumBytesWritten += fOurSink.addWord(fAVISubsessionTag);
  if (strcmp(fOurSubsession.codecName(), "H264") == 0) {
    // Insert a 'start code' (0x00 0x00 0x00 0x01) in front of the frame:
    fOurSink.fNumBytesWritten += fOurSink.addWord(4+frameSize);
    fOurSink.fNumBytesWritten += fOurSink.addWord(fourChar(0x00, 0x00, 0x00, 0x01));//add start code
  } else {
    fOurSink.fNumBytesWritten += fOurSink.addWord(frameSize);
  }
  fwrite(frameSource, 1, frameSize, fOurSink.fOutFid);

  static int i = 0;
  envir() << "frame count = " << ++i << "\n";

  fOurSink.fNumBytesWritten += frameSize;
  // Pad to an even length:
  if (frameSize%2 != 0) fOurSink.fNumBytesWritten += fOurSink.addByte(0);

  ++fNumFrames;
}

void SMFAVISubsessionIOState::onSourceClosure() {
  fOurSourceIsActive = False;
  fOurSink.onSourceClosure1();
}


////////// AVI-specific implementation //////////

unsigned SMFAVIFileSink::addWord(unsigned word) {
  // Add "word" to the file in little-endian order:
  addByte(word); addByte(word>>8);
  addByte(word>>16); addByte(word>>24);

  return 4;
}

unsigned SMFAVIFileSink::addHalfWord(unsigned short halfWord) {
  // Add "halfWord" to the file in little-endian order:
  addByte((unsigned char)halfWord); addByte((unsigned char)(halfWord>>8));

  return 2;
}

unsigned SMFAVIFileSink::addZeroWords(unsigned numWords) {
  for (unsigned i = 0; i < numWords; ++i) {
    addWord(0);
  }

  return numWords*4;
}

unsigned SMFAVIFileSink::add4ByteString(char const* str) {
  addByte(str[0]); addByte(str[1]); addByte(str[2]);
  addByte(str[3] == '\0' ? ' ' : str[3]); // e.g., for "AVI "

  return 4;
}

void SMFAVIFileSink::setWord(unsigned filePosn, unsigned size) {
  do {
    if (SeekFile64(fOutFid, filePosn, SEEK_SET) < 0) break;
    addWord(size);
    if (SeekFile64(fOutFid, 0, SEEK_END) < 0) break; // go back to where we were

    return;
  } while (0);

  // One of the SeekFile64()s failed, probable because we're not a seekable file
  envir() << "SMFAVIFileSink::setWord(): SeekFile64 failed (err "
	  << envir().getErrno() << ")\n";
}

// Methods for writing particular file headers.  Note the following macros:

#define addFileHeader(tag,name) \
    unsigned SMFAVIFileSink::addFileHeader_##name() { \
        add4ByteString("" #tag ""); \
        unsigned headerSizePosn = (unsigned)TellFile64(fOutFid); addWord(0); \
        add4ByteString("" #name ""); \
        unsigned ignoredSize = 8;/*don't include size of tag or size fields*/ \
        unsigned size = 12

#define addFileHeader1(name) \
    unsigned SMFAVIFileSink::addFileHeader_##name() { \
        add4ByteString("" #name ""); \
        unsigned headerSizePosn = (unsigned)TellFile64(fOutFid); addWord(0); \
        unsigned ignoredSize = 8;/*don't include size of name or size fields*/ \
        unsigned size = 8

#define addFileHeaderEnd \
  setWord(headerSizePosn, size-ignoredSize); \
  return size; \
}

addFileHeader(RIFF,AVI);
    size += addFileHeader_hdrl();
    size += addFileHeader_movi();
    fRIFFSizePosition = headerSizePosn;
    fRIFFSizeValue = size-ignoredSize;
addFileHeaderEnd;

addFileHeader(LIST,hdrl);
    size += addFileHeader_avih();

    // Then, add a "strl" header for each subsession (stream):
    // (Make the video subsession (if any) come before the audio subsession.)
    unsigned subsessionCount = 0;
    MediaSubsessionIterator iter(fInputSession);
    MediaSubsession* subsession;
    while ((subsession = iter.next()) != NULL) {
      fCurrentIOState = (SMFAVISubsessionIOState*)(subsession->miscPtr);
      if (fCurrentIOState == NULL) continue;
      if (strcmp(subsession->mediumName(), "video") != 0) continue;

      fCurrentIOState->setAVIstate(subsessionCount++);
      size += addFileHeader_strl();
    }
    iter.reset();
    while ((subsession = iter.next()) != NULL) {
      fCurrentIOState = (SMFAVISubsessionIOState*)(subsession->miscPtr);
      if (fCurrentIOState == NULL) continue;
      if (strcmp(subsession->mediumName(), "video") == 0) continue;

      fCurrentIOState->setAVIstate(subsessionCount++);
      size += addFileHeader_strl();
    }

    // Then add another JUNK entry
    ++fJunkNumber;
    size += addFileHeader_JUNK();
addFileHeaderEnd;

#define AVIF_HASINDEX           0x00000010 // Index at end of file?
#define AVIF_MUSTUSEINDEX       0x00000020
#define AVIF_ISINTERLEAVED      0x00000100
#define AVIF_TRUSTCKTYPE        0x00000800 // Use CKType to find key frames?
#define AVIF_WASCAPTUREFILE     0x00010000
#define AVIF_COPYRIGHTED        0x00020000

addFileHeader1(avih);
    unsigned usecPerFrame = fMovieFPS == 0 ? 0 : 1000000/fMovieFPS;
    size += addWord(usecPerFrame); // dwMicroSecPerFrame
    fAVIHMaxBytesPerSecondPosition = (unsigned)TellFile64(fOutFid);
    size += addWord(0); // dwMaxBytesPerSec (fill in later)
    size += addWord(0); // dwPaddingGranularity
    size += addWord(AVIF_TRUSTCKTYPE|AVIF_HASINDEX|AVIF_ISINTERLEAVED); // dwFlags
    fAVIHFrameCountPosition = (unsigned)TellFile64(fOutFid);
    size += addWord(0); // dwTotalFrames (fill in later)
    size += addWord(0); // dwInitialFrame
    size += addWord(fNumSubsessions); // dwStreams
    size += addWord(fBufferSize); // dwSuggestedBufferSize
    size += addWord(fMovieWidth); // dwWidth
    size += addWord(fMovieHeight); // dwHeight
    size += addZeroWords(4); // dwReserved
addFileHeaderEnd;

addFileHeader(LIST,strl);
    size += addFileHeader_strh();
    size += addFileHeader_strf();
    fJunkNumber = 0;
    size += addFileHeader_JUNK();
addFileHeaderEnd;

addFileHeader1(strh);
    size += add4ByteString(fCurrentIOState->fIsVideo ? "vids" :
			   fCurrentIOState->fIsAudio ? "auds" :
			   "????"); // fccType
    size += addWord(fCurrentIOState->fAVICodecHandlerType); // fccHandler
    size += addWord(0); // dwFlags
    size += addWord(0); // wPriority + wLanguage
    size += addWord(0); // dwInitialFrames
    size += addWord(fCurrentIOState->fAVIScale); // dwScale
    size += addWord(fCurrentIOState->fAVIRate); // dwRate
    size += addWord(0); // dwStart
    fCurrentIOState->fSTRHFrameCountPosition = (unsigned)TellFile64(fOutFid);
    size += addWord(0); // dwLength (fill in later)
    size += addWord(fBufferSize); // dwSuggestedBufferSize
    size += addWord((unsigned)-1); // dwQuality
    size += addWord(fCurrentIOState->fAVISize); // dwSampleSize
    size += addWord(0); // rcFrame (start)
    if (fCurrentIOState->fIsVideo) {
        size += addHalfWord(fMovieWidth);
        size += addHalfWord(fMovieHeight);
    } else {
        size += addWord(0);
    }
addFileHeaderEnd;

addFileHeader1(strf);
    if (fCurrentIOState->fIsVideo) {
      // Add a BITMAPINFO header:
      unsigned extraDataSize = 0;
      size += addWord(10*4 + extraDataSize); // size
      size += addWord(fMovieWidth);
      size += addWord(fMovieHeight);
      size += addHalfWord(1); // planes
      size += addHalfWord(24); // bits-per-sample #####
      size += addWord(fCurrentIOState->fAVICodecHandlerType); // compr. type
      size += addWord(fCurrentIOState->fAVISize);
      size += addZeroWords(4); // ??? #####
      // Later, add extra data here (if any) #####
    } else if (fCurrentIOState->fIsAudio) {
      // Add a WAVFORMATEX header:
      size += addHalfWord(fCurrentIOState->fWAVCodecTag);
      unsigned numChannels = fCurrentIOState->fOurSubsession.numChannels();
      size += addHalfWord(numChannels);
      size += addWord(fCurrentIOState->fAVISamplingFrequency);
      size += addWord(fCurrentIOState->fAVIRate); // bytes per second
      size += addHalfWord(fCurrentIOState->fAVISize); // block alignment
      unsigned bitsPerSample = (fCurrentIOState->fAVISize*8)/numChannels;
      size += addHalfWord(bitsPerSample);
      if (strcmp(fCurrentIOState->fOurSubsession.codecName(), "MPA") == 0) {
	// Assume MPEG layer II audio (not MP3): #####
	size += addHalfWord(22); // wav_extra_size
	size += addHalfWord(2); // fwHeadLayer
	size += addWord(8*fCurrentIOState->fAVIRate); // dwHeadBitrate #####
	size += addHalfWord(numChannels == 2 ? 1: 8); // fwHeadMode
	size += addHalfWord(0); // fwHeadModeExt
	size += addHalfWord(1); // wHeadEmphasis
	size += addHalfWord(16); // fwHeadFlags
	size += addWord(0); // dwPTSLow
	size += addWord(0); // dwPTSHigh
      }
    }
addFileHeaderEnd;

#define AVI_MASTER_INDEX_SIZE   256

addFileHeader1(JUNK);
    if (fJunkNumber == 0) {
      size += addHalfWord(4); // wLongsPerEntry
      size += addHalfWord(0); // bIndexSubType + bIndexType
      size += addWord(0); // nEntriesInUse #####
      size += addWord(fCurrentIOState->fAVISubsessionTag); // dwChunkId
      size += addZeroWords(2); // dwReserved
      size += addZeroWords(AVI_MASTER_INDEX_SIZE*4);
    } else {
      size += add4ByteString("odml");
      size += add4ByteString("dmlh");
      unsigned wtfCount = 248;
      size += addWord(wtfCount); // ??? #####
      size += addZeroWords(wtfCount/4);
    }
addFileHeaderEnd;

addFileHeader(LIST,movi);
    fMoviSizePosition = headerSizePosn;
    fMoviSizeValue = size-ignoredSize;
addFileHeaderEnd;
