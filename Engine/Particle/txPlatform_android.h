//-----------------------------------------------------------------------------
// File: platform_android.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef _TX_PLATFORM_ANDROID
#define _TX_PLATFORM_ANDROID

#include "txPlatform_win_posix.h"
#include "txEngineDefine.h"
#if RUN_PLATFORM == PLATFORM_ANDROID

class MP_Platform_ANDROID : public MP_Platform_WIN_POSIX
{
protected:
	//AAssetDir* asset_dir;

public:
	MP_Platform_ANDROID();
	virtual ~MP_Platform_ANDROID();

	// eng: Returns path to folder with emitters
	// rus: Возвращает путь к папке с ptc-файлами
	virtual const char* GetPathToPTC();

	// eng: Finds the first ptc-file in emitters folder
	// rus: Поиск первого ptc-файла в папке с эмиттерами
	virtual const char* GetFirstFile();

	// eng: Finds the next ptc-file in emitters folder
	// rus: Поиск очередного ptc-файла в папке с эмиттерами
	virtual const char* GetNextFile();

	// eng: Opens ptc-file
	// rus: Открывает ptc-файл
	virtual HM_FILE OpenPTC(const char* ptc_file);
};

#endif

#endif
