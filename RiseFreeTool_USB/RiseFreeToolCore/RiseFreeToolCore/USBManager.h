#ifndef _USB_MANAGER_H_
#define _USB_MANAGER_H_

#define LIB_USB

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "txSerializer.h"
#include "txThreadLock.h"
#ifdef LIB_USB
#include "libusb.h"
#else
extern "C" 
{
#include "hidsdi.h"
#include <setupapi.h>
}
#endif
#include "CommandReceiver.h"

class USBManager : public CommandReceiver, public EditorCoreBase
{
public:
	USBManager();
	virtual ~USBManager(){ destroy(); }
	void init();
	void destroy();
	void update(const float& elapsedTime);
	void setFriction(const int& friction);
	void notifyDeviceChanged();
	void openDevice();
	void closeDevice();
protected:
	static DWORD WINAPI writeThread(LPVOID lpParameter);
	static DWORD WINAPI readThread(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
	static DWORD WINAPI openDeviceThread(LPVOID lpParameter);
	void resizeBuffer(const int& size);
	void addDataToBuffer(unsigned char* data, const int& dataCount);
	void removeDataFromBuffer(const int& start, const int& count);
	void clearBuffer();
	// 检查指定设备是否存在
	bool checkDevice();
protected:
	HANDLE mWriteThread;
	HANDLE mReadThread;
	HANDLE mParseDataThread;
	HANDLE mOpenDeviceThread;
	txThreadLock mReadBufferLock;
	txThreadLock mWriteBufferLock;
	unsigned char* mReadBuffer;			// 接收到的数据的缓冲区
	std::atomic<int> mDataLength;		// 缓冲区中剩余接收到的数据长度
	std::atomic<int> mBufferSize;		// 缓冲区总大小
	txVector<WriteData*> mWriteDataBuffer;	// 写入数据的缓冲列表,用于添加数据
	txVector<WriteData*> mWriteDataList;	// 需要写入的数据列表,用于写入数据
#ifdef LIB_USB
	libusb_context* mContext;
	libusb_device_handle* mDeviceHandle;
	int mEndPointIN;
	int mEndPointOUT;
#else
	HANDLE mDeviceHandle;
#endif
	unsigned short VID;
	unsigned short PID;
	float mSearchInterval;	// 设备未打开时,查找设备的时间间隔
	float mCurSearchTime;
};

#endif