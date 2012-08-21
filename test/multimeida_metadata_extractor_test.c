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


#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <dlog.h>
#include <metadata_extractor.h>

#define SAFE_FREE(src)      { if(src) {free(src); src = NULL;}}

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "TIZEN_N_METADATAEXTRACTOR"


static int _is_file_exist (const char *filename);
static bool __capi_metadata_extractor(metadata_extractor_h metadata);

static int _is_file_exist (const char *filename)
{
	int ret = 1;
	if (filename) {
		const char *to_access = (strstr(filename,"file://")!=NULL)? filename+7:filename;
		ret = access (to_access, R_OK );
		if (ret != 0) {
			LOGI("file [%s] not found.\n", to_access);
		}
	}
	return !ret;
}

static bool __capi_metadata_extractor(metadata_extractor_h metadata)
{
	char *duration = 0;
	char *audio_bitrate = 0;
	char *audio_channel = 0;
	char *audio_samplerate = 0;
	char *audio_track_cnt = 0;
	char *video_bitrate = 0;
	char *video_fps = 0;
	char *video_width = 0;
	char *video_height = 0;
	char *video_track_cnt = 0;
	void *video_thumbnail = NULL;
	int video_thumbnail_len = 0;
	void *video_frame = NULL;
	int video_frame_len = 0;

	/*Tag info*/
	char *artist = NULL;
	char *title = NULL;
	char *album = NULL;
	char *genre = NULL;
	char *author = NULL;
	char *copyright = NULL;
	char *date = NULL;
	char *description = NULL;
	void *artwork = NULL;
	int artwork_size = 0;
	char *artwork_mime = NULL;
	char *track_num = NULL;
	char *classification = NULL;
	char *rating = NULL;
	char *longitude = 0;
	char *latitude = 0;
	char *altitude = 0;
	char *conductor = NULL;
	char *unsynclyrics = NULL;
	char *synclyrics_num = 0;
	char *rec_date = NULL;

	int idx = 0;
	unsigned long time_info = 0;
	char *lyrics = NULL;

	if(metadata == NULL)
	{
		LOGI("Invalid handle \n");
		return false;
	}

	/*Get metadata*/
	metadata_extractor_get_metadata(metadata, METADATA_DURATION, &duration);
	LOGI("duration = [%s]\n", duration);
	metadata_extractor_get_metadata(metadata, METADATA_AUDIO_BITRATE, &audio_bitrate);
	LOGI("audio_bitrate = [%s]bps\n", audio_bitrate);
	metadata_extractor_get_metadata(metadata, METADATA_AUDIO_CHANNELS, &audio_channel);
	LOGI("audio_channel = [%s]\n", audio_channel);
	metadata_extractor_get_metadata(metadata, METADATA_AUDIO_SAMPLERATE, &audio_samplerate);
	LOGI("audio_samplerate = [%s]Hz\n", audio_samplerate);
	metadata_extractor_get_metadata(metadata, METADATA_HAS_AUDIO, &audio_track_cnt);
	LOGI("audio_track_cnt = [%s]\n", audio_track_cnt);
	metadata_extractor_get_metadata(metadata, METADATA_VIDEO_BITRATE, &video_bitrate);
	LOGI("video_bitrate = [%s]bps\n", video_bitrate);
	metadata_extractor_get_metadata(metadata, METADATA_VIDEO_FPS, &video_fps);
	LOGI("video_fps = [%s]\n", video_fps);
	metadata_extractor_get_metadata(metadata, METADATA_VIDEO_WIDTH, &video_width);
	LOGI("video_width = [%s]\n", video_width);
	metadata_extractor_get_metadata(metadata, METADATA_VIDEO_HEIGHT, &video_height);
	LOGI("video_height = [%s]\n", video_height);
	metadata_extractor_get_metadata(metadata, METADATA_HAS_VIDEO, &video_track_cnt);
	LOGI("video_track_cnt = [%s]\n", video_track_cnt);

	metadata_extractor_get_metadata(metadata, METADATA_ARTIST, &artist);
	LOGI("artist = [%s]\n", artist);
	metadata_extractor_get_metadata(metadata, METADATA_TITLE, &title);
	LOGI("title = [%s]\n", title);
	metadata_extractor_get_metadata(metadata, METADATA_ALBUM, &album);
	LOGI("album = [%s]\n", album);
	metadata_extractor_get_metadata(metadata, METADATA_GENRE, &genre);
	LOGI("genre = [%s]\n", genre);
	metadata_extractor_get_metadata(metadata, METADATA_AUTHOR, &author);
	LOGI("author = [%s]\n", author);
	metadata_extractor_get_metadata(metadata, METADATA_COPYRIGHT, &copyright);
	LOGI("copyright = [%s]\n", copyright);
	metadata_extractor_get_metadata(metadata, METADATA_DATE, &date);
	LOGI("date = [%s]\n", date);
	metadata_extractor_get_metadata(metadata, METADATA_DESCRIPTION, &description);
	LOGI("description = [%s]\n", description);
	metadata_extractor_get_metadata(metadata, METADATA_TRACK_NUM, &track_num);
	LOGI("track_num = [%s]\n", track_num);
	metadata_extractor_get_metadata(metadata, METADATA_CLASSIFICATION, &classification);
	LOGI("classification = [%s]\n", classification);
	metadata_extractor_get_metadata(metadata, METADATA_RATING, &rating);
	LOGI("rating = [%s]\n", rating);
	metadata_extractor_get_metadata(metadata, METADATA_LONGITUDE, &longitude);
	LOGI("longitude = [%s]\n", longitude);
	metadata_extractor_get_metadata(metadata, METADATA_LATITUDE, &latitude);
	LOGI("latitude = [%s]\n", latitude);
	metadata_extractor_get_metadata(metadata, METADATA_ALTITUDE, &altitude);
	LOGI("altitude = [%s]\n", altitude);
	metadata_extractor_get_metadata(metadata, METADATA_CONDUCTOR, &conductor);
	LOGI("conductor = [%s]\n", conductor);
	metadata_extractor_get_metadata(metadata, METADATA_UNSYNCLYRICS, &unsynclyrics);
	LOGI("unsynclyrics = [%s]\n", unsynclyrics);
	metadata_extractor_get_metadata(metadata, METADATA_RECDATE, &rec_date);
	LOGI("rec_date = [%s]\n", rec_date);

	metadata_extractor_get_metadata(metadata, METADATA_SYNCLYRICS_NUM, &synclyrics_num);
	int s_num = atoi(synclyrics_num);
	for(idx = 0; idx < s_num; idx++)
	{
		metadata_extractor_get_synclyrics(metadata, idx, &time_info, &lyrics);
		LOGI("[%2d][%6d][%s]\n", idx, time_info, lyrics);
		SAFE_FREE(lyrics);
	}

	/*Get Artwork*/
	metadata_extractor_get_artwork(metadata, &artwork, &artwork_size, &artwork_mime);
	LOGI("artwork = [%p], artwork_size = [%d]\n", artwork, artwork_size);
	LOGI("artwork_mime = [%s]\n", artwork_mime);

	/*Get Thumbnail*/
	metadata_extractor_get_frame(metadata, &video_thumbnail, &video_thumbnail_len);
	LOGI("video_thumbnail[%p], video_thumbnail_len = [%d]\n\n", video_thumbnail, video_thumbnail_len);

	/*Get Video frame at time, extract frame of 22.5 sec and not key frame*/
	metadata_extractor_get_frame_at_time(metadata, 22500, false, &video_frame, &video_frame_len);
	LOGI("video_frame[%p], video_frame_len = [%d]\n\n", video_frame, video_frame_len);

	SAFE_FREE(duration );
	SAFE_FREE(audio_bitrate );
	SAFE_FREE(audio_channel );
	SAFE_FREE(audio_samplerate );
	SAFE_FREE(audio_track_cnt );
	SAFE_FREE(video_bitrate );
	SAFE_FREE(video_fps );
	SAFE_FREE(video_width );
	SAFE_FREE(video_height );
	SAFE_FREE(video_track_cnt );
	SAFE_FREE(video_thumbnail);
	SAFE_FREE(video_frame);

	SAFE_FREE(artist);
	SAFE_FREE(title);
	SAFE_FREE(album);
	SAFE_FREE(genre);
	SAFE_FREE(author);
	SAFE_FREE(copyright);
	SAFE_FREE(date);
	SAFE_FREE(description);
	SAFE_FREE(artwork);
	SAFE_FREE(artwork_mime);
	SAFE_FREE(track_num);
	SAFE_FREE(classification);
	SAFE_FREE(rating);
	SAFE_FREE(longitude);
	SAFE_FREE(latitude);
	SAFE_FREE(altitude);
	SAFE_FREE(conductor);
	SAFE_FREE(unsynclyrics);
	SAFE_FREE(synclyrics_num);
	SAFE_FREE(rec_date);

	return true;

}

int main(int argc, char *argv[])
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_h metadata;
	int idx = 0;
	int cnt = argc -1;
	LOGI("--- metadata extractor test start ---\n\n");

	if(cnt < 1)
	{
		LOGI("type file path plz. [%d]\n", cnt);
		return 0;
	}

	ret = metadata_extractor_create(&metadata);
	if(ret != METADATA_EXTRACTOR_ERROR_NONE)
	{
		LOGE("Fail metadata_extractor_create [%d]\n", ret);
		return 0;
	}

	for(idx = 0; idx < cnt; idx++)
	{
		LOGI("--------------------------------------------\n");
		if (!_is_file_exist (argv[idx+1]))
		{
			LOGI("there is no file [%s]\n", argv[idx+1]);
			goto exception;
		}

		ret = metadata_extractor_set_path(metadata, argv[idx+1]);
		if(ret != METADATA_EXTRACTOR_ERROR_NONE)
		{
			LOGE("Fail metadata_extractor_set_path [%d]\n", ret);
			goto exception;
		}

		__capi_metadata_extractor(metadata);
		LOGI("--------------------------------------------\n");

	}

exception:
	ret = metadata_extractor_destroy(metadata);
	LOGI("metadata_extractor_destroy [%d]\n", ret);

	LOGI("--- metadata extractor test end ---\n\n");

	return 0;

}

