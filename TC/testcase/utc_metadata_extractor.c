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


#include <tet_api.h>
#include <stdlib.h>
#include <metadata_extractor.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


#define MEDIA_PATH		"/opt/media/Music/Over the horizon.mp3"

metadata_extractor_h metadata = NULL;


static void utc_metadata_extractor_create_n(void);
static void utc_metadata_extractor_create_p(void);
static void utc_metadata_extractor_set_path_n(void);
static void utc_metadata_extractor_set_path_p(void);
static void utc_metadata_extractor_destroy_n(void);
static void utc_metadata_extractor_destroy_p(void);
static void utc_metadata_extractor_get_metadata_n(void);
static void utc_metadata_extractor_get_metadata_p(void);
static void utc_metadata_extractor_get_artwork_n(void);
static void utc_metadata_extractor_get_artwork_p(void);
static void utc_metadata_extractor_get_frame_n(void);
static void utc_metadata_extractor_get_frame_p(void);
static void utc_metadata_extractor_get_synclyrics_n(void);
static void utc_metadata_extractor_get_synclyrics_p(void);


struct tet_testlist tet_testlist[] = {
	{ utc_metadata_extractor_create_n, 1 },
	{ utc_metadata_extractor_create_p, 1 },
	{ utc_metadata_extractor_set_path_n, 2 },
	{ utc_metadata_extractor_set_path_p, 2 },
	{ utc_metadata_extractor_get_metadata_n, 2 },
	{ utc_metadata_extractor_get_metadata_p, 2 },
	{ utc_metadata_extractor_get_artwork_n, 2 },
	{ utc_metadata_extractor_get_artwork_p, 2 },
	{ utc_metadata_extractor_get_frame_n, 2 },
	{ utc_metadata_extractor_get_frame_p, 2 },
	{ utc_metadata_extractor_get_synclyrics_n, 2 },
	{ utc_metadata_extractor_get_synclyrics_p, 2 },
	{ utc_metadata_extractor_destroy_n, 3 },
	{ utc_metadata_extractor_destroy_p, 3 },
	{ NULL, 0 },
};


static void startup(void)
{
	/* start of TC */
}


static void cleanup(void)
{
	/* end of TC */
}


/**
 * @brief Negative test case of metadata_extractor_create()
 */
static void utc_metadata_extractor_create_n(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	ret = metadata_extractor_create(NULL);

	dts_check_eq("utc_metadata_extractor_create_n", ret, METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER, "Must return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER in case of invalid parameter");
}

/**
 * @brief Positive test case of metadata_extractor_create()
 */
static void utc_metadata_extractor_create_p(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	ret = metadata_extractor_create(&metadata);

	dts_check_eq("utc_metadata_extractor_create_p", ret, METADATA_EXTRACTOR_ERROR_NONE, "Failed to create handle");
}

/**
 * @brief Negative test case of metadata_extractor_set_path()
 */
static void utc_metadata_extractor_set_path_n(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	ret = metadata_extractor_set_path(NULL, MEDIA_PATH);

	dts_check_eq("utc_metadata_extractor_create_n", ret, METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER, "Must return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER in case of invalid parameter");
}

/**
 * @brief Positive test case of metadata_extractor_set_path()
 */
static void utc_metadata_extractor_set_path_p(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	ret = metadata_extractor_set_path(metadata, MEDIA_PATH);

	dts_check_eq("utc_metadata_extractor_set_path_p", ret, METADATA_EXTRACTOR_ERROR_NONE, "Failed to set path");
}

/**
 * @brief Negative test case of metadata_extractor_get_metadata()
 */
static void utc_metadata_extractor_get_metadata_n(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char * value = NULL;

	ret = metadata_extractor_get_metadata(metadata, -1, &value);

	dts_check_eq("utc_metadata_extractor_get_metadata_n", ret, METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER, "Must return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER in case of invalid parameter");
}

/**
 * @brief Positive test case of metadata_extractor_get_metadata()
 */
static void utc_metadata_extractor_get_metadata_p(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char * value = NULL;

	ret = metadata_extractor_get_metadata(metadata, METADATA_TITLE, &value);
	if(value)
		free(value);

	dts_check_eq("utc_metadata_extractor_get_metadata_p", ret, METADATA_EXTRACTOR_ERROR_NONE, "Failed to get metadata");
}

/**
 * @brief Negative test case of metadata_extractor_get_artwork()
 */
static void utc_metadata_extractor_get_artwork_n(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	void * artwork = NULL;
	char * artwork_mime = NULL;
	int artwork_size = 0;

	ret = metadata_extractor_get_artwork(NULL, &artwork, &artwork_size, &artwork_mime);

	dts_check_eq("utc_metadata_extractor_get_artwork_n", ret, METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER, "Must return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER in case of invalid parameter");
}

/**
 * @brief Positive test case of metadata_extractor_get_artwork()
 */
static void utc_metadata_extractor_get_artwork_p(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	void * artwork = NULL;
	char * artwork_mime = NULL;
	int artwork_size = 0;

	ret = metadata_extractor_get_artwork(metadata, &artwork, &artwork_size, &artwork_mime);
	if(artwork)
		free(artwork);
	if(artwork_mime)
		free(artwork_mime);

	dts_check_eq("utc_metadata_extractor_get_artwork_p", ret, METADATA_EXTRACTOR_ERROR_NONE, "Failed to get artwork");
}

/**
 * @brief Negative test case of metadata_extractor_get_frame()
 */
static void utc_metadata_extractor_get_frame_n(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	void * frame = NULL;
	int frame_size = 0;

	ret = metadata_extractor_get_frame(NULL, &frame, &frame_size);

	dts_check_eq("utc_metadata_extractor_get_frame_n", ret, METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER, "Must return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER in case of invalid parameter");
}

/**
 * @brief Positive test case of metadata_extractor_get_frame()
 */
static void utc_metadata_extractor_get_frame_p(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	void * frame = NULL;
	int frame_size = 0;

	ret = metadata_extractor_get_frame(metadata, &frame, &frame_size);
	if(frame)
		free(frame);

	dts_check_eq("utc_metadata_extractor_get_frame_p", ret, METADATA_EXTRACTOR_ERROR_NONE, "Failed to get frame");
}

/**
 * @brief Negative test case of metadata_extractor_get_synclyrics()
 */
static void utc_metadata_extractor_get_synclyrics_n(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	unsigned long  time_info = 0;
	char * lyrics = NULL;

	ret = metadata_extractor_get_synclyrics(NULL, 1, &time_info, &lyrics);

	dts_check_eq("utc_metadata_extractor_get_synclyrics_n", ret, METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER, "Must return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER in case of invalid parameter");
}

/**
 * @brief Positive test case of metadata_extractor_get_synclyrics()
 */
static void utc_metadata_extractor_get_synclyrics_p(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	unsigned long  time_info = 0;
	char * lyrics = NULL;

	ret = metadata_extractor_get_synclyrics(metadata, 1, &time_info, &lyrics);
	if(lyrics)
		free(lyrics);

	dts_check_eq("utc_metadata_extractor_get_synclyrics_p", ret, METADATA_EXTRACTOR_ERROR_NONE, "Failed to get lyrics");
}

/**
 * @brief Negative test case of metadata_extractor_destroy()
 */
static void utc_metadata_extractor_destroy_n(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	ret = metadata_extractor_destroy(NULL);

	dts_check_eq("utc_metadata_extractor_destroy_n", ret, METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER, "Must return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER in case of invalid parameter");
}

/**
 * @brief Positive test case of metadata_extractor_destroy()
 */

static void utc_metadata_extractor_destroy_p(void)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	ret = metadata_extractor_destroy(metadata);

	dts_check_eq("utc_metadata_extractor_destroy_p", ret, METADATA_EXTRACTOR_ERROR_NONE, "Failed to destroy handle");
}

