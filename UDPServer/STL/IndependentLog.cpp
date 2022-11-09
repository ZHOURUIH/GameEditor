#include "IndependentLog.h"
#include "CommonDefine.h"

void IndependentLog::directError(const string& info)
{
	ERROR(info);
}

void IndependentLog::directLog(const string& info)
{
	LOG(info);
}