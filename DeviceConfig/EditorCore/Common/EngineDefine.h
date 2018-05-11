#ifndef _ENGINE_DEFINE_H_
#define _ENGINE_DEFINE_H_

#include <windows.h>
#include <winsock.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <list>
#include <stdlib.h>
#include <sys/stat.h>
#include <typeinfo>
#include <memory>
#include <time.h>
#include <cmath>
#include <math.h>
#include <sys/types.h>
#include <mmsystem.h>
#include <iphlpapi.h>
#include <comutil.h>  
#include <Wbemidl.h>  
#include <tchar.h>  
#include <strsafe.h>  
#include <algorithm>  
#include <atlconv.h>  
#include <ntddndis.h>
#include <winioctl.h>

//角度转弧度
#define AngleToRadian(angle) angle * 3.141592654 / 180.0
//弧度转角度
#define RadianToAngle(radian) radian * 180.0 / 3.141592654
#define TX_MAX(x, y) ((x) > (y) ? (x) : (y))
#define TX_MIN(x, y) ((x) < (y) ? (x) : (y))
#define SPRINTF sprintf_s
#define DEBUG_EMPTY "%s", ""
#define TOSTRING(t) #t
#define COMMAND_PARAM __FILE__, __LINE__
#define COMMAND_CONSTRUCT(className, baseCommand)					\
	className(const char* file, int line, bool showInfo = true)		\
	:																\
	baseCommand(file, line, showInfo)

#define COMMAND_EDITOR_CORE_CONSTRUCT(className) COMMAND_CONSTRUCT(className, EditorCoreCommand)

#define COMMAND_DEBUG(...)																\
{																						\
	static char strBuf[256];															\
	sprintf_s(strBuf, 256, __VA_ARGS__);												\
	std::string cmdName = typeid(*this).name();											\
	cmdName = cmdName.substr(strlen("class "), cmdName.length() - strlen("class "));	\
	std::string str = cmdName;															\
	if (strlen(strBuf) > 0)																\
	{																					\
		str += std::string(" : ") + strBuf;												\
	}																					\
	return str;																			\
}

#endif