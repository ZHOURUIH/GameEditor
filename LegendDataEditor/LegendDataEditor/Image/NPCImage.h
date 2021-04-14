#ifndef _NPC_IMAGE_H_
#define _NPC_IMAGE_H_

#include "ServerBase.h"
#include "ImageDefine.h"

class NPCImage : public ServerBase
{
public:
	int mPosX;
	int mPosY;
	string mActionName;
	int mDirection;
	int mFrameIndex;		// 在当前动作中的下标
	int mID;
	int mActionIndex;		// 动作在数组中的下标
public:
	void setFileName(const string& fileName);
	bool isValidImage() const { return mFrameIndex < NPC_ACTION[mActionIndex].mFrameCount; }
};

#endif