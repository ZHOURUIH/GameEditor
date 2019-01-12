#ifndef _MONSTER_IMAGE_H_
#define _MONSTER_IMAGE_H_

#include "ServerDefine.h"
#include "ImageDefine.h"

class MonsterImage
{
public:
	int mPosX;
	int mPosY;
	string mLabel;
	string mActionName;
	int mDirection;
	int mFrameIndex;		// �ڵ�ǰ�����е��±�
	int mMonsterID;
	int mActionIndex;		// �����������е��±�
public:
	void setFileName(const string& fileName);
	bool isValidImage() const { return mFrameIndex < MONSTER_ACTION[mActionIndex].mFrameCount; }
};

#endif