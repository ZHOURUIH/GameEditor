#ifndef _DATA_H_
#define _DATA_H_

#include "txSerializedData.h"

#include "DataDefine.h"

class Data : public txSerializedData
{
public:
	Data(const DATA_TYPE& type)
	:
	mType(type)
	{}
	const DATA_TYPE& getType() { return mType; }
protected:
	DATA_TYPE mType;
};

#endif