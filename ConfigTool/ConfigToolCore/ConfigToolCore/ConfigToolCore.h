#ifndef _CONFIG_TOOL_CORE_H_
#define _CONFIG_TOOL_CORE_H_

#include <vector>
#include <windows.h>

#include "txSingleton.h"

class SocketNetManager;
class CTEventSystem;
class txCommandSystem;
class SpeedDataManager;
class ConfigToolCore : public txSingleton<ConfigToolCore>
{
public:
	ConfigToolCore();
	virtual ~ConfigToolCore(){ destroy(); }
	virtual void init();
	virtual void update(float elapsedTime);
	virtual void destroy();
	void notifyInitDone() { mInitFlag = true; }
	void checkErrorInfoBuffer();
	void logError(const std::string& info);
	void logInfo(const std::string& info);
	// 刷新串口设备列表
	void checkDevice();
	// 发送阻力的网络广播消息
	void setFriction(const int& friction);
	void setColor(const int& r, const int& g, const int& b);
	bool setCardParameter(const std::string& name);
	bool setDeviceParameter(const std::string& ip, const int& machineIndex);
	void setCurDeviceName(const std::string& deviceName) { mCurDeviceName = deviceName; }
	std::vector<std::pair<std::string, std::string> >& getDeviceList() { return mDeviceList; }
	SocketNetManager* getSocketNetManager() { return mSocketNetManager; }
	CTEventSystem* getEventSystem() { return mEventSystem; }
	txCommandSystem* getCommandSystem() { return mCommandSystem; }
	SpeedDataManager* getSpeedDataManager() { return mSpeedDataManager; }
protected:
	static DWORD WINAPI readCOMResult(LPVOID lpParameter);
	// 生成写入设备的数据内容
	void generateDeviceData(std::string& str, const std::string& ip, const int& machineIndex);
	// 生成写入卡的数据内容
	void generateCardData(std::wstring& str, const std::string& cardName);
	// 向指定串口写入数据
	bool writeCOMData(const std::string& comName, const char* data, const int& dataSize);
protected:
	SocketNetManager* mSocketNetManager;
	CTEventSystem* mEventSystem;
	txCommandSystem* mCommandSystem;
	SpeedDataManager* mSpeedDataManager;
	std::vector<std::string> mErrorInfoBuffer;	// 错误信息缓冲,用于保存事件系统创建之前产生的错误信息
	bool mInitFlag;								// 初始化标记,为false表示还未初始化完成,由外部进行标记
	std::vector<std::pair<std::string, std::string> > mDeviceList;
	std::string mCurDeviceName;
	HANDLE mReadRetThread;
	HANDLE mDeviceNameMutex;
	volatile HANDLE mHcom;	// 当前操作的串口句柄
	volatile char* mReceiveData;
	volatile int mReceiveDataCount;
};

#endif