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
#include <stdio.h>
#include <string.h>
#include <mm_file.h>
#include <metadata_extractor.h>
#include <metadata_extractor_private.h>

#define SAFE_FREE(src)      { if(src) {free(src); src = NULL;}}
#define META_MAX_LEN	256

typedef enum {
	METADATA_TYPE_STREAM	= 0,
	METADATA_TYPE_ATTR,
	METADATA_TYPE_TAG,
} metadata_extractor_type_e;

static int __metadata_extractor_check_and_extract_meta(metadata_extractor_s *metadata, metadata_extractor_type_e metadata_type);
static int __metadata_extractor_get_stream_info(metadata_extractor_s *metadata, const char *path);
static int __metadata_extractor_create_content_attrs(metadata_extractor_s *metadata, const char *path);
static int __metadata_extractor_create_tag_attr(metadata_extractor_s *metadata, const char *path);
static int __metadata_extractor_create_content_attrs_from_buffer(metadata_extractor_s *metadata, const void *buffer, int size);
static int __metadata_extractor_create_tag_attrs_from_buffer(metadata_extractor_s *metadata, const void *buffer, int size);
static int __metadata_extractor_get_artwork(metadata_extractor_s *metadata, void **artwork, int *artwork_size);
static int __metadata_extractor_get_artwork_mime(metadata_extractor_s *metadata, char **artwork_mime);
static int __metadata_extractor_get_video_thumbnail(metadata_extractor_s *metadata, void **thumbnail, int *thumbnail_len);
static int __metadata_extractor_get_duration(metadata_extractor_s *metadata, int *duration);
static int __metadata_extractor_get_audio_bitrate(metadata_extractor_s *metadata, int *bitrate);
static int __metadata_extractor_get_audio_channel(metadata_extractor_s *metadata, int *channel);
static int __metadata_extractor_get_audio_samplerate(metadata_extractor_s *metadata, int *samplerate);
static int __metadata_extractor_get_audio_track_count(metadata_extractor_s *metadata, int *track_cnt);
static int __metadata_extractor_get_video_bitrate(metadata_extractor_s *metadata, int *bitrate);
static int __metadata_extractor_get_video_FPS(metadata_extractor_s *metadata, int *fps);
static int __metadata_extractor_get_video_width(metadata_extractor_s *metadata, int *width);
static int __metadata_extractor_get_video_height(metadata_extractor_s *metadata, int *height);
static int __metadata_extractor_get_video_track_count(metadata_extractor_s *metadata, int *track_cnt);
static int __metadata_extractor_get_artist(metadata_extractor_s *metadata, char **artist);
static int __metadata_extractor_get_title(metadata_extractor_s *metadata, char **title);
static int __metadata_extractor_get_album(metadata_extractor_s *metadata, char **album);
static int __metadata_extractor_get_album_artist(metadata_extractor_s *metadata, char **album_artist);
static int __metadata_extractor_get_genre(metadata_extractor_s *metadata, char **genre);
static int __metadata_extractor_get_author(metadata_extractor_s *metadata, char **author);
static int __metadata_extractor_get_copyright(metadata_extractor_s *metadata, char **copyright);
static int __metadata_extractor_get_date(metadata_extractor_s *metadata, char **date);
static int __metadata_extractor_get_description(metadata_extractor_s *metadata, char **description);
static int __metadata_extractor_get_comment(metadata_extractor_s *metadata, char **comment);
static int __metadata_extractor_get_track_num(metadata_extractor_s *metadata, char **track_num);
static int __metadata_extractor_get_classification(metadata_extractor_s *metadata, char **classification);
static int __metadata_extractor_get_rating(metadata_extractor_s *metadata, char **rating);
static int __metadata_extractor_get_longitude(metadata_extractor_s *metadata, double *longitude);
static int __metadata_extractor_get_latitude(metadata_extractor_s *metadata, double *latitude);
static int __metadata_extractor_get_altitude(metadata_extractor_s *metadata, double *altitude);
static int __metadata_extractor_get_conductor(metadata_extractor_s *metadata, char **conductor);
static int __metadata_extractor_get_unsynclyrics(metadata_extractor_s *metadata, char **unsynclyrics);
static int __metadata_extractor_get_recording_date(metadata_extractor_s *metadata, char **rec_date);
static int __metadata_extractor_get_rotate(metadata_extractor_s *metadata, char **rotate);
static int __metadata_extractor_get_synclyrics_pair_num(metadata_extractor_s *metadata, int *synclyrics_num);
static int __metadata_extractor_destroy_handle(metadata_extractor_s *metadata);

static int __metadata_extractor_check_and_extract_meta(metadata_extractor_s *metadata, metadata_extractor_type_e metadata_type)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	if ((!metadata) || ((!metadata->path) && (!metadata->buffer))) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata_type == METADATA_TYPE_STREAM) {
		if ((metadata) && (metadata->attr_h)) {
			return ret;
		} else {
			metadata_extractor_debug("Extract Stream Info");
			ret = __metadata_extractor_get_stream_info(metadata, metadata->path);
			if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
				return ret;
			}
		}
	} else if (metadata_type == METADATA_TYPE_ATTR) {
		if ((metadata) && (metadata->attr_h)) {
			return ret;
		} else {
			metadata_extractor_debug("Extract Content attrs");
			if (metadata->path != NULL) {
				ret = __metadata_extractor_create_content_attrs(metadata, metadata->path);
				if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
					return ret;
				}
			} else if (metadata->buffer != NULL) {
				ret = __metadata_extractor_create_content_attrs_from_buffer(metadata, metadata->buffer, metadata->buffer_size);
				if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
					return ret;
				}
			}
		}
	} else if (metadata_type == METADATA_TYPE_TAG) {
		if ((metadata) && (metadata->tag_h)) {
			return ret;
		} else {
			metadata_extractor_debug("Extract Tag attrs");
			if (metadata->path != NULL) {
				ret = __metadata_extractor_create_tag_attr(metadata, metadata->path);
				if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
					return ret;
				}
			} else if (metadata->buffer != NULL) {
				ret = __metadata_extractor_create_tag_attrs_from_buffer(metadata, metadata->buffer, metadata->buffer_size);
				if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
					return ret;
				}
			}
		}
	} else {
		metadata_extractor_error("INVALID_PARAMETER(%d)", metadata_type);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	return ret;
}

static int __metadata_extractor_get_stream_info(metadata_extractor_s *metadata, const char *path)
{
	unsigned int ret = METADATA_EXTRACTOR_ERROR_NONE;
	int _audio_track_cnt = 0;
	int _video_track_cnt = 0;

	ret = mm_file_get_stream_info(path, &_audio_track_cnt, &_video_track_cnt);

	if (ret != FILEINFO_ERROR_NONE) {
		if (ret == FILEINFO_ERROR_FILE_NOT_FOUND) {
			metadata_extractor_error("FILE_NOT_EXISTS(0x%08x)", METADATA_EXTRACTOR_ERROR_FILE_EXISTS);
			return METADATA_EXTRACTOR_ERROR_FILE_EXISTS;
		} else {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	metadata->audio_track_cnt = _audio_track_cnt;
	metadata->video_track_cnt = _video_track_cnt;

	return ret;
}

static int __metadata_extractor_create_content_attrs(metadata_extractor_s *metadata, const char *path)
{
	unsigned int ret = METADATA_EXTRACTOR_ERROR_NONE;
	MMHandleType content = 0;
	char *err_attr_name = NULL;

	int _audio_track_cnt = 0;
	int _video_track_cnt = 0;

	ret = mm_file_create_content_attrs(&content, path);

	if (ret != FILEINFO_ERROR_NONE) {
		if (ret == FILEINFO_ERROR_FILE_NOT_FOUND) {
			metadata_extractor_error("FILE_NOT_EXISTS(0x%08x)", METADATA_EXTRACTOR_ERROR_FILE_EXISTS);
			return METADATA_EXTRACTOR_ERROR_FILE_EXISTS;
		} else {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	ret = mm_file_get_attrs(content, &err_attr_name,
	                        MM_FILE_CONTENT_VIDEO_TRACK_COUNT, &_video_track_cnt,
	                        MM_FILE_CONTENT_AUDIO_TRACK_COUNT, &_audio_track_cnt,
	                        NULL);

	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		mm_file_destroy_content_attrs(content);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	metadata->attr_h = content;
	metadata->audio_track_cnt = _audio_track_cnt;
	metadata->video_track_cnt = _video_track_cnt;

	return ret;

}


static int __metadata_extractor_create_tag_attr(metadata_extractor_s *metadata, const char *path)
{
	unsigned int ret = METADATA_EXTRACTOR_ERROR_NONE;
	MMHandleType tag = 0;

	ret = mm_file_create_tag_attrs(&tag, path);

	if (ret != FILEINFO_ERROR_NONE) {
		if (ret == FILEINFO_ERROR_FILE_NOT_FOUND) {
			metadata_extractor_error("FILE_NOT_EXISTS(0x%08x)", METADATA_EXTRACTOR_ERROR_FILE_EXISTS);
			return METADATA_EXTRACTOR_ERROR_FILE_EXISTS;
		} else {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	metadata->tag_h = tag;

	return ret;

}

static int __metadata_extractor_create_content_attrs_from_buffer(metadata_extractor_s *metadata, const void *buffer, int size)
{
	unsigned int ret = METADATA_EXTRACTOR_ERROR_NONE;
	MMHandleType content = 0;
	char *err_attr_name = NULL;

	int _audio_track_cnt = 0;
	int _video_track_cnt = 0;

	ret = mm_file_create_content_attrs_from_memory(&content, buffer, size, 0);

	if (ret != FILEINFO_ERROR_NONE) {
		if (ret == FILEINFO_ERROR_FILE_NOT_FOUND) {
			metadata_extractor_error("FILE_NOT_EXISTS(0x%08x)", METADATA_EXTRACTOR_ERROR_FILE_EXISTS);
			return METADATA_EXTRACTOR_ERROR_FILE_EXISTS;
		} else {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	ret = mm_file_get_attrs(content, &err_attr_name,
	                        MM_FILE_CONTENT_VIDEO_TRACK_COUNT, &_video_track_cnt,
	                        MM_FILE_CONTENT_AUDIO_TRACK_COUNT, &_audio_track_cnt,
	                        NULL);

	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		mm_file_destroy_content_attrs(content);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	metadata->attr_h = content;
	metadata->audio_track_cnt = _audio_track_cnt;
	metadata->video_track_cnt = _video_track_cnt;

	return ret;

}

static int __metadata_extractor_create_tag_attrs_from_buffer(metadata_extractor_s *metadata, const void *buffer, int size)
{
	unsigned int ret = METADATA_EXTRACTOR_ERROR_NONE;
	MMHandleType tag = 0;

	ret = mm_file_create_tag_attrs_from_memory(&tag, buffer, size, 0);

	if (ret != FILEINFO_ERROR_NONE) {
		if (ret == FILEINFO_ERROR_FILE_NOT_FOUND) {
			metadata_extractor_error("FILE_NOT_EXISTS(0x%08x)", METADATA_EXTRACTOR_ERROR_FILE_EXISTS);
			return METADATA_EXTRACTOR_ERROR_FILE_EXISTS;
		} else {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	metadata->tag_h = tag;

	return ret;

}

static int __metadata_extractor_get_duration(metadata_extractor_s *metadata, int *duration)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _duration = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_DURATION, &_duration, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*duration = _duration;

	return ret;
}

static int __metadata_extractor_get_audio_bitrate(metadata_extractor_s *metadata, int *bitrate)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _audio_bitrate = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->audio_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_AUDIO_BITRATE, &_audio_bitrate, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*bitrate = _audio_bitrate;

	return ret;
}

static int __metadata_extractor_get_audio_channel(metadata_extractor_s *metadata, int *channel)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _audio_channel = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->audio_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_AUDIO_CHANNELS, &_audio_channel, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*channel = _audio_channel;

	return ret;
}

static int __metadata_extractor_get_audio_samplerate(metadata_extractor_s *metadata, int *samplerate)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _audio_samplerate = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->audio_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_AUDIO_SAMPLERATE, &_audio_samplerate, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*samplerate = _audio_samplerate;

	return ret;
}

static int __metadata_extractor_get_audio_bitpersample(metadata_extractor_s *metadata, int *bitpersample)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _audio_bit_per_sample = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->audio_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_AUDIO_BITPERSAMPLE, &_audio_bit_per_sample, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*bitpersample = _audio_bit_per_sample;

	return ret;
}

static int __metadata_extractor_get_audio_track_count(metadata_extractor_s *metadata, int *track_cnt)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	if (!metadata) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	*track_cnt = metadata->audio_track_cnt;

	return ret;
}

static int __metadata_extractor_get_video_bitrate(metadata_extractor_s *metadata, int *bitrate)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _video_bitrate = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->video_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_VIDEO_BITRATE, &_video_bitrate, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}
	*bitrate = _video_bitrate;

	return ret;
}

static int __metadata_extractor_get_video_FPS(metadata_extractor_s *metadata, int *fps)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _video_fps = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->video_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_VIDEO_FPS, &_video_fps, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*fps = _video_fps;

	return ret;
}

static int __metadata_extractor_get_video_width(metadata_extractor_s *metadata, int *width)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _video_width = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->video_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_VIDEO_WIDTH, &_video_width, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*width = _video_width;

	return ret;
}

static int __metadata_extractor_get_video_height(metadata_extractor_s *metadata, int *height)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _video_height = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->video_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_VIDEO_HEIGHT, &_video_height, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*height = _video_height;

	return ret;
}

static int __metadata_extractor_get_video_thumbnail(metadata_extractor_s *metadata, void **thumbnail, int *thumbnail_len)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	void *_video_thumbnail = NULL;
	int _video_thumbnail_len = 0;

	if ((!metadata) || (!metadata->attr_h)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (metadata->video_track_cnt > 0) {
		ret = mm_file_get_attrs(metadata->attr_h, &err_attr_name, MM_FILE_CONTENT_VIDEO_THUMBNAIL, &_video_thumbnail, &_video_thumbnail_len, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*thumbnail = _video_thumbnail;
	*thumbnail_len = _video_thumbnail_len;

	return ret;
}

static int __metadata_extractor_get_video_track_count(metadata_extractor_s *metadata, int *track_cnt)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	if (!metadata) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	*track_cnt = metadata->video_track_cnt;

	return ret;
}

static int __metadata_extractor_get_artist(metadata_extractor_s *metadata, char **artist)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_artist = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_ARTIST, &_artist, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*artist = _artist;

	return ret;
}

static int __metadata_extractor_get_title(metadata_extractor_s *metadata, char **title)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_title = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_TITLE, &_title, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*title = _title;

	return ret;
}

static int __metadata_extractor_get_album(metadata_extractor_s *metadata, char **album)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_album = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_ALBUM, &_album, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*album = _album;

	return ret;
}

static int __metadata_extractor_get_album_artist(metadata_extractor_s *metadata, char **album_artist)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_album_artist = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name, MM_FILE_TAG_ALBUM_ARTIST, &_album_artist, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*album_artist = _album_artist;

	return ret;
}

static int __metadata_extractor_get_genre(metadata_extractor_s *metadata, char **genre)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_genre = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_GENRE, &_genre, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*genre = _genre;

	return ret;
}

static int __metadata_extractor_get_author(metadata_extractor_s *metadata, char **author)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_author = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_AUTHOR, &_author, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*author = _author;

	return ret;
}

static int __metadata_extractor_get_copyright(metadata_extractor_s *metadata, char **copyright)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_copyright = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_COPYRIGHT, &_copyright, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*copyright = _copyright;

	return ret;
}

static int __metadata_extractor_get_date(metadata_extractor_s *metadata, char **date)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_date = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_DATE, &_date, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*date = _date;

	return ret;
}

static int __metadata_extractor_get_description(metadata_extractor_s *metadata, char **description)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_description = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_DESCRIPTION, &_description, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*description = _description;

	return ret;
}

static int __metadata_extractor_get_comment(metadata_extractor_s *metadata, char **comment)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_comment = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name, MM_FILE_TAG_COMMENT, &_comment, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*comment = _comment;

	return ret;
}

static int __metadata_extractor_get_artwork(metadata_extractor_s *metadata, void **artwork, int *artwork_size)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	void *_artwork = NULL;
	int _artwork_size = 0;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name, MM_FILE_TAG_ARTWORK_SIZE, &_artwork_size, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	if (_artwork_size > 0) {
		ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_ARTWORK, &_artwork, &_tag_len, NULL);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			SAFE_FREE(err_attr_name);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	*artwork = _artwork;
	*artwork_size = _artwork_size;

	return ret;
}

static int __metadata_extractor_get_artwork_mime(metadata_extractor_s *metadata, char **artwork_mime)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_artwork_mime = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_ARTWORK_MIME, &_artwork_mime, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*artwork_mime = _artwork_mime;

	return ret;
}


static int __metadata_extractor_get_track_num(metadata_extractor_s *metadata, char **track_num)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_track_num = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_TRACK_NUM, &_track_num, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*track_num = _track_num;

	return ret;
}

static int __metadata_extractor_get_classification(metadata_extractor_s *metadata, char **classification)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_classification = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_CLASSIFICATION, &_classification, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*classification = _classification;

	return ret;
}

static int __metadata_extractor_get_rating(metadata_extractor_s *metadata, char **rating)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_rating = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_RATING, &_rating, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*rating = _rating;

	return ret;
}

static int __metadata_extractor_get_longitude(metadata_extractor_s *metadata, double *longitude)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	double _longitude = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_LONGITUDE, &_longitude, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*longitude = _longitude;

	return ret;
}

static int __metadata_extractor_get_latitude(metadata_extractor_s *metadata, double *latitude)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	double _latitude = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_LATIDUE, &_latitude, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*latitude = _latitude;

	return ret;
}

static int __metadata_extractor_get_altitude(metadata_extractor_s *metadata, double *altitude)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	double _altitude = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_ALTIDUE, &_altitude, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*altitude = _altitude;

	return ret;
}

static int __metadata_extractor_get_conductor(metadata_extractor_s *metadata, char **conductor)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_conductor = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_CONDUCTOR, &_conductor, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*conductor = _conductor;

	return ret;
}
static int __metadata_extractor_get_unsynclyrics(metadata_extractor_s *metadata, char **unsynclyrics)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_unsynclyrics = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_UNSYNCLYRICS, &_unsynclyrics, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*unsynclyrics = _unsynclyrics;

	return ret;
}

static int __metadata_extractor_get_recording_date(metadata_extractor_s *metadata, char **rec_date)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_rec_date = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_RECDATE, &_rec_date, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*rec_date = _rec_date;

	return ret;
}

static int __metadata_extractor_get_rotate(metadata_extractor_s *metadata, char **rotate)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	char *_rotate = NULL;
	int _tag_len = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name,	MM_FILE_TAG_ROTATE, &_rotate, &_tag_len, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*rotate = _rotate;

	return ret;
}

static int __metadata_extractor_get_synclyrics_pair_num(metadata_extractor_s *metadata, int *synclyrics_num)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	char *err_attr_name = NULL;
	int _synclyrics_num = 0;

	ret = mm_file_get_attrs(metadata->tag_h, &err_attr_name, MM_FILE_TAG_SYNCLYRICS_NUM, &_synclyrics_num, NULL);
	if (ret != FILEINFO_ERROR_NONE) {
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		SAFE_FREE(err_attr_name);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	*synclyrics_num = _synclyrics_num;

	return ret;
}

static int __metadata_extractor_destroy_handle(metadata_extractor_s *metadata)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	if (metadata->attr_h) {
		ret = mm_file_destroy_content_attrs(metadata->attr_h);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	if (metadata->tag_h) {
		ret = mm_file_destroy_tag_attrs(metadata->tag_h);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	}

	return ret;
}


int metadata_extractor_create(metadata_extractor_h *metadata)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;

	if (metadata == NULL) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	metadata_extractor_s *_metadata = (metadata_extractor_s *)calloc(1, sizeof(metadata_extractor_s));
	if (_metadata == NULL) {
		metadata_extractor_error("OUT_OF_MEMORY(0x%08x)", METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY);
		return METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY;
	}

	_metadata->path = NULL;
	_metadata->buffer = NULL;
	_metadata->buffer_size = 0;
	_metadata->audio_track_cnt = 0;
	_metadata->video_track_cnt = 0;

	*metadata = (metadata_extractor_h)_metadata;

	return ret;
}

int metadata_extractor_set_path(metadata_extractor_h metadata, const char *path)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;

	if ((_metadata == NULL) || (path == NULL)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	/*check file path.. */
	if (g_file_test(path, G_FILE_TEST_IS_REGULAR)) {
		metadata_extractor_sucure_info("path[%s] is exists.", path);
	} else {
		metadata_extractor_sucure_info("path[%s] is not exists.", path);
		return METADATA_EXTRACTOR_ERROR_FILE_EXISTS;
	}

	if ((_metadata->path != NULL) || (_metadata->buffer != NULL)) {
		__metadata_extractor_destroy_handle(_metadata);
		SAFE_FREE(_metadata->path);
		_metadata->attr_h = 0;
		_metadata->tag_h = 0;
	}

	_metadata->path = strdup(path);
	if (_metadata->path == NULL) {
		metadata_extractor_error("OUT_OF_MEMORY(0x%08x)", METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY);
		return METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY;
	}

	return ret;
}

int metadata_extractor_set_buffer(metadata_extractor_h metadata, const void *buffer, int size)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;

	if ((_metadata == NULL) || (buffer == NULL) || (size <= 0)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if ((_metadata->path != NULL) || (_metadata->buffer != NULL)) {
		__metadata_extractor_destroy_handle(_metadata);
		SAFE_FREE(_metadata->path);
		_metadata->attr_h = 0;
		_metadata->tag_h = 0;
	}

	_metadata->buffer = (void *)buffer;
	_metadata->buffer_size = size;

	return ret;
}

int metadata_extractor_destroy(metadata_extractor_h metadata)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;

	if (!_metadata) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	ret = __metadata_extractor_destroy_handle(_metadata);

	SAFE_FREE(_metadata->path);

	SAFE_FREE(_metadata);

	return ret;
}

int metadata_extractor_get_synclyrics(metadata_extractor_h metadata, int index, unsigned long *time_stamp, char **lyrics)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;
	unsigned long _time_info = 0;
	char *_lyrics = NULL;
	int _synclyrics_num = 0;

	if ((!_metadata) || ((!_metadata->path) && (!_metadata->buffer))) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	ret = __metadata_extractor_check_and_extract_meta(_metadata, METADATA_TYPE_TAG);
	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		return ret;
	}

	ret = __metadata_extractor_get_synclyrics_pair_num(_metadata, &_synclyrics_num);
	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		return ret;
	}

	if ((_synclyrics_num > 0) && (index < _synclyrics_num)) {
		ret = mm_file_get_synclyrics_info((MMHandleType)_metadata->tag_h, index, &_time_info, &_lyrics);
		if (ret != FILEINFO_ERROR_NONE) {
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
		if ((_lyrics != NULL) && (strlen(_lyrics) > 0)) {
			*lyrics = strdup(_lyrics);
			if (*lyrics == NULL) {
				metadata_extractor_error("OUT_OF_MEMORY(0x%08x)", METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY);
				return METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY;
			}
		} else {
			*lyrics = NULL;
		}

		*time_stamp = _time_info;
	} else {
		*lyrics = NULL;
		*time_stamp = 0;
	}

	return ret;
}

int metadata_extractor_get_metadata(metadata_extractor_h metadata, metadata_extractor_attr_e attribute, char **value)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;
	int i_value = 0;
	double d_value = 0;
	char *s_value = NULL;
	int is_string = 0;
	int is_double = 0;

	if ((!_metadata) || ((!_metadata->path) && (!_metadata->buffer))) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if ((attribute == METADATA_HAS_VIDEO) || (attribute == METADATA_HAS_AUDIO)) {
		if (_metadata->path != NULL)
			ret = __metadata_extractor_check_and_extract_meta(_metadata, METADATA_TYPE_STREAM);
		else
			ret = __metadata_extractor_check_and_extract_meta(_metadata, METADATA_TYPE_ATTR);
	} else if ((attribute >= METADATA_DURATION) && (attribute < METADATA_HAS_AUDIO)) {
		ret = __metadata_extractor_check_and_extract_meta(_metadata, METADATA_TYPE_ATTR);
	} else if ((attribute > METADATA_HAS_AUDIO) && (attribute <= METADATA_ROTATE)) {
		ret = __metadata_extractor_check_and_extract_meta(_metadata, METADATA_TYPE_TAG);
	} else {
		metadata_extractor_error("INVALID_PARAMETER [%d]", attribute);
		ret = METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		return ret;
	}

	/*metadata_extractor_debug("attribute [%d]", attribute); */

	switch (attribute) {
		case METADATA_DURATION: {
			is_string = 0;
			ret = __metadata_extractor_get_duration(_metadata, &i_value);
			break;
		}
		case METADATA_VIDEO_BITRATE: {
			is_string = 0;
			ret = __metadata_extractor_get_video_bitrate(_metadata, &i_value);
			break;
		}
		case METADATA_VIDEO_FPS: {
			is_string = 0;
			ret = __metadata_extractor_get_video_FPS(_metadata, &i_value);
			break;
		}
		case METADATA_VIDEO_WIDTH: {
			is_string = 0;
			ret = __metadata_extractor_get_video_width(_metadata, &i_value);
			break;
		}
		case METADATA_VIDEO_HEIGHT: {
			is_string = 0;
			ret = __metadata_extractor_get_video_height(_metadata, &i_value);
			break;
		}
		case METADATA_HAS_VIDEO: {
			is_string = 0;
			ret = __metadata_extractor_get_video_track_count(_metadata, &i_value);
			break;
		}
		case METADATA_AUDIO_BITRATE: {
			is_string = 0;
			ret = __metadata_extractor_get_audio_bitrate(_metadata, &i_value);
			break;
		}
		case METADATA_AUDIO_CHANNELS: {
			is_string = 0;
			ret = __metadata_extractor_get_audio_channel(_metadata, &i_value);
			break;
		}
		case METADATA_AUDIO_SAMPLERATE: {
			is_string = 0;
			ret = __metadata_extractor_get_audio_samplerate(_metadata, &i_value);
			break;
		}
		case METADATA_AUDIO_BITPERSAMPLE: {
			is_string = 0;
			ret = __metadata_extractor_get_audio_bitpersample(_metadata, &i_value);
			break;
		}
		case METADATA_HAS_AUDIO: {
			is_string = 0;
			ret = __metadata_extractor_get_audio_track_count(_metadata, &i_value);
			break;
		}
		case METADATA_ARTIST: {
			is_string = 1;
			ret = __metadata_extractor_get_artist(_metadata, &s_value);
			break;
		}
		case METADATA_TITLE: {
			is_string = 1;
			ret = __metadata_extractor_get_title(_metadata, &s_value);
			break;
		}
		case METADATA_ALBUM: {
			is_string = 1;
			ret = __metadata_extractor_get_album(_metadata, &s_value);
			break;
		}
		case METADATA_ALBUM_ARTIST: {
			is_string = 1;
			ret = __metadata_extractor_get_album_artist(_metadata, &s_value);
			break;
		}
		case METADATA_GENRE: {
			is_string = 1;
			ret = __metadata_extractor_get_genre(_metadata, &s_value);
			break;
		}
		case METADATA_AUTHOR: {
			is_string = 1;
			ret = __metadata_extractor_get_author(_metadata, &s_value);
			break;
		}
		case METADATA_COPYRIGHT: {
			is_string = 1;
			ret = __metadata_extractor_get_copyright(_metadata, &s_value);
			break;
		}
		case METADATA_DATE: {
			is_string = 1;
			ret = __metadata_extractor_get_date(_metadata, &s_value);
			break;
		}
		case METADATA_DESCRIPTION: {
			is_string = 1;
			ret = __metadata_extractor_get_description(_metadata, &s_value);
			break;
		}
		case METADATA_COMMENT: {
			is_string = 1;
			ret = __metadata_extractor_get_comment(_metadata, &s_value);
			break;
		}
		case METADATA_TRACK_NUM: {
			is_string = 1;
			ret = __metadata_extractor_get_track_num(_metadata, &s_value);
			break;
		}
		case METADATA_CLASSIFICATION: {
			is_string = 1;
			ret = __metadata_extractor_get_classification(_metadata, &s_value);
			break;
		}
		case METADATA_RATING: {
			is_string = 1;
			ret = __metadata_extractor_get_rating(_metadata, &s_value);
			break;
		}
		case METADATA_LONGITUDE: {
			is_double = 1;
			ret = __metadata_extractor_get_longitude(_metadata, &d_value);
			break;
		}
		case METADATA_LATITUDE: {
			is_double = 1;
			ret = __metadata_extractor_get_latitude(_metadata, &d_value);
			break;
		}
		case METADATA_ALTITUDE: {
			is_double = 1;
			ret = __metadata_extractor_get_altitude(_metadata, &d_value);
			break;
		}
		case METADATA_CONDUCTOR: {
			is_string = 1;
			ret = __metadata_extractor_get_conductor(_metadata, &s_value);
			break;
		}
		case METADATA_UNSYNCLYRICS: {
			is_string = 1;
			ret = __metadata_extractor_get_unsynclyrics(_metadata, &s_value);
			break;
		}
		case METADATA_SYNCLYRICS_NUM: {
			is_string = 0;
			ret = __metadata_extractor_get_synclyrics_pair_num(_metadata, &i_value);
			break;
		}
		case METADATA_RECDATE: {
			is_string = 1;
			ret = __metadata_extractor_get_recording_date(_metadata, &s_value);
			break;
		}
		case METADATA_ROTATE: {
			is_string = 1;
			ret = __metadata_extractor_get_rotate(_metadata, &s_value);
			break;
		}
		default :
			break;
	}

	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		*value = NULL;
		return ret;
	}

	if (is_string) {
		if ((s_value != NULL) && (strlen(s_value) > 0)) {
			*value = strdup(s_value);
			if (*value == NULL) {
				metadata_extractor_error("OUT_OF_MEMORY(0x%08x)", METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY);
				return METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY;
			}
		} else {
			*value = NULL;
		}
	} else {
		char extracted_metadata[META_MAX_LEN] = {0, };

		if (is_double) {
			snprintf(extracted_metadata, sizeof(extracted_metadata), "%f", d_value);
		} else {
			snprintf(extracted_metadata, sizeof(extracted_metadata), "%d", i_value);
		}

		*value = strdup(extracted_metadata);
		if (*value == NULL) {
			metadata_extractor_error("OUT_OF_MEMORY(0x%08x)", METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY);
			return METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY;
		}
	}

	return ret;
}

int metadata_extractor_get_artwork(metadata_extractor_h metadata, void **artwork, int *size, char **mime_type)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;
	void *_artwork = NULL;
	int _artwork_size = 0;
	char *_artwork_mime = NULL;

	if ((!_metadata) || ((!_metadata->path) && (!_metadata->buffer))) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	ret = __metadata_extractor_check_and_extract_meta(_metadata, METADATA_TYPE_TAG);
	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		return ret;
	}

	ret = __metadata_extractor_get_artwork(_metadata, &_artwork, &_artwork_size);
	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		return ret;
	}

	if ((_artwork_size > 0) && (_artwork != NULL)) {
		ret = __metadata_extractor_get_artwork_mime(_metadata, &_artwork_mime);
		if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
			*artwork = NULL;
			*size = 0;

			return ret;
		}

		*artwork = calloc(1, _artwork_size);
		if (*artwork != NULL) {
			memcpy(*artwork, _artwork, _artwork_size);
			*size = _artwork_size;
		} else {
			*size = 0;
		}

		if ((_artwork_mime != NULL) && (strlen(_artwork_mime) > 0)) {
			*mime_type = strdup(_artwork_mime);
			if (*mime_type == NULL) {
				metadata_extractor_error("OUT_OF_MEMORY(0x%08x)", METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY);
				return METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY;
			}
		} else {
			*mime_type = NULL;
		}

	} else {
		*artwork = NULL;
		*mime_type = NULL;
		*size = 0;
	}

	return ret;
}

int metadata_extractor_get_frame(metadata_extractor_h metadata, void **frame, int *size)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;
	void *_frame = NULL;
	int _frame_size = 0;

	if ((!_metadata) || ((!_metadata->path) && (!_metadata->buffer)) || (!size)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	ret = __metadata_extractor_check_and_extract_meta(_metadata, METADATA_TYPE_ATTR);
	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		return ret;
	}

	ret = __metadata_extractor_get_video_thumbnail(_metadata, &_frame, &_frame_size);
	if (ret != METADATA_EXTRACTOR_ERROR_NONE) {
		return ret;
	}

	if ((_frame_size > 0) && (_frame != NULL)) {
		*frame = calloc(1, _frame_size);
		if (*frame == NULL) {
			metadata_extractor_error("OUT_OF_MEMORY(0x%08x)", METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY);
			return METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY;
		}
		memcpy(*frame, _frame, _frame_size);
		*size = _frame_size;
	} else {
		*frame = NULL;
		*size = 0;
	}

	return ret;
}

int metadata_extractor_get_frame_at_time(metadata_extractor_h metadata, unsigned long timestamp, bool is_accurate, void **frame, int *size)
{
	int ret = METADATA_EXTRACTOR_ERROR_NONE;
	metadata_extractor_s *_metadata = (metadata_extractor_s *)metadata;
	void *_frame = NULL;
	int _frame_size = 0;
	int width = 0;
	int height = 0;
	long long micro_timestamp = 0;

	if ((!_metadata) || ((!_metadata->path) && (!_metadata->buffer)) || (!size)) {
		metadata_extractor_error("INVALID_PARAMETER(0x%08x)", METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER);
		return METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER;
	}

	micro_timestamp = (long long)timestamp * 1000;

	metadata_extractor_debug("accurate [%d] timestamp[%d] micro_timestamp[%lld]", is_accurate, timestamp, micro_timestamp);

	if (_metadata->path) {
		ret = mm_file_get_video_frame(_metadata->path, micro_timestamp, is_accurate, (unsigned char **)&_frame, &_frame_size, &width, &height);
	} else {
		ret = mm_file_get_video_frame_from_memory(_metadata->buffer, _metadata->buffer_size, micro_timestamp, is_accurate, (unsigned char **)&_frame, &_frame_size, &width, &height);
	}

	if (ret != FILEINFO_ERROR_NONE) {
		SAFE_FREE(_frame);
		metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED(0x%08x)", ret);
		return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
	}

	if (_frame != NULL) {
		if ((_frame_size > 0)) {
			*frame = _frame;
			*size = _frame_size;
		} else {
			SAFE_FREE(_frame);
			metadata_extractor_error("METADATA_EXTRACTOR_ERROR_OPERATION_FAILED (0x%08x)", METADATA_EXTRACTOR_ERROR_OPERATION_FAILED);
			return METADATA_EXTRACTOR_ERROR_OPERATION_FAILED;
		}
	} else {
		*frame = NULL;
		*size = 0;
	}

	return ret;
}
