#ifndef _SETUP_DEVICE_H_
#define _SETUP_DEVICE_H_

#include "CommonDefine.h"
#include "LibcurlPacket.h"

class DeviceInfo;
class SetupDevice : public LibcurlPacket
{
public:
	SetupDevice(){}
	virtual ~SetupDevice(){}
	virtual void write(std::string& stream);
	virtual void execute(const std::string& value);
	void setDeviceInfoList(txMap<int, txMap<char, DeviceInfo*>>& deviceInfoList){ mDeviceInfoList = deviceInfoList; }
public:
	txMap<int, txMap<char, DeviceInfo*>> mDeviceInfoList;
};

#endif