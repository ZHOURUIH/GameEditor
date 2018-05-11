#include "ConfigToolCore.h"
#include "SocketNetManager.h"
#include "txMemeryTrace.h"
#include "CTEventSystem.h"
#include "txCommandSystem.h"
#include "SpeedDataManager.h"
#include "SocketPacketFriction.h"
#include "SocketPacketColour.h"
#include "txSerializer.h"
#include "ToolCoreBase.h"

template<> ConfigToolCore* txSingleton<ConfigToolCore>::ms_Singleton = NULL;

ConfigToolCore::ConfigToolCore()
:
mSocketNetManager(NULL),
mEventSystem(NULL),
mCommandSystem(NULL),
mSpeedDataManager(NULL),
mHcom(NULL),
mInitFlag(false),
mReceiveData(NULL),
mReceiveDataCount(0)
{}

void ConfigToolCore::init()
{
	// 创建事件系统
	mEventSystem = TRACE_NEW(CTEventSystem, mEventSystem);
	// 事件系统创建完成后检查是否有错误信息
	checkErrorInfoBuffer();

	mCommandSystem = TRACE_NEW(txCommandSystem, mCommandSystem);
	mSpeedDataManager = TRACE_NEW(SpeedDataManager, mSpeedDataManager);
	mSocketNetManager = TRACE_NEW(SocketNetManager, mSocketNetManager);
	ToolCoreBase::notifyConstructDone();
	
	mCommandSystem->init();
	mSpeedDataManager->init();
	mSocketNetManager->init(60060, 5000);

	mReadRetThread = CreateThread(NULL, 0, readCOMResult, this, 0, NULL);
	mDeviceNameMutex = CreateMutex(NULL, false, NULL);
}

void ConfigToolCore::update(float elapsedTime)
{
	mEventSystem->update(elapsedTime);
	mCommandSystem->update(elapsedTime);
	mSocketNetManager->update(elapsedTime);
	mSpeedDataManager->update(elapsedTime);
}

DWORD ConfigToolCore::readCOMResult(LPVOID lpParameter)
{
	ConfigToolCore* configToolCore = static_cast<ConfigToolCore*>(lpParameter);
	while (true)
	{
		if (configToolCore->mHcom != NULL)
		{
			DWORD dwMask, dwTrans, dwError = 0;
			OVERLAPPED os;
			memset(&os, 0, sizeof(OVERLAPPED));
			os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			BOOL waiwRet = WaitCommEvent(configToolCore->mHcom, &dwMask, &os);
			bool read = false;
			if (waiwRet)
			{
				read = true;
			}
			else
			{
				// 如果异步操作不能立即完成的话,函数返回FALSE,并且调用GetLastError()函
				// 数分析错误原因后返回ERROR_IO_PENDING,指示异步操作正在后台进行.这种情
				// 况下,在函数返回之前系统设置OVERLAPPED结构中的事件为无信号状态,该函数
				// 等待用SetCommMask()函数设置的串口事件发生，共有9种事件可被监视：
				// EV_BREAK，EV_CTS，EV_DSR，EV_ERR，EV_RING，EV_RLSD，EV_RXCHAR，
				// EV_RXFLAG，EV_TXEMPTY；当其中一个事件发生或错误发生时，函数将
				// OVERLAPPED结构中的事件置为有信号状态，并将事件掩码填充到dwMask参数中
				if (GetLastError() == ERROR_IO_PENDING)
				{
					// 在此等待异步操作结果, 直到异步操作结束时才返回.实际上此时
					// WaitCommEvent()函数一直在等待串口监控的事件之一发生, 当事件发
					// 生时该函数将OVERLAPPED结构中的事件句柄置为有信号状态, 此时
					// GetOverlappedResult()函数发现此事件有信号后马上返回, 然后下面
					// 的程序马上分析WaitCommEvent()函数等到的事件是被监视的串口事
					// 件中的哪一个, 然后执行相应的动作并发出相应消息.
					GetOverlappedResult(configToolCore->mHcom, &os, &dwTrans, true);
					read = true;
				}
			}

			if (read && dwMask == EV_RXCHAR)
			{
				char* dataBuffer = TRACE_NEW_ARRAY(char, 256, dataBuffer);
				memset(dataBuffer, 0, 256);
				DWORD readCount = 0;
				ClearCommError(configToolCore->mHcom, NULL, NULL);
				if (ReadFile(configToolCore->mHcom, (void*)dataBuffer, 256, &readCount, NULL))
				{
					TOOL_CORE_INFO("设备配置成功! %s", dataBuffer);
					std::vector<std::string> params;
					params.push_back(txUtility::intToString((int)dataBuffer));
					params.push_back(txUtility::intToString(readCount));
					configToolCore->mEventSystem->pushEvent(CET_RECEIVE_DATA, params);

					CloseHandle(configToolCore->mHcom);
					configToolCore->mHcom = NULL;
					TRACE_DELETE_ARRAY(dataBuffer);
				}
			}
		}
	}
	return 0;
}

void ConfigToolCore::destroy()
{
	TerminateThread(mReadRetThread, 0);
	mReadRetThread = NULL;
	CloseHandle(mDeviceNameMutex);
	if (mHcom != NULL)
	{
		CloseHandle(mHcom);
		mHcom = NULL;
	}
	TRACE_DELETE(mCommandSystem);
	TRACE_DELETE(mSpeedDataManager);
	TRACE_DELETE(mSocketNetManager);
	TRACE_DELETE(mEventSystem);
}

void ConfigToolCore::checkErrorInfoBuffer()
{
	int infoCount = mErrorInfoBuffer.size();
	for (int i = 0; i < infoCount; ++i)
	{
		logError(mErrorInfoBuffer[i]);
	}
	mErrorInfoBuffer.clear();
}

void ConfigToolCore::logError(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		mErrorInfoBuffer.push_back(info);
		return;
	}
	// 如果还没有初始化完成,则先将消息放入事件缓冲中
	std::vector<std::string> params;
	params.push_back(info);
	mEventSystem->pushEvent(CET_ERROR_LOG, params, mInitFlag);
}

void ConfigToolCore::logInfo(const std::string& info)
{
	if (mEventSystem == NULL)
	{
		return;
	}
	// 如果还没有初始化完成,则先将消息放入事件缓冲中
	std::vector<std::string> params;
	params.push_back(info);
	mEventSystem->pushEvent(CET_INFO_LOG, params, mInitFlag);
}

void ConfigToolCore::checkDevice()
{
	mDeviceList.clear();
	HKEY   hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm", NULL, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		int nCount = 0;
		while (true)
		{
			char szPortName[256];
			char szComName[256];
			DWORD dwLong = 256;
			DWORD dwSize = 256;
			if (RegEnumValue(hKey, nCount, szPortName, &dwLong, NULL, NULL, (PUCHAR)szComName, &dwSize) == ERROR_NO_MORE_ITEMS)
			{
				break;
			}
			mDeviceList.push_back(std::pair<std::string, std::string>(szPortName, szComName));
			++nCount;
		}
		RegCloseKey(hKey);
	}
	mEventSystem->pushEvent(CET_DEVICE_CHANGED, std::vector<std::string>());
}

void ConfigToolCore::setFriction(const int& friction)
{
	SocketPacketFriction* packetFriction = static_cast<SocketPacketFriction*>(mSocketNetManager->createPacket(SP_FRICTION));
	packetFriction->setFrictionParam(friction * 10);
	mSocketNetManager->sendMessage(packetFriction);
}

void ConfigToolCore::setColor(const int& r, const int& g, const int& b)
{
	SocketPacketColour* packetColour = static_cast<SocketPacketColour*>(mSocketNetManager->createPacket(SP_COLOUR));
	packetColour->setColour(r, g, b);
	mSocketNetManager->sendMessage(packetColour);
}

bool ConfigToolCore::setCardParameter(const std::string& name)
{
	std::wstring str;
	generateCardData(str, name);
	bool ret = writeCOMData(mCurDeviceName, (const char*)str.c_str(), str.length() * sizeof(wchar_t));
	return ret;
}

bool ConfigToolCore::setDeviceParameter(const std::string& ip, const int& machineIndex)
{
	std::string str;
	generateDeviceData(str, ip, machineIndex);
	bool ret = writeCOMData(mCurDeviceName, str.c_str(), str.length());
	return ret;
}

void ConfigToolCore::generateDeviceData(std::string& str, const std::string& ip, const int& machineIndex)
{
	str = "net:" + ip + ":" + txUtility::intToString(machineIndex);
}

void ConfigToolCore::generateCardData(std::wstring& str, const std::string& cardName)
{	
	std::string ansiStr = "name:" + cardName;
	str = txUtility::ANSIToUnicode(ansiStr);
}

bool ConfigToolCore::writeCOMData(const std::string& comName, const char* data, const int& dataSize)
{
	if (mHcom != NULL)
	{
		return false;
	}
	if (data == NULL)
	{
		return false;
	}
	// 打开串口
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = &sd;
	HANDLE hcom = CreateFile(comName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_DEVICE, NULL);
	if (hcom == INVALID_HANDLE_VALUE)
	{
		TOOL_CORE_ERROR("error : device open failed, device name : %s", comName.c_str());
		return false;
	}

	// 设置串口状态
	SetCommMask(hcom, EV_RXCHAR | EV_TXEMPTY);//设置事件驱动的类型 
	SetupComm(hcom, 1024, 1024);//设置输入、输出缓冲区的大小
	COMMTIMEOUTS timeOver;
	timeOver.ReadIntervalTimeout = 200;
	timeOver.ReadTotalTimeoutMultiplier = 200;
	timeOver.ReadTotalTimeoutConstant = 200;
	timeOver.WriteTotalTimeoutMultiplier = 200;
	timeOver.WriteTotalTimeoutConstant = 200;
	SetCommTimeouts(hcom, &timeOver);
	PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR); //清干净输入、输出缓冲区
	DCB dcb;
	GetCommState(hcom, &dcb);
	dcb.BaudRate = 115200;
	dcb.ByteSize = 8;
	dcb.fParity = 1;
	dcb.Parity = 0;
	dcb.StopBits = 0;
	dcb.fRtsControl = 0;
	dcb.fDtrControl = 0;
	SetCommState(hcom, &dcb);

	// 向串口发送数据
	DWORD dwWrittenLen = 0;
	if (!WriteFile(hcom, data, dataSize, &dwWrittenLen, NULL))
	{
		DWORD err = GetLastError();
		CloseHandle(hcom);
		TOOL_CORE_ERROR("error : write device failed! error : %d", err);
		return false;
	}
	std::vector<std::string> params;
	params.push_back(txUtility::intToString((int)data));
	params.push_back(txUtility::intToString(dataSize));
	mEventSystem->pushEvent(CET_SEND_DATA, params);
	mHcom = hcom;
	return true;
}