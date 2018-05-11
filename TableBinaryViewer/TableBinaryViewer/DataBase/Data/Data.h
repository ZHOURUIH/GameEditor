#ifndef _DATA_H_
#define _DATA_H_

#include "txSerializedData.h"

class Data : public txSerializedData
{
public:
	Data::Data(const std::string& type)
		:
		mType(type)
	{}
	const std::string& getType() const { return mType; }
	virtual void fillParams(){}
protected:
	std::string mType;
};

#endif