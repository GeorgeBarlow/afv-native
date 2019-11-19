/* audio/AudioDevice.h
 *
 * This file is part of AFV-Native.
 *
 * Copyright (c) 2019 Christopher Collins
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef AFV_NATIVE_AUDIODEVICE_H
#define AFV_NATIVE_AUDIODEVICE_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <soundio/soundio.h>

#include "afv-native/audio/ISampleSink.h"
#include "afv-native/audio/ISampleSource.h"

namespace afv_native {
    namespace audio {
        class AudioDevice {
        private:
            static void staticSioReadCallback(struct SoundIoInStream *stream, int frame_count_min, int frame_count_max);
            static void staticSioWriteCallback(struct SoundIoOutStream *stream, int frame_count_min, int frame_count_max);
            void sioReadCallback(struct SoundIoInStream *stream, int frame_count_min, int frame_count_max);
            void sioWriteCallback(struct SoundIoOutStream *stream, int frame_count_min, int frame_count_max);

        protected:
            unsigned mApi;
            std::string mUserStreamName;
            std::string mOutputDeviceName;
            std::string mInputDeviceName;

            SoundIo *mSoundIO;
            SoundIoInStream *mInputStream;
            SoundIoOutStream *mOutputStream;
            bool mOutputIsStereo;

            SoundIoRingBuffer *mInputRingBuffer;
            SoundIoRingBuffer *mOutputRingBuffer;

            std::shared_ptr<ISampleSink> mSink;
            std::shared_ptr<ISampleSource> mSource;

            static size_t optimumFrameCount(size_t staleFrames, size_t min, size_t max);

            SoundIoDevice * getInputDeviceForId(const std::string &deviceId);
            SoundIoDevice * getOutputDeviceForId(const std::string &deviceId);

        public:
            typedef unsigned int Api;

            struct DeviceInfo {
                std::string name;
                std::string id;
                explicit DeviceInfo(const SoundIoDevice *src);
            };

            explicit AudioDevice(
                    const std::string &userStreamName,
                    const std::string &outputDeviceName,
                    const std::string &inputDeviceName,
                    Api audioApi=-1);
            virtual ~AudioDevice();
            bool open();
            void close();

            void setSource(std::shared_ptr<ISampleSource> newSrc);
            void setSink(std::shared_ptr<ISampleSink> newSink);

            static std::map<Api,std::string> getAPIs();
            static std::map<int,DeviceInfo> getCompatibleInputDevicesForApi(AudioDevice::Api api);
            static std::map<int,DeviceInfo> getCompatibleOutputDevicesForApi(AudioDevice::Api api);
        protected:
            static bool isAbleToOpen(SoundIoDevice *device_info);

        };
    }
}


#endif //AFV_NATIVE_AUDIODEVICE_H
