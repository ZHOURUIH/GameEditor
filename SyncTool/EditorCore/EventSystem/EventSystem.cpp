#include "EventSystem.h"
#include "EditorCoreLog.h"

EventSystem::EventSystem()
{
	ADD_PARAM_COUNT(CET_ERROR_LOG, 1);
	ADD_PARAM_COUNT(CET_INFO_LOG, 1);
	ADD_PARAM_COUNT(CET_START_DOWNLOAD_VERSION, 0);
	ADD_PARAM_COUNT(CET_START_DOWNLOAD_LIST_FILE, 0);
	ADD_PARAM_COUNT(CET_NEW_VERSION, 1);
	ADD_PARAM_COUNT(CET_START_DOWNLOAD, 1);
	ADD_PARAM_COUNT(CET_FINISH_DOWNLOAD, 1);
	ADD_PARAM_COUNT(CET_START_GENERATE_LOCAL_FILE, 1);
	ADD_PARAM_COUNT(CET_GENERATING_LOCAL_FILE, 2);
	ADD_PARAM_COUNT(CET_FINISH_GENERATE_LOCAL_FILE, 0);
	ADD_PARAM_COUNT(CET_UPDATING_FILE, 0);
	ADD_PARAM_COUNT(CET_UPDATE_DONE, 0);
	ADD_PARAM_COUNT(CET_CANCEL_UPDATE, 0);
	ADD_PARAM_COUNT(CET_NOTHING_UPDATE, 0);
	ADD_PARAM_COUNT(CET_START_UPLOAD, 1);
	ADD_PARAM_COUNT(CET_UPLOADING_FILE, 1);
	ADD_PARAM_COUNT(CET_FINISH_UPLOAD, 2);
	ADD_PARAM_COUNT(CET_ALL_UPLOADED, 0);
	ADD_PARAM_COUNT(CET_STOP_PROGRAM, 0);
	ADD_PARAM_COUNT(CET_UPDATING_PROGRESS, 1);
	if (mEventParamCountList.size() < CET_MAX)
	{
		EDITOR_CORE_ERROR("error : all events must be assigned parameter count! cur count : %d, max count : %d", mEventParamCountList.size(), CET_MAX);
	}
}