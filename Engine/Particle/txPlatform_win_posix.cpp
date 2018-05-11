//-----------------------------------------------------------------------------
// File: platform_win_posix.cpp
//
// Copyright (c) Astralax. All rights reserved.
// Author: Sedov Alexey
//-----------------------------------------------------------------------------

#include "txPlatform_win_posix.h"

#ifndef _WINDOWS
#include <sys/time.h>
#include <unistd.h>
#endif

#define MAX_PATH_LEN	2048

#include "txEngineLog.h"
#include "txEngineRoot.h"

MP_Platform_WIN_POSIX::MP_Platform_WIN_POSIX()
{
	#ifndef _WINDOWS
	dir=NULL;
	#endif
}

MP_Platform_WIN_POSIX::~MP_Platform_WIN_POSIX()
{
}

// eng: Returns time in milliseconds
// rus: Возвращает время в милисекундах
unsigned long MP_Platform_WIN_POSIX::GetTick()
{
	#ifdef _WINDOWS
	return GetTickCount();
	#else
	timeval time_val;
	gettimeofday(&time_val, NULL);

	return time_val.tv_sec*1000+time_val.tv_usec/1000;
	#endif
}

// eng: Returns path to folder with emitters
// rus: Возвращает путь к папке с ptc-файлами
const char* MP_Platform_WIN_POSIX::GetPathToPTC()
{
	ptc_path = "ParticleResources/";
	return ptc_path.c_str();
}

// eng: Returns path to folder with textures
// rus: Возвращает путь к папке с текстурами
const char* MP_Platform_WIN_POSIX::GetPathToTexture()
{
	if (texture_path.empty())
	{
		texture_path = GetPathToPTC();
	}

	return texture_path.c_str();
}

// eng: Returns path to folder which could be used by wrapper to store temporary files. If NULL is returned the temporary files are saved in RAM.
// rus: Возвращает путь к временной папке. Если вернуть NULL, то временные файлы будут сохраняться в ОЗУ.
const char* MP_Platform_WIN_POSIX::GetPathToTemp()
{
	#ifdef _WINDOWS
	if (temp_path.empty())
	{
		wchar_t path[MAX_PATH_LEN];
		GetTempPathW(MAX_PATH_LEN, path);
		temp_path=wchar_to_utf8(path);
	}

	return temp_path.c_str();

	#else

	return NULL;
    
	#endif
}

// eng: Finds the first ptc-file in emitters folder
// rus: Поиск первого ptc-файла в папке с эмиттерами
const char* MP_Platform_WIN_POSIX::GetFirstFile()
{
	std::string ptc_path=GetPathToPTC();

	#ifdef _WINDOWS
	ptc_path+="*.ptc";
	const wchar_t* mask=utf8_to_wchar(ptc_path.c_str());
	hFindFile=FindFirstFileW(mask,&fd);
	if (hFindFile!=INVALID_HANDLE_VALUE)
	{
		file=wchar_to_utf8(fd.cFileName);
		return file.c_str();
	}
	FindClose(hFindFile);

	#else

	dir=opendir(ptc_path.c_str());
	if (dir)
		return GetNextFile();

	#endif

	return NULL;
}

// eng: Finds the next ptc-file in emitters folder
// rus: Поиск следующего ptc-файла в папке с эмиттерами
const char* MP_Platform_WIN_POSIX::GetNextFile()
{
	#ifdef _WINDOWS

	if (FindNextFileW(hFindFile,&fd))
	{
		file=wchar_to_utf8(fd.cFileName);
		return file.c_str();
	}
	FindClose(hFindFile);

	#else

	dirent* result=readdir(dir);
	while(result)
	{
		int len=strlen(result->d_name);
		if (len>4)
		{
			const char* ext=&(result->d_name[len-4]);
			if (strcmp(ext, ".ptc")==0)
			{
				return result->d_name;
			}
		}
		result=readdir(dir);
	}
	closedir(dir);
	dir=NULL;
	
	#endif

	return NULL;
}

// ru: Удаление файла (аналог "remove")
// en: Deleting of file (analogue "remove")
int MP_Platform_WIN_POSIX::RemoveFile(const char* file)
{
	#ifdef _WINDOWS

	return _wremove(utf8_to_wchar(file));

	#else

	return remove(file);

	#endif
}
HM_FILE MP_Platform_WIN_POSIX::OpenPTC(const char* ptc_file)
{
	int fileLength = 0;
	char* buffer = txFileUtility::openBinaryFile(ptc_file, &fileLength);
	if (buffer == NULL)
	{
		ENGINE_ERROR("error : can not load file : %s", ptc_file);
		return 0;
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
