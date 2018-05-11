//-----------------------------------------------------------------------------
// File: platform_win_posix.h
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#ifndef MAGIC_PARTICLES_PLATFORM
#define MAGIC_PARTICLES_PLATFORM

#ifdef _WINDOWS
#include <windows.h>
#else
#include <dirent.h>
#endif
#include "txMP.h"

class MP_Platform_WIN_POSIX : public MP_Platform
{
protected:
	#ifdef _WINDOWS
	WIN32_FIND_DATAW fd;
	HANDLE hFindFile;
	#else
	DIR* dir;
	#endif

	std::string file;

public:
	MP_Platform_WIN_POSIX();
	virtual ~MP_Platform_WIN_POSIX();

	// eng: Returns time in milliseconds
	// rus: Возвращает время в милисекундах
	virtual unsigned long GetTick();

	// eng: Returns path to folder with emitters
	// rus: Возвращает путь к папке с ptc-файлами
	virtual const char* GetPathToPTC();

	// eng: Returns path to folder with textures
	// rus: Возвращает путь к папке с текстурами
	virtual const char* GetPathToTexture();

	// eng: Returns path to folder which could be used by wrapper to store temporary files. If NULL is returned the temporary files are saved in RAM.
	// rus: Возвращает путь к временной папке. Если вернуть NULL, то временные файлы будут сохраняться в ОЗУ.
	virtual const char* GetPathToTemp();

	// eng: Finds the first ptc-file in emitters folder
	// rus: Поиск первого ptc-файла в папке с эмиттерами
	virtual const char* GetFirstFile();

	// eng: Finds the next ptc-file in emitters folder
	// rus: Поиск очередного ptc-файла в папке с эмиттерами
	virtual const char* GetNextFile();

	// ru: Удаление файла (аналог "remove")
	// en: Deleting of file (analogue "remove")
	int RemoveFile(const char* file);
	virtual HM_FILE OpenPTC(const char* ptc_file);
};

#endif
