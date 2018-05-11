#ifndef _COM_MANAGER_H_
#define _COM_MANAGER_H_

#include "EditorCoreBase.h"
#include "CommonDefine.h"
#include "txSerializer.h"
#include "txThreadLock.h"

class WriteData
{
public:
	char* mData;
	int mDataCount;
};

class ComManager : public EditorCoreBase
{
public:
	ComManager();
	virtual ~ComManager(){ destroy(); }
	void init();
	void destroy();
	void update(float elapsedTime);
	// 刷新串口设备列表
	void checkDevice();
	void setCurDevice(const std::string& device);
	void setFriction(const int& friction);
	const std::string& getCurDevice() { return mCurDevice; }
	txVector<std::string>& getDeviceList() { return mDeviceList; }
protected:
	static DWORD WINAPI writeThread(LPVOID lpParameter);
	static DWORD WINAPI readThread(LPVOID lpParameter);
	static DWORD WINAPI praseDataThread(LPVOID lpParameter);
	void closeCurDeviceHandle();
	void resizeBuffer(int size);
	void addDataToBuffer(unsigned char* data, int dataCount);
	void removeDataFromBuffer(int start, int count);
	void clearBuffer();
protected:
	txVector<std::string> mDeviceList;
	std::string mCurDevice;
	std::atomic<HANDLE> mCurDeviceHandle;
	HANDLE mWriteThread;
	HANDLE mReadThread;
	HANDLE mParseDataThread;
	txThreadLock mReadBufferLock;
	txThreadLock mWriteBufferLock;
	unsigned char* mReadBuffer;			// 接收到的数据的缓冲区
	std::atomic<int> mDataLength;		// 缓冲区中剩余接收到的数据长度
	std::atomic<int> mBufferSize;		// 缓冲区总大小
	txVector<WriteData*> mWriteDataBuffer;	// 写入数据的缓冲列表,用于添加数据
	txVector<WriteData*> mWriteDataList;	// 需要写入的数据列表,用于写入数据
};

#endif