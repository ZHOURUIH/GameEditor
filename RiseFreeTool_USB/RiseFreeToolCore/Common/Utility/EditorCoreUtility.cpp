#include "EditorCoreUtility.h"
#include "RiseFreeToolCore.h"

void EditorCoreUtility::logError(const std::string& info, const bool& delay)
{
	RiseFreeToolCore::getSingletonPtr()->logError(info, delay);
}

void EditorCoreUtility::logInfo(const std::string& info, const bool& delay)
{
	RiseFreeToolCore::getSingletonPtr()->logInfo(info, delay);
}