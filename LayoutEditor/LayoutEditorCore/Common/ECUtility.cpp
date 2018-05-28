#include "ECUtility.h"
#include "EditorCoreRoot.h"

void ECUtility::logError(const std::string& info)
{
	EditorCoreRoot::getSingletonPtr()->logError(info);
}