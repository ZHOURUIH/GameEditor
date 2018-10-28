#ifndef _DEVICE_REGISTE_MANAGER_H_
#define _DEVICE_REGISTE_MANAGER_H_

#include "txCommandReceiver.h"
#include "CommonDefine.h"
#include "ToolCoreBase.h"
#include "ThreadLock.h"

class DeviceInfo
{
public:
	int mID;
	int mMachineIndex;
	char mIDBytes[4];
	char mDeviceType;
	std::string mDeviceTypeName;
};

class DeviceRegisteManager : public txCommandReceiver, public ToolCoreBase
{
public:
	DeviceRegisteManager()
	:
	txCommandReceiver(TOSTRING(DeviceRegisteManager))
	{}
	virtual ~DeviceRegisteManager(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	void destroy();
	// 0表示成功,1表示单车号已经绑定了该设备,2表示单车号已经绑定了其他设备,3表示单车号无效
	int registeDevice(char* deviceID, int machineIndex, char deviceType);
	bool unregisteDevice(char* deviceID);
	bool unregisteDeviceFromMachineIndex(int machineIndex);
	bool isDeviceRegisted(char* deviceID, int& machineIndex);
	// checkExist表示当目录中没有表格时是否要显示提示框
	void readFromDataBase(bool checkExist);
	void saveToDataBase();
	void uploadRegisteDeviceInfoList();
	int getRegistedCount() { return mMachineIndexDeviceList.size(); }
	txMap<int, txMap<char, DeviceInfo*>>& getRegisteDeviceList() { return mMachineIndexDeviceList; }
protected:
	void removeFromList(DeviceInfo* info);
	void addToList(DeviceInfo* info);
	void unregisteAllDevice();
protected:
	txMap<int, txMap<char, DeviceInfo*>> mMachineIndexDeviceList;	// 一个单车号可以绑定多个设备,但是必须是不同类型的设备,second的first是设备的类型
	txMap<int, DeviceInfo*> mDeviceMachineList;
};

#endif