/*
 *  Copyright (C) 2011, 2012, 2013 Stephen F. Booth <me@sbooth.org>
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

#pragma once

#include <AudioToolbox/AudioToolbox.h>

#include "AudioDecoder.h"

/*! @file AudioConverter.h @brief Support for converting audio from one PCM format to another */

/*! @brief An \c AudioConverter converts the output of an \c AudioDecoder to a different PCM format */
class AudioConverter
{
public:

	// ========================================
	/*! @name Creation and Destruction */
	//@{

	/*!
	 * @brief Create a new \c AudioConverter
	 * @note The \c AudioConverter will take ownership of the decoder
	 * @param decoder The \c AudioDecoder providing the input
	 * @param format The desired output format
	 * @param channelLayout The desired output channel layout or \c nullptr if not specified
	 */
	AudioConverter(AudioDecoder *decoder, const AudioStreamBasicDescription& format, AudioChannelLayout *channelLayout = nullptr);

	/*! @brief Destroy this \c AudioConverter */
	~AudioConverter();

	/*! @cond */

	/*! @internal This class is non-copyable */
	AudioConverter(const AudioConverter& rhs) = delete;

	/*! @internal This class is non-assignable */
	AudioConverter& operator=(const AudioConverter& rhs) = delete;

	/*! @endcond */
	//@}


	// ========================================
	/*! @name Opening and closing */
	//@{

	/*!
	 * @brief Open the converter's \c AudioDecoder and set up for conversion
	 * @param error An optional pointer to a \c CFErrorRef to receive error information
	 * @return \c true on success, \c false otherwise
	 * @see AudioDecoder::Open()
	 */
	bool Open(CFErrorRef *error = nullptr);

	/*!
	 * @brief Close the converter
	 * @param error An optional pointer to a \c CFErrorRef to receive error information
	 * @return \c true on success, \c false otherwise
	 * @see AudioDecoder::Close()
	 */
	bool Close(CFErrorRef *error = nullptr);

	/*! @brief Query whether this converter is open */
	inline bool IsOpen() const									{ return mIsOpen; }

	//@}


	// ========================================
	/*! @name Internals */
	//@{

	/*! @brief Get the \c AudioDecoder feeding this converter */
	inline AudioDecoder * GetDecoder() const					{ return mDecoder; }

	//@}


	// ========================================
	/*! @name Audio access */
	//@{

	/*! @brief Get the type of PCM data provided by this converter */
	inline AudioStreamBasicDescription GetFormat() const		{ return mFormat; }

	/*!
	 * @brief Create a description of the type of PCM data provided by this converter
	 * @note The returned string must be released by the caller
	 * @return A description of the type of PCM data provided by this converter
	 */
	CFStringRef CreateFormatDescription() const;


	/*! @brief Get the layout of the converter's audio channels, or \c nullptr if not specified */
	inline AudioChannelLayout * GetChannelLayout() const		{ return mChannelLayout; }

	/*!
	 * @brief Create a description of the layout of the converter's audio channels
	 * @note The returned string must be released by the caller
	 * @return A description of the layout of the converter's audio channels
	 */
	CFStringRef CreateChannelLayoutDescription() const;


	/*!
	 * @brief Convert audio into the specified buffer
	 * @param bufferList A buffer to receive the decoded audio
	 * @param frameCount The requested number of audio frames
	 * @return The actual number of frames read, or \c 0 on error
	 */
	UInt32 ConvertAudio(AudioBufferList *bufferList, UInt32 frameCount);

	/*! @brief Reset the internal conversion state */
	bool Reset();


	//@}


	/*! @cond */

	/*! @internal This class is exposed so it can be used inside C callbacks */
	class ConverterStateData;

	/*! @endcond */

private:
	AudioStreamBasicDescription			mFormat;			/*!< The format produced by this converter */
	AudioChannelLayout					*mChannelLayout;	/*!< The channel layout of the audio produced by this converter */
	AudioDecoder						*mDecoder;			/*!< The AudioDecoder providing the audio */
	AudioConverterRef					mConverter;			/*!< The actual object performing the conversion */
	ConverterStateData					*mConverterState;	/*!< Internal conversion state */
	bool								mIsOpen;			/*!< Flag indicating if the mConverter is open */
};
