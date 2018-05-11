#ifndef __TX_LOG_H__
#define __TX_LOG_H__

#define LOGI(...)												\
{																\
	char buffer[2048];											\
	sprintf_s(buffer, 2048, __VA_ARGS__);						\
	OutputDebugStringA((std::string(buffer) + "\n").c_str());	\
}

#define LOGW(...)												\
{																\
	char buffer[2048];											\
	sprintf_s(buffer, 2048, __VA_ARGS__);						\
	OutputDebugStringA((std::string(buffer) + "\n").c_str());	\
}

#endif