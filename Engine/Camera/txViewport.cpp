#include "txViewport.h"
#include "txEngineDefine.h"
#include "txCamera.h"

txViewport::txViewport(txCamera* camera)
{
	mCamera = camera;
	mStartX = 0;
	mStartY = 0;
	mWidth = 0;
	mHeight = 0;
}

void txViewport::setViewport(const int& startX, const int& startY, const int& width, const int& height)
{
	mStartX = startX;
	mStartY = startY;
	mWidth = width;
	mHeight = height;
	applyViewport();
	mCamera->setAspect((float)mWidth / (float)mHeight);
}

void txViewport::applyViewport()
{
	glViewport(mStartX, mStartY, mWidth, mHeight);
}