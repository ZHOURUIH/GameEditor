#ifndef _QUERY_GYM_H_
#define _QUERY_GYM_H_

#include "CommonDefine.h"
#include "LibCurlPacket.h"

class QueryGym : public LibCurlPacket
{
public:
	QueryGym(){}
	virtual ~QueryGym(){}
	virtual void write(std::string& stream);
	virtual void execute(const std::string& value);
	virtual int getPacketSize()
	{
		return mRegisteCode.length() + 1;
	}
public:
	std::string mRegisteCode;
};

#endif