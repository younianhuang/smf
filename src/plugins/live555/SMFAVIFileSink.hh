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
// C++ header

#ifndef SMF_AVI_FILE_SINK_HH
#define SMF_AVI_FILE_SINK_HH

#ifndef _MEDIA_SESSION_HH
#include "MediaSession.hh"
#endif

#include "InputFile.hh"
#include "OutputFile.hh"
#include "GroupsockHelper.hh"

class SMFAVIFileSink: public Medium {
public:
  static SMFAVIFileSink* createNew(UsageEnvironment& env,
                MediaSession& inputSession,
                char const* outputFileName,
                unsigned bufferSize = 20000,
                unsigned short movieWidth = 240,
                unsigned short movieHeight = 180,
                unsigned movieFPS = 15,
                Boolean packetLossCompensate = False);

  typedef void (afterPlayingFunc)(void* clientData);
  Boolean startPlaying(afterPlayingFunc* afterFunc,
                       void* afterClientData);

  unsigned numActiveSubsessions() const { return fNumSubsessions; }

private:
  SMFAVIFileSink(UsageEnvironment& env, MediaSession& inputSession,
          char const* outputFileName, unsigned bufferSize,
          unsigned short movieWidth, unsigned short movieHeight,
          unsigned movieFPS, Boolean packetLossCompensate);
      // called only by createNew()
  virtual ~SMFAVIFileSink();

  Boolean continuePlaying();
  static void afterGettingFrame(void* clientData, unsigned frameSize,
                unsigned numTruncatedBytes,
                struct timeval presentationTime,
                unsigned durationInMicroseconds);
  static void onSourceClosure(void* clientData);
  void onSourceClosure1();
  static void onRTCPBye(void* clientData);
  void addIndexRecord(class AVIIndexRecord* newIndexRecord);
  void completeOutputFile();

private:
  friend class SMFAVISubsessionIOState;
  MediaSession& fInputSession;
  FILE* fOutFid;
  class AVIIndexRecord *fIndexRecordsHead, *fIndexRecordsTail;
  unsigned fNumIndexRecords;
  unsigned fBufferSize;
  Boolean fPacketLossCompensate;
  Boolean fAreCurrentlyBeingPlayed;
  afterPlayingFunc* fAfterFunc;
  void* fAfterClientData;
  unsigned fNumSubsessions;
  unsigned fNumBytesWritten;
  struct timeval fStartTime;
  Boolean fHaveCompletedOutputFile;

private:
  ///// Definitions specific to the AVI file format:

  unsigned addWord(unsigned word); // outputs "word" in little-endian order
  unsigned addHalfWord(unsigned short halfWord);
  unsigned addByte(unsigned char byte) {
    putc(byte, fOutFid);
    return 1;
  }
  unsigned addZeroWords(unsigned numWords);
  unsigned add4ByteString(char const* str);
  void setWord(unsigned filePosn, unsigned size);

  // Define member functions for outputting various types of file header:
#define _header(name) unsigned addFileHeader_##name()
  _header(AVI);
      _header(hdrl);
          _header(avih);
          _header(strl);
              _header(strh);
              _header(strf);
              _header(JUNK);
//        _header(JUNK);
      _header(movi);
private:
  unsigned short fMovieWidth, fMovieHeight;
  unsigned fMovieFPS;
  unsigned fRIFFSizePosition, fRIFFSizeValue;
  unsigned fAVIHMaxBytesPerSecondPosition;
  unsigned fAVIHFrameCountPosition;
  unsigned fMoviSizePosition, fMoviSizeValue;
  class SMFAVISubsessionIOState* fCurrentIOState;
  unsigned fJunkNumber;
};

class SubsessionBuffer {
public:
  SubsessionBuffer(unsigned bufferSize)
    : fBufferSize(bufferSize) {
    reset();
    fData = new unsigned char[bufferSize];
  }
  virtual ~SubsessionBuffer() { delete[] fData; }
  void reset() { fBytesInUse = 0; }
  void addBytes(unsigned numBytes) { fBytesInUse += numBytes; }

  unsigned char* dataStart() { return &fData[0]; }
  unsigned char* dataEnd() { return &fData[fBytesInUse]; }
  unsigned bytesInUse() const { return fBytesInUse; }
  unsigned bytesAvailable() const { return fBufferSize - fBytesInUse; }

  void setPresentationTime(struct timeval const& presentationTime) {
    fPresentationTime = presentationTime;
  }
  struct timeval const& presentationTime() const {return fPresentationTime;}

private:
  unsigned fBufferSize;
  struct timeval fPresentationTime;
  unsigned char* fData;
  unsigned fBytesInUse;
};

////////// SMFAVISubsessionIOState ///////////
// A structure used to represent the I/O state of each input 'subsession':

class SMFAVISubsessionIOState {
public:
  SMFAVISubsessionIOState(SMFAVIFileSink& sink, MediaSubsession& subsession);
  virtual ~SMFAVISubsessionIOState();

  void setAVIstate(unsigned subsessionIndex);
  void setFinalAVIstate();

  void afterGettingFrame(unsigned packetDataSize,
             struct timeval presentationTime);
  void onSourceClosure();

  UsageEnvironment& envir() const { return fOurSink.envir(); }

public:
  SubsessionBuffer *fBuffer, *fPrevBuffer;
  SMFAVIFileSink& fOurSink;
  MediaSubsession& fOurSubsession;

  unsigned short fLastPacketRTPSeqNum;
  Boolean fOurSourceIsActive;
  struct timeval fPrevPresentationTime;
  unsigned fMaxBytesPerSecond;
  Boolean fIsVideo, fIsAudio, fIsByteSwappedAudio;
  unsigned fAVISubsessionTag;
  unsigned fAVICodecHandlerType;
  unsigned fAVISamplingFrequency; // for audio
  u_int16_t fWAVCodecTag; // for audio
  unsigned fAVIScale;
  unsigned fAVIRate;
  unsigned fAVISize;
  unsigned fNumFrames;
  unsigned fSTRHFrameCountPosition;

private:
  void useFrame(SubsessionBuffer& buffer);
};

#endif // SMF_AVI_FILE_SINK_HH
