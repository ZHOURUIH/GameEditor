#include "EditorCoreUtility.h"
#include "EditorCore.h"

void EditorCoreUtility::logError(const std::string& info, const bool& delay)
{
	EditorCore::getSingletonPtr()->logError(info, delay);
}

void EditorCoreUtility::logInfo(const std::string& info, const bool& delay)
{
	EditorCore::getSingletonPtr()->logInfo(info, delay);
}