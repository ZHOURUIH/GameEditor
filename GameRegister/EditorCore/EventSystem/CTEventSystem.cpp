#include "CTEventSystem.h"
#include "EditorCoreLog.h"

CTEventSystem::CTEventSystem()
{
	ADD_PARAM_COUNT(CET_ERROR_LOG, 1);
	ADD_PARAM_COUNT(CET_INFO_LOG, 1);
	if (mEventParamCountList.size() < CET_MAX)
	{
		EDITOR_CORE_ERROR("error : all events must be assigned parameter count! cur count : %d, max count : %d", mEventParamCountList.size(), CET_MAX);
	}
}