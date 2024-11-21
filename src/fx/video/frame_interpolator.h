#pragma once

#include "fixed_map.h"
#include "fx/detail/circular_buffer.h"
#include "fx/detail/data_stream.h"
#include "fx/frame.h"
#include "fx/video/frame_tracker.h"
#include "namespace.h"

FASTLED_NAMESPACE_BEGIN

FASTLED_SMART_REF(FrameInterpolator);

// Holds onto frames and allow interpolation. This allows
// effects to have high effective frame rate and also
// respond to things like sound which can modify the timing.
class FrameInterpolator : public Referent {
  public:
    struct Less {
        bool operator()(uint32_t a, uint32_t b) const { return a < b; }
    };
    typedef SortedHeapMap<uint32_t, FrameRef, Less> FrameBuffer;
    FrameInterpolator(size_t nframes, float fpsVideo);

    // Will search through the array, select the two frames that are closest to
    // the current time and then interpolate between them, storing the results
    // in the provided frame. The destination frame will have "now" as the
    // current timestamp if and only if there are two frames that can be
    // interpolated. Else it's set to the timestamp of the frame that was
    // selected. Returns true if the interpolation was successful, false
    // otherwise. If false then the destination frame will not be modified. Note
    // that this adjustable_time is allowed to go pause or go backward in time.
    bool draw(uint32_t adjustable_time, Frame *dst); // Frame has alpha.
    bool draw(uint32_t adjustable_time, CRGB *leds, uint8_t *alpha = nullptr);



    bool insert(uint32_t frameNumber, FrameRef frame) {
        return mFrames.insert(frameNumber, frame);
    }

    // Clear all frames
    void clear() { mFrames.clear(); }

    bool empty() const { return mFrames.empty(); }

    bool has(uint32_t frameNum) const { return mFrames.has(frameNum); }

    FrameRef erase(uint32_t frameNum) {
        FrameRef out;
        auto it = mFrames.find(frameNum);
        if (it == mFrames.end()) {
            return out;
        }
        out = it->second;
        mFrames.erase(it);
        return out;
    }

    FrameRef get(uint32_t frameNum) const {
        auto it = mFrames.find(frameNum);
        if (it != mFrames.end()) {
            return it->second;
        }
        return FrameRef();
    }

    bool full() const { return mFrames.full(); }
    size_t capacity() const { return mFrames.capacity(); }

    FrameBuffer *getFrames() { return &mFrames; }

    bool needsFrame(uint32_t now, uint32_t *currentFrameNumber,
                    uint32_t *nextFrameNumber) const {
        mFrameTracker.get_interval_frames(now, currentFrameNumber,
                                          nextFrameNumber);
        return !has(*currentFrameNumber) || !has(*nextFrameNumber);
    }

    bool get_newest_frame_number(uint32_t *frameNumber) const {
        if (mFrames.empty()) {
            return false;
        }
        auto &front = mFrames.back();
        *frameNumber = front.first;
        return true;
    }

    bool get_oldest_frame_number(uint32_t *frameNumber) const {
        if (mFrames.empty()) {
            return false;
        }
        auto &front = mFrames.front();
        *frameNumber = front.first;
        return true;
    }

    uint32_t get_exact_timestamp_ms(uint32_t frameNumber) const {
        return mFrameTracker.get_exact_timestamp_ms(frameNumber);
    }

    FrameTracker &getFrameTracker() { return mFrameTracker; }

  private:
    FrameBuffer mFrames;
    FrameTracker mFrameTracker;
};

FASTLED_NAMESPACE_END
