#include "CTEventSystem.h"
#include "ToolCoreUtility.h"
#include "ToolCoreLog.h"

CTEventSystem::CTEventSystem()
{
	ADD_PARAM_COUNT(CE_ERROR_LOG, 1);
	ADD_PARAM_COUNT(CE_INFO_LOG, 1);
	ADD_PARAM_COUNT(CE_HEART_RATE_TIME, 0);
	ADD_PARAM_COUNT(CE_REGISTE_DEVICE_LIST_MODIFIED, 1);
	ADD_PARAM_COUNT(CE_UPLOAD_STATE, 1);
	ADD_PARAM_COUNT(CE_US_UNUPLOAD, 1);
	ADD_PARAM_COUNT(CE_UPLOAD_FAILED, 1);
	if (mEventParamCountList.size() < CE_MAX)
	{
		TOOL_CORE_ERROR("error : all events must be assigned parameter count! cur count : %d, max count : %d", mEventParamCountList.size(), CE_MAX);
	}
}