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

#ifndef __TIZEN_MEDIA_METADATA_EXTRACTOR_H__
#define __TIZEN_MEDIA_METADATA_EXTRACTOR_H__


#include <tizen.h>
#include <metadata_extractor_type.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @addtogroup CAPI_METADATA_EXTRACTOR_MODULE
 * @{
 *
 * @file metadata_extractor.h
 * @brief This file contains the multimedia content metadata extractor API and related structure and enumeration. \n
 *        Description of metadata: duration, bitrate, album, artist, author, genre and description etc. \n
 */


/**
 * @brief Create metadata
 *
 * @remarks @a metadata must be released metadata_extractor_destroy() by you
 *
 * @param [in] metadata The handle to metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_create(metadata_extractor_h* metadata);


/**
 * @brief Set file path to extract
 *
 * @param [in] metadata The handle to metadata
 * @param [in] path path to extract metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_FILE_EXISTS File not exist
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Create metadata handle by calling metadata_extractor_create()
 * @see metadata_extractor_create(), metadata_extractor_destroy()
 */
int metadata_extractor_set_path(metadata_extractor_h metadata, const char* path);


/**
 * @brief Destroy metadata
 *
 * @param [in] metadata The handle to metadata
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Create metadata handle by calling metadata_extractor_create()
 * @see metadata_extractor_create()
 */
int metadata_extractor_destroy(metadata_extractor_h metadata);


/**
 * @brief Get metadata
 *
 * @remarks @a value must be released with @c free() by you
 *
 * @param [in] metadata The handle to metadata
 * @param [in] attribute key attribute name to get
 * @param [out] value The value of the attribute
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Set path to extract by calling metadata_extractor_set_path()
 * @see metadata_extractor_create(), metadata_extractor_destroy()
 */
int metadata_extractor_get_metadata(metadata_extractor_h metadata, metadata_extractor_attr_e attribute, char ** value);


/**
 * @brief Get artwork image in media file
 *
 * @remarks @a artwork and @a artwork_mime must be released with @c free() by you
 *
 * @param [in] metadata The handle to metadata
 * @param [out] artwork encoded artwork image
 * @param [out] size encoded artwork size
 * @param [out] mime_type mime type of artwork
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Set path to extract by calling metadata_extractor_set_path()
 * @see metadata_extractor_create(), metadata_extractor_destroy()
 */
int metadata_extractor_get_artwork(metadata_extractor_h metadata, void ** artwork, int * size, char ** mime_type);


/**
 * @brief Get frame of video media file
 *
 * @remarks @a frame must be released with @c free() by you
 *
 * @param [in] metadata The handle to metadata
 * @param [out] frame raw frame data
 * @param [out] size The frame data size
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Set path to extract by calling metadata_extractor_set_path()
 * @see metadata_extractor_create(), metadata_extractor_destroy()
 */
int metadata_extractor_get_frame(metadata_extractor_h metadata, void ** frame, int * size);


/**
 * @brief Get synclyric of media file
 *
 * @remarks @a lyrics must be released with @c free() by you
 *
 * @param [in] metadata The handle to metadata
 * @param [in] index Index of time/lyrics set
 * @param [out] time_info Time information of index
 * @param [out] lyrics Lyric of index
 * @return 0 on success, otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal Operation Fail
 * @pre Set path to extract by calling metadata_extractor_set_path()
 * @pre Get time/lyrics set number by calling metadata_extractor_get_metadata(METADATA_SYNCLYRICS_NUM)
 * @see metadata_extractor_create(), metadata_extractor_destroy()
 */
int metadata_extractor_get_synclyrics(metadata_extractor_h metadata, int index, unsigned long *time_stamp, char ** lyrics);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_METADATA_EXTRACTOR_H__ */


