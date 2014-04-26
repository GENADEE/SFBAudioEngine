/*
 *  Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014 Stephen F. Booth <me@sbooth.org>
 *  All Rights Reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    - Neither the name of Stephen F. Booth nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "AudioOutput.h"

#include <CoreAudio/CoreAudioTypes.h>
#include <AudioToolbox/AudioToolbox.h>

/*! @file CoreAudioOutput.h @brief Core \w Audio output functionality */

/*! @brief \c SFBAudioEngine's encompassing namespace */
namespace SFB {

	/*! @brief %Audio functionality */
	namespace Audio {

		/*! @brief Output subclass supporting Apple's Core Audio */
		class CoreAudioOutput : public Output
		{
		public:

			CoreAudioOutput();
			virtual ~CoreAudioOutput();


			// ========================================
			/*! @name Device Parameters */
			//@{

			/*!
			 * @brief Get the device volume
			 *
			 * This corresponds to the property \c kHALOutputParam_Volume on element \c 0
			 * @note The volume is linear across the interval [0, 1]
			 * @param volume A \c Float32 to receive the volume
			 * @return \c true on success, \c false otherwise
			 */
			bool GetVolume(Float32& volume) const;

			/*!
			 * @brief Set the device volume
			 *
			 * This corresponds to the property \c kHALOutputParam_Volume on element \c 0
			 * @note The volume is linear and the value will be clamped to the interval [0, 1]
			 * @param volume The desired volume
			 * @return \c true on success, \c false otherwise
			 */
			bool SetVolume(Float32 volume);


			/*!
			 * @brief Get the volume for the specified channel
			 *
			 * This corresponds to the property \c kHALOutputParam_Volume on element \c channel
			 * @note The volume is linear across the interval [0, 1]
			 * @param channel The desired channel
			 * @param volume A \c Float32 to receive the channel's volume
			 * @return \c true on success, \c false otherwise
			 */
			bool GetVolumeForChannel(UInt32 channel, Float32& volume) const;

			/*!
			 * @brief Set the volume for the specified channel
			 *
			 * This corresponds to the property \c kHALOutputParam_Volume on element \c channel
			 * @note The volume is linear and the value will be clamped to the interval [0, 1]
			 * @param channel The desired channel
			 * @param volume The desired volume
			 * @return \c true on success, \c false otherwise
			 */
			bool SetVolumeForChannel(UInt32 channel, Float32 volume);


			/*!
			 * @brief Get the audio processing graph pre-gain
			 *
			 * This corresponds to the property \c kMultiChannelMixerParam_Volume
			 * @note Pre-gain is linear across the interval [0, 1]
			 * @param preGain A \c Float32 to receive the pre-gain
			 * @return \c true on success, \c false otherwise
			 */
			bool GetPreGain(Float32& preGain) const;

			/*!
			 * @brief Set the audio processing graph pre-gain
			 *
			 * This corresponds to the property \c kMultiChannelMixerParam_Volume
			 * @note The pre-gain is linear and the value will be clamped to the interval [0, 1]
			 * @param preGain The desired pre-gain
			 * @return \c true on success, \c false otherwise
			 */
			bool SetPreGain(Float32 preGain);


			/*! @brief Query whether the output is performing sample rate conversion */
			bool IsPerformingSampleRateConversion() const;

			/*!
			 * @brief Get the sample rate converter's complexity
			 *
			 * This corresponds to the property \c kAudioUnitProperty_SampleRateConverterComplexity
			 * @param complexity A \c UInt32 to receive the SRC complexity
			 * @return \c true on success, \c false otherwise
			 * @see kAudioUnitProperty_SampleRateConverterComplexity
			 */
			bool GetSampleRateConverterComplexity(UInt32& complexity) const;

			/*!
			 * @brief Set the sample rate converter's complexity
			 *
			 * This corresponds to the property \c kAudioUnitProperty_SampleRateConverterComplexity
			 * @param complexity The desired SRC complexity
			 * @return \c true on success, \c false otherwise
			 * @see kAudioUnitProperty_SampleRateConverterComplexity
			 */
			bool SetSampleRateConverterComplexity(UInt32 complexity);

			//@}


			// ========================================
			/*! @name DSP Effects */
			//@{

            /*!
			 * @brief Add a DSP effect to the audio processing graph with the component type kAudioUnitType_Effect
			 * @param subType The \c AudioComponent subtype
			 * @param manufacturer The \c AudioComponent manufacturer
			 * @param flags The \c AudioComponent flags
			 * @param mask The \c AudioComponent mask
			 * @param effectUnit An optional pointer to an \c AudioUnit to receive the effect
			 * @return \c true on success, \c false otherwise
			 * @see AudioComponentDescription
			 */
			bool AddEffect(OSType subType, OSType manufacturer, UInt32 flags, UInt32 mask, AudioUnit *effectUnit = nullptr);

			/*!
			 * @brief Add a DSP effect to the audio processing graph
             * @param componentType The \c AudioComponent type, normally \c kAudioUnitType_Effect
			 * @param subType The \c AudioComponent subtype
			 * @param manufacturer The \c AudioComponent manufacturer
			 * @param flags The \c AudioComponent flags
			 * @param mask The \c AudioComponent mask
			 * @param effectUnit An optional pointer to an \c AudioUnit to receive the effect
			 * @return \c true on success, \c false otherwise
			 * @see AudioComponentDescription
			 */
			bool AddEffect(OSType componentType, OSType subType, OSType manufacturer, UInt32 flags, UInt32 mask, AudioUnit *effectUnit = nullptr);

			/*!
			 * @brief Remove the specified DSP effect
			 * @param effectUnit The \c AudioUnit to remove from the processing graph
			 * @return \c true on success, \c false otherwise
			 */
			bool RemoveEffect(AudioUnit effectUnit);

			//@}


#if !TARGET_OS_IPHONE
			// ========================================
			/*! @name Hog Mode */
			//@{

			/*! @brief Query whether the output device hogged */
			bool OutputDeviceIsHogged() const;

			/*!
			 * @brief Start hogging the output device
			 *
			 * This will attempt to set the property \c kAudioDevicePropertyHogMode
			 * @return \c true on success, \c false otherwise
			 */
			bool StartHoggingOutputDevice();

			/*!
			 * @brief Stop hogging the output device
			 *
			 * This will attempt to clear the property \c kAudioDevicePropertyHogMode
			 * @return \c true on success, \c false otherwise
			 */
			bool StopHoggingOutputDevice();

			//@}


			// ========================================
			/*! @name Device parameters */
			//@{

			/*!
			 * @brief Get the device's master volume
			 *
			 * This corresponds to the property \c kAudioDevicePropertyVolumeScalar on element \c kAudioObjectPropertyElementMaster
			 * @param volume A \c Float32 to receive the master volume
			 * @return \c true on success, \c false otherwise
			 */
			bool GetDeviceMasterVolume(Float32& volume) const;

			/*!
			 * @brief Set the device's master volume
			 *
			 * This corresponds to the property \c kAudioDevicePropertyVolumeScalar on element \c kAudioObjectPropertyElementMaster
			 * @param volume The desired master volume
			 * @return \c true on success, \c false otherwise
			 * @see kAudioDevicePropertyVolumeScalar
			 */
			bool SetDeviceMasterVolume(Float32 volume);


			/*!
			 * @brief Get the device's volume for the specified channel
			 *
			 * This corresponds to the property \c kAudioDevicePropertyVolumeScalar on element \c channel
			 * @param channel The desired channel
			 * @param volume A \c Float32 to receive the volume
			 * @return \c true on success, \c false otherwise
			 */
			bool GetDeviceVolumeForChannel(UInt32 channel, Float32& volume) const;

			/*!
			 * @brief Set the device's volume for the specified channel
			 *
			 * This corresponds to the property \c kAudioDevicePropertyVolumeScalar on element \c channel
			 * @param channel The desired channel
			 * @param volume The desired volume
			 * @return \c true on success, \c false otherwise
			 */
			bool SetDeviceVolumeForChannel(UInt32 channel, Float32 volume);


			/*!
			 * @brief Get the number of output channels on the device
			 * @param channelCount A \c UInt32 to receive the channel count
			 * @return \c true on success, \c false otherwise
			 */
			bool GetDeviceChannelCount(UInt32& channelCount) const;

			/*!
			 * @brief Get the device's preferred stereo channel
			 * @param preferredStereoChannels A \c std::pair to receive the channels
			 * @return \c true on success, \c false otherwise
			 */
			bool GetDevicePreferredStereoChannels(std::pair<UInt32, UInt32>& preferredStereoChannels) const;

			//@}


			// ========================================
			/*! @name Device Management */
			//@{

			/*!
			 * @brief Create the UID of the output device
			 * @note The returned string must be released by the caller
			 * @param deviceUID A \c CFStringRef to receive the UID
			 * @return \c true on success, \c false otherwise
			 * @see GetOutputDeviceID()
			 */
			bool CreateOutputDeviceUID(CFStringRef& deviceUID) const;

			/*!
			 * @brief Set the output device to the device matching the provided UID
			 * @param deviceUID The UID of the desired device
			 * @return \c true on success, \c false otherwise
			 * @see SetOutputDeviceID()
			 */
			bool SetOutputDeviceUID(CFStringRef deviceUID);


			/*!
			 * @brief Get the device ID of the output device
			 * @param deviceID An \c AudioDeviceID to receive the device ID
			 * @return \c true on success, \c false otherwise
			 * @see CreateOutputDeviceUID()
			 */
			bool GetOutputDeviceID(AudioDeviceID& deviceID) const;

			/*!
			 * @brief Set the output device to the device matching the provided ID
			 * @param deviceID The ID of the desired device
			 * @return \c true on success, \c false otherwise
			 * @see SetOutputDeviceUID()
			 */
			bool SetOutputDeviceID(AudioDeviceID deviceID);


			/*!
			 * @brief Get the sample rate of the output device
			 * @param sampleRate A \c Float64 to receive the sample rate
			 * @return \c true on success, \c false otherwise
			 */
			bool GetOutputDeviceSampleRate(Float64& sampleRate) const;

			/*!
			 * @brief Set the sample rate of the output device
			 * @param sampleRate The desired sample rate
			 * @return \c true on success, \c false otherwise
			 */
			bool SetOutputDeviceSampleRate(Float64 sampleRate);
			
			//@}
			
#endif

		private:

			virtual bool _Open();
			virtual bool _Close();

			virtual bool _Start();
			virtual bool _Stop();
			virtual bool _RequestStop();

			virtual bool _IsOpen() const;
			virtual bool _IsRunning() const;

			virtual bool _Reset();

			virtual bool _SetupForDecoder(const Decoder& decoder);

			virtual bool _CreateDeviceUID(CFStringRef& deviceUID) const;
			virtual bool _SetDeviceUID(CFStringRef deviceUID);

			virtual bool _GetDeviceSampleRate(Float64& sampleRate) const;
			virtual bool _SetDeviceSampleRate(Float64 sampleRate);

			virtual size_t _GetPreferredBufferSize() const;

			// ========================================
			// AUGraph Utilities
			bool GetAUGraphLatency(Float64& latency) const;
			bool GetAUGraphTailTime(Float64& tailTime) const;

			bool SetPropertyOnAUGraphNodes(AudioUnitPropertyID propertyID, const void *propertyData, UInt32 propertyDataSize);

			bool SetOutputUnitChannelMap(const ChannelLayout& channelLayout);


			AUGraph		mAUGraph;
			AUNode		mMixerNode;
			AUNode		mOutputNode;
			UInt32		mDefaultMaximumFramesPerSlice;

		public:

			// ========================================
			/*! @cond */

			/*! @internal AUNode render callback */
			OSStatus Render(AudioUnitRenderActionFlags		*ioActionFlags,
							const AudioTimeStamp			*inTimeStamp,
							UInt32							inBusNumber,
							UInt32							inNumberFrames,
							AudioBufferList					*ioData);

			/*! @endcond */
		};
	}
}
