//-----------------------------------------------------------------------------
// File: platform_android.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#include "txPlatform_android.h"
#include "txEngineRoot.h"

#if RUN_PLATFORM == PLATFORM_ANDROID
#include <android/asset_manager.h>

//extern AAssetManager* asset_mgr;

MP_Platform_ANDROID::MP_Platform_ANDROID() : MP_Platform_WIN_POSIX()
{
	//asset_dir=NULL;
}

MP_Platform_ANDROID::~MP_Platform_ANDROID()
{
}

// eng: Returns path to folder with emitters
// rus: Возвращает путь к папке с ptc-файлами
const char* MP_Platform_ANDROID::GetPathToPTC()
{
	return MP_Platform_WIN_POSIX::GetPathToPTC();
}

// eng: Finds the first ptc-file in emitters folder
// rus: Поиск первого ptc-файла в папке с эмиттерами
const char* MP_Platform_ANDROID::GetFirstFile()
{
//	std::string ptc_path=GetPathToPTC();
//
//	asset_dir=AAssetManager_openDir(asset_mgr, ptc_path.c_str());
//
//	if (asset_dir)
//		return GetNextFile();
	return NULL;
}

// eng: Finds the next ptc-file in emitters folder
// rus: Поиск следующего ptc-файла в папке с эмиттерами
const char* MP_Platform_ANDROID::GetNextFile()
{
//	const char* result=AAssetDir_getNextFileName(asset_dir);
//	while(result)
//	{
//		int len=strlen(result);
//		if (len>4)
//		{
//			const char* ext=&(result[len-4]);
//			if (strcmp(ext, ".ptc")==0)
//				return result;
//		}
//		result=AAssetDir_getNextFileName(asset_dir);
//	}
//	AAssetDir_close(asset_dir);
//	asset_dir=NULL;
	return NULL;
}

// eng: Opens ptc-file
// rus: Открывает ptc-файл
HM_FILE MP_Platform_ANDROID::OpenPTC(const char* ptc_file)
{
	int fileLength = 0;
	char* buffer = txFileUtility::openBinaryFile(ptc_file, &fileLength);
	if (buffer == NULL)
	{
		ENGINE_ERROR("error : can not load file : %s", ptc_file);
		return;
	}
	HM_STREAM hmStream = Magic_StreamOpenMemory(buffer, fileLength, MAGIC_STREAM_READ);
	HM_FILE hmFile = Magic_OpenStream(hmStream);
	if (hmFile > 0)
	{
		MP_PTC* ptc = TRACE_NEW(MP_PTC, ptc);
		ptc->hmFile = hmFile;
		ptc->hmStream = hmStream;
		ptc->data = buffer;
		MP->AddPTC(ptc);
	}
	else
	{
		Magic_StreamClose(hmStream);
		TRACE_DELETE_ARRAY(buffer);
	}
	return hmFile;
}

#endif
