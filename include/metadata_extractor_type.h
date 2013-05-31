/*
* Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/



#ifndef __TIZEN_MEDIA_METADATA_EXTRACTOR_TYPE_H__
#define __TIZEN_MEDIA_METADATA_EXTRACTOR_TYPE_H__

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
* @addtogroup CAPI_METADATA_EXTRACTOR_MODULE
* @{
*/

/** Content Error Class */
#define METADATA_EXTRACTOR_ERROR_CLASS				TIZEN_ERROR_MULTIMEDIA_CLASS | 0x80

/**
 * @ingroup CAPI_METADATA_EXTRACTOR_MODULE
 * @brief The enumerations of metadata extractor error
 */
typedef enum
{
    METADATA_EXTRACTOR_ERROR_NONE					= TIZEN_ERROR_NONE,				/**< Successful */
    METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER	= TIZEN_ERROR_INVALID_PARAMETER,	/**< Invalid parameter */
    METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY 		= TIZEN_ERROR_OUT_OF_MEMORY,		/**< Out of memory */
    METADATA_EXTRACTOR_ERROR_FILE_EXISTS				= TIZEN_ERROR_FILE_EXISTS,			/**< File not exist */
    METADATA_EXTRACTOR_ERROR_OPERATION_FAILED		= METADATA_EXTRACTOR_ERROR_CLASS |0x01,	/**< Invalid internal operation */
} metadata_extractor_error_e;


/**
 * @ingroup CAPI_METADATA_EXTRACTOR_MODULE
 * @brief The enumerations of attribute
 */
typedef enum
{
	METADATA_DURATION	= 0,			/**< Duration */
	METADATA_VIDEO_BITRATE,			/**< Video Bitrate */
	METADATA_VIDEO_FPS,				/**< Video FPS */
	METADATA_VIDEO_WIDTH,			/**< Video Width*/
	METADATA_VIDEO_HEIGHT,			/**< Video Height*/
	METADATA_HAS_VIDEO,				/**< Video stream count */
	METADATA_AUDIO_BITRATE,			/**< Audio Bitrate*/
	METADATA_AUDIO_CHANNELS,		/**< Audio Channels*/
	METADATA_AUDIO_SAMPLERATE,		/**< Audio Samplerate*/
	METADATA_HAS_AUDIO,				/**< Audio stream count */
	METADATA_ARTIST,					/**< Artist*/
	METADATA_TITLE,					/**< Title*/
	METADATA_ALBUM,					/**< Album*/
	METADATA_GENRE,					/**< Genre*/
	METADATA_AUTHOR,					/**< Author*/
	METADATA_COPYRIGHT,				/**< Copyright*/
	METADATA_DATE,					/**< Date*/
	METADATA_DESCRIPTION,				/**< Description*/
	METADATA_COMMENT,				/**< Comment*/
	METADATA_TRACK_NUM,				/**< Track number info*/
	METADATA_CLASSIFICATION,			/**< Classification*/
	METADATA_RATING,					/**< Rating*/
	METADATA_LONGITUDE,				/**< Longitude*/
	METADATA_LATITUDE,				/**< Latitude*/
	METADATA_ALTITUDE,				/**< Altitude*/
	METADATA_CONDUCTOR,				/**< Conductor*/
	METADATA_UNSYNCLYRICS,			/**< Unsyncronized lyric*/
	METADATA_SYNCLYRICS_NUM,		/**< Syncronized lyric (time/lyric set) number*/
	METADATA_RECDATE,				/**< Recording date*/
	METADATA_ROTATE,					/**< Rotate(Orientation) Information*/
} metadata_extractor_attr_e;


/**
 * @ingroup CAPI_METADATA_EXTRACTOR_MODULE
 * @brief The handle of metadata extractor
 */
typedef struct metadata_extractor_s* metadata_extractor_h;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*__TIZEN_MEDIA_METADATA_EXTRACTOR_TYPE_H__*/
