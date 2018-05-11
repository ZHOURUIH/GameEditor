#include "EventSystem.h"
#include "EditorCoreLog.h"

EventSystem::EventSystem()
{
	ADD_PARAM_COUNT(CET_ERROR_LOG, 1);
	ADD_PARAM_COUNT(CET_INFO_LOG, 1);
	ADD_PARAM_COUNT(CET_DOWNLOAD_VERSION, 1);
	ADD_PARAM_COUNT(CET_DOWNLOAD_LIST_FILE, 1);
	ADD_PARAM_COUNT(CET_START_GENERATE_LOCAL_FILE, 1);
	ADD_PARAM_COUNT(CET_GENERATING_LOCAL_FILE, 2);
	ADD_PARAM_COUNT(CET_FINISH_GENERATE_MODIFY_FILE, 1);
	if (mEventParamCountList.size() < CET_MAX)
	{
		EDITOR_CORE_ERROR("error : all events must be assigned parameter count! cur count : %d, max count : %d", mEventParamCountList.size(), CET_MAX);
	}
}