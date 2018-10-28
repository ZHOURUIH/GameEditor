#ifndef _DATA_REGISTE_DEVICE_H_
#define _DATA_REGISTE_DEVICE_H_

#include "Data.h"

class DataRegisteDevice : public Data
{
public:
	DataRegisteDevice(const DATA_TYPE& type)
		:
		Data(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void fillParams()
	{
		pushParam(mMachineIndex, "MachineIndex");
		pushArrayParam(mDeviceID, 4, "DeviceID");
		pushParam(mDeviceType, "DeviceType");
	}
public:
	char mMachineIndex;
	char mDeviceID[4];
	char mDeviceType;
};

#endif
