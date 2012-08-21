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


#ifndef __TIZEN_MEDIA_METADATA_EXTRACTOR_PRIVATE_H__
#define __TIZEN_MEDIA_METADATA_EXTRACTOR_PRIVATE_H__

#include <stdbool.h>
#include <mm_types.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef struct
{
	char *path;
	bool extract_meta;

	int audio_track_cnt;
	int video_track_cnt;

	MMHandleType attr_h;
	MMHandleType tag_h;
}metadata_extractor_s;


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TIZEN_MEDIA_METADATA_EXTRACTOR_PRIVATE_H__*/



