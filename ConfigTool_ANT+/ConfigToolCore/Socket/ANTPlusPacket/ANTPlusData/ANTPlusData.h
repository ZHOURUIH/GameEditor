#ifndef _ANT_PLUS_DATA_H_
#define _ANT_PLUS_DATA_H_

#include "CommonDefine.h"
#include "ToolCoreBase.h"

class ANTPlusPacket;
class ANTPlusData : public ToolCoreBase
{
public:
	ANTPlusPacket* mParentPacket;
	unsigned char mCmdID;
	char* mBuffer;
	int mLength;
public:
	ANTPlusData(ANTPlusPacket* packet);
	~ANTPlusData();
	virtual void copyData(ANTPlusData* data);
	void setData(char* data);
protected:
	void setLength(int length);
	virtual void parseData() = 0;
};

#endif