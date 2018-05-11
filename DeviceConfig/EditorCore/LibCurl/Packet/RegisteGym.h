#ifndef _REGISTE_GYM_H_
#define _REGISTE_GYM_H_

#include "CommonDefine.h"
#include "LibCurlPacket.h"

class RegisteGym : public LibCurlPacket
{
public:
	RegisteGym(){}
	virtual ~RegisteGym(){}
	virtual void write(std::string& stream);
	virtual int getPacketSize()
	{
		return mGymName.length() + 1 + mRegisteCode.length() + 1;
	}
	virtual void execute(const std::string& value);
public:
	std::string mGymName;
	std::string mRegisteCode;
};

#endif