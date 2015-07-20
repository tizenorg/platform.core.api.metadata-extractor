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
 * @file metadata_extractor.h
 * @brief This file contains the multimedia content metadata extractor API and related structure and enumeration.
 *        It also contains the description of metadata: duration, bitrate, album, artist, author, genre, description, and so on.
 */

/**
 * @addtogroup CAPI_METADATA_EXTRACTOR_MODULE
 * @{
 */

/**
 * @brief Creates metadata.
 * @since_tizen 2.3
 * @remarks You must release @a metadata using metadata_extractor_destroy().
 *
 * @param[in] metadata The handle to metadata
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_create(metadata_extractor_h *metadata);

/**
 * @brief Sets the file path to extract.
 * @since_tizen 2.3
 * @param [in] metadata The handle to metadata
 * @param [in] path The path to extract metadata
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_FILE_EXISTS File does not exist
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Create a metadata handle by calling metadata_extractor_create().
 * @see metadata_extractor_create()
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_set_path(metadata_extractor_h metadata, const char *path);

/**
 * @brief Sets the buffer to extract.
 * @since_tizen 2.3
 * @param [in] metadata The handle to metadata
 * @param [in] buffer The buffer to extract metadata
 * @param [in] size The buffer size
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Create a metadata handle by calling metadata_extractor_create().
 * @see metadata_extractor_create()
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_set_buffer(metadata_extractor_h metadata, const void *buffer, int size);

/**
 * @brief Destroys metadata.
 * @since_tizen 2.3
 * @param [in] metadata The handle to metadata
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Create a metadata handle by calling metadata_extractor_create().
 * @see metadata_extractor_create()
 */
int metadata_extractor_destroy(metadata_extractor_h metadata);

/**
 * @brief Gets metadata.
 * @since_tizen 2.3
 *
 * @remarks You must release @a value using @c free(). \n
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add previlege http://tizen.org/privilege/mediastorage or http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] attribute The key attribute name to get
 * @param[out] value The value of the attribute
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Set the path to extract by calling metadata_extractor_set_path().
 * @see metadata_extractor_create()
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_get_metadata(metadata_extractor_h metadata, metadata_extractor_attr_e attribute, char **value);

/**
 * @brief Gets the artwork image in a media file.
 * @since_tizen 2.3
 *
 * @remarks You must release @a artwork and @a artwork_mime using @c free(). \n
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add previlege http://tizen.org/privilege/mediastorage or http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[out] artwork The encoded artwork image
 * @param[out] size The encoded artwork size
 * @param[out] mime_type The mime type of artwork
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Set the path to extract by calling metadata_extractor_set_path().
 * @see metadata_extractor_create()
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_get_artwork(metadata_extractor_h metadata, void **artwork, int *size, char **mime_type);

/**
 * @brief Gets the frame of a video media file.
 * @since_tizen 2.3
 *
 * @remarks You must release @a frame using @c free().
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add previlege http://tizen.org/privilege/mediastorage or http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[out] frame The raw frame data in RGB888
 * @param[out] size The frame data size
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Set the path to extract by calling metadata_extractor_set_path().
 * @see metadata_extractor_create()
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_get_frame(metadata_extractor_h metadata, void **frame, int *size);

/**
 * @brief Gets the synclyrics of a media file.
 * @since_tizen 2.3
 *
 * @remarks You must release @a lyrics using @c free().
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add previlege http://tizen.org/privilege/mediastorage or http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] index The index of time/lyrics to set
 * @param[out] time_stamp The time information of the index
 * @param[out] lyrics The lyrics of the index
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Set the path to extract by calling metadata_extractor_set_path().
 * @pre Get the time/lyrics set number by calling metadata_extractor_get_metadata(METADATA_SYNCLYRICS_NUM).
 * @see metadata_extractor_create()
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_get_synclyrics(metadata_extractor_h metadata, int index, unsigned long *time_stamp, char **lyrics);

/**
 * @brief Gets the frame of a video media.
 * @since_tizen 2.3
 *
 * @remarks You must release @a frame using @c free().
 *                   In case of accessing specific path in internal storage or external storage, you may add the privilege for accessing the path. \n
 *                   For example, if you get the specific path by using storage_get_directory(). you should add previlege http://tizen.org/privilege/mediastorage or http://tizen.org/privilege/externalstorage.
 *
 * @param[in] metadata The handle to metadata
 * @param[in] timestamp The timestamp in milliseconds
 * @param[in] is_accurate If @c true the user can get an accurate frame for the given timestamp,\n
 *                        otherwise @c false if the user can only get the nearest i-frame of the video rapidly
 * @param[out] frame The raw frame data in RGB888
 * @param[out] size The frame data size
 * @return @c 0 on success,
 *         otherwise a negative error value
 * @retval #METADATA_EXTRACTOR_ERROR_NONE Successful
 * @retval #METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY Not enough memory is available
 * @retval #METADATA_EXTRACTOR_ERROR_OPERATION_FAILED Internal operation failed
 * @retval #METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED Permission denied
 * @pre Set the path to extract by calling metadata_extractor_set_path().
 * @see metadata_extractor_create()
 * @see metadata_extractor_destroy()
 */
int metadata_extractor_get_frame_at_time(metadata_extractor_h metadata, unsigned long timestamp, bool is_accurate, void **frame, int *size);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TIZEN_MEDIA_METADATA_EXTRACTOR_H__ */


