#include "ECUtility.h"
#include "EditorCoreRoot.h"
#include "txEngineRoot.h"
#include "txRenderWindow.h"

void ECUtility::logError(const std::string& info)
{
	if (mEditorCoreRoot == NULL)
	{
		mEditorCoreRoot = EditorCoreRoot::getSingletonPtr();
	}
	mEditorCoreRoot->logError(info);
}

int ECUtility::getScreenWidth()
{
	return mRenderWindow->getWidth();
}

int ECUtility::getScreenHeight()
{
	return mRenderWindow->getHeight();
}

VECTOR2 ECUtility::getScreenSize()
{
	VECTOR2 size;
	size.x = (float)getScreenWidth();
	size.y = (float)getScreenHeight();
	return size;
}