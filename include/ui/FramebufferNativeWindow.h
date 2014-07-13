/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_FRAMEBUFFER_NATIVE_WINDOW_H
#define ANDROID_FRAMEBUFFER_NATIVE_WINDOW_H

#include <stdint.h>
#include <sys/types.h>

#include <EGL/egl.h>

#include <utils/threads.h>
#include <utils/String8.h>

#include <ui/ANativeObjectBase.h>
#include <ui/Rect.h>

#ifdef SAMSUNG_HDMI_SUPPORT
#include "SecHdmiClient.h"
#endif

#ifndef NUM_FRAME_BUFFERS
#define NUM_FRAME_BUFFERS  2
#endif

extern "C" EGLNativeWindowType android_createDisplaySurface(void);

// ---------------------------------------------------------------------------
namespace android {
// ---------------------------------------------------------------------------

class Surface;
class NativeBuffer;
#ifdef SAMSUNG_HDMI_SUPPORT
class SecHdmiClient;
#endif

// ---------------------------------------------------------------------------

class FramebufferNativeWindow 
    : public ANativeObjectBase<
        ANativeWindow, 
        FramebufferNativeWindow, 
        LightRefBase<FramebufferNativeWindow> >
{
public:
    FramebufferNativeWindow(); 

    framebuffer_device_t const * getDevice() const { return fbDev; } 

    bool isUpdateOnDemand() const { return mUpdateOnDemand; }
    status_t setUpdateRectangle(const Rect& updateRect);
    status_t compositionComplete();

    status_t rotate(unsigned int absoluteDegree);

    void discardQueuedBuffers(bool on);

    void dump(String8& result);
    void UIRotationChange(int uiRotation);
    void enableHDMIMirroring(bool enable);

    // for debugging only
    int getCurrentBufferIndex() const;

private:
    friend class LightRefBase<FramebufferNativeWindow>;    
    ~FramebufferNativeWindow(); // this class cannot be overloaded
    static int setSwapInterval(ANativeWindow* window, int interval);
    static int dequeueBuffer(ANativeWindow* window, ANativeWindowBuffer** buffer);
    static int lockBuffer(ANativeWindow* window, ANativeWindowBuffer* buffer);
    static int queueBuffer(ANativeWindow* window, ANativeWindowBuffer* buffer);
    static int query(const ANativeWindow* window, int what, int* value);
    static int perform(ANativeWindow* window, int operation, ...);

    static int cancelBuffer(ANativeWindow* window, android_native_buffer_t* buffer);
    
    framebuffer_device_t* fbDev;
    alloc_device_t* grDev;

    sp<NativeBuffer> buffers[NUM_FRAME_BUFFERS];
    sp<NativeBuffer> front;
    
    mutable Mutex mutex;
    Condition mCondition;
    int32_t mNumBuffers;
    int32_t mNumFreeBuffers;
    int32_t mBufferHead;
    int32_t mCurrentBufferIndex;
    bool mUpdateOnDemand;
    int mDiscardQueuedBuffersCnt;
#ifdef SAMSUNG_HDMI_SUPPORT
    SecHdmiClient *mHdmiClient;
#endif
};
    
// ---------------------------------------------------------------------------
}; // namespace android
// ---------------------------------------------------------------------------

#endif // ANDROID_FRAMEBUFFER_NATIVE_WINDOW_H

