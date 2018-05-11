#include "CTEventSystem.h"
#include "ToolCoreLog.h"

CTEventSystem::CTEventSystem()
{
	ADD_PARAM_COUNT(CET_ERROR_LOG, 1);
	ADD_PARAM_COUNT(CET_INFO_LOG, 1);
	ADD_PARAM_COUNT(CET_DEVICE_CHANGED, 0);
	ADD_PARAM_COUNT(CET_RECEIVE_DATA, 2);
	ADD_PARAM_COUNT(CET_SEND_DATA, 2);
	ADD_PARAM_COUNT(CET_HEART_RATE_TIME, 0);
	if (mEventParamCountList.size() < CET_MAX)
	{
		TOOL_CORE_ERROR("error : all events must be assigned parameter count! cur count : %d, max count : %d", mEventParamCountList.size(), CET_MAX);
	}
}