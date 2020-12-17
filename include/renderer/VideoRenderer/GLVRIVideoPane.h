/*

*/
#ifndef GLVR_IVIDEO_PANE_H
#define GLVR_IVIDEO_PANE_H

#include "GLVRPrerequisites.h"
#include "GLVRIRenderable.h"
#include "GLVRColor.h"
#include <framework/SMFPrimitives.h>


namespace surveon
{

namespace glvr
{

/** The class in charge of rendering a video stream.
*/
class IVideoPane : public IRenderable
{
public:       
        virtual ~IVideoPane(void)    {}

        /** Delivers a video buffer to the stream.
         *    This makes video renderer processes and render this the video buffer.
         * @param pVideoBuffer video buffer to render. The input pVideoBuffer is set as "Referenced"
         * when it is queued in VideoPane.
         * And it will be set as free after it is rendered.
        */
        virtual void processData(mf::IVideoBuffer* pVideoBuffer) = 0;

        /** Starts to render the video stream processed by antoher VideoPane.
         * This can save resources when 2 VideoPane render the same video.
         * @remarks
         * Onec a VideoPane share video stream form another, it can not process video data any more.
         * Call processData will raise an exception.
        */
        virtual void shareStreamFrom(IVideoPane* pVideoPane) = 0;

        /** Stops rendering the video stream form antoher VideoPane.
        */
        virtual void stopShareStream(void) = 0;

        /** Sets the area of video to be displayed.
         *   @param normalizedRect  the area of video want to display. This is in normalized coordinate of video.
        */
        virtual void setSourceRect(const mf::FloatRect& normalizedRect) = 0;

        virtual mf::FloatRect getSourceRect(void) const = 0;

        /** Sets the area of video to be displayed.
         * @param rect the area in the view port of rendering window.
         *    This is in window coordinate.
        */
        virtual void setDestinationRect(const mf::FloatRect& rect) = 0;

        virtual mf::FloatRect getDestinationRect(void) const = 0;

        /** Drop all video data that it has received and has not rendered yet.
        */
        virtual void flush(void) = 0;

        virtual void setBorderThickness(int thickness) = 0;

        virtual void setBorderColor(const Color& color) = 0;

        };

} // namespace glvr
} // namespace surveon




#endif // GLVR_IVIDEO_PANE_H

