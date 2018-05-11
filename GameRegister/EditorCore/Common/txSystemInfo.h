#ifndef _TX_SYSTEM_INFO_H_
#define _TX_SYSTEM_INFO_H_

#include "CommonDefine.h"
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment (lib, "comsuppw.lib")  
#pragma comment (lib, "wbemuuid.lib")  

enum DEVIDE_TYPE
{
	DT_ORIGINAL_MAC,	// 网卡原生MAC地址
	DT_HDD,				// 硬盘序列号
	DT_MAINBORD,		// 主板序列号
	DT_CPU,				// CPU ID
	DT_BIOS,			// BIOS序列号
	DT_MAINBORD_TYPE,	// 主板型号
	DT_CUR_MAC,			// 网卡当前MAC地址
};

const int DEV_PROP_LEN = 128;
struct DEVICE_PROPERTY
{
	char szProperty[DEV_PROP_LEN];
};

struct WQL_QUERY
{
	char*   szSelect;       // SELECT语句  
	wchar_t*  szProperty;     // 属性字段  
};

// WQL查询语句  
const WQL_QUERY szWQLQuery[] =
{
	// 网卡原生MAC地址  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", L"PNPDeviceID",
	// 硬盘序列号  
	"SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')", L"SerialNumber",
	// 主板序列号  
	"SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)", L"SerialNumber",
	// 处理器ID  
	"SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)", L"ProcessorId",
	// BIOS序列号  
	"SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)", L"SerialNumber",
	// 主板型号  
	"SELECT * FROM Win32_BaseBoard WHERE (Product IS NOT NULL)", L"Product",
	// 网卡当前MAC地址  
	"SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", L"MACAddress",
};

class txSystemInfo
{
public:
	txSystemInfo(){}
	virtual ~txSystemInfo(){ destroy(); }
	void init();
	void destroy(){}
	bool isDone()							{ return mDone; }
	const std::string& getOriMAC()			{ return mOriMAC; }
	const std::string& getHDD()				{ return mHDD; }
	const std::string& getMainbord()		{ return mMainbord; }
	const std::string& getCPU()				{ return mCPU; }
	const std::string& getBIOS()			{ return mBIOS; }
	const std::string& getMainbordType()	{ return mMainbordType; }
	const std::string& getCurMAC()			{ return mCurMAC; }
protected:
	static DWORD WINAPI deviceInfoThread(LPVOID lpParameter);
	// 通过“PNPDeviceID”获取网卡原生MAC地址
	static BOOL WMI_DoWithPNPDeviceID(const char* PNPDeviceID, char* MacAddress, int uSize);
	// 处理硬盘序列号
	static BOOL WMI_DoWithHarddiskSerialNumber(char* serialNumber, int uSize);
	// 处理硬件属性值
	static BOOL WMI_DoWithProperty(DEVIDE_TYPE iQueryType, char* szProperty, int uSize);
	//-1：不支持的设备属性值
	//-2：WMI连接失败
	//-3：不正确的WQL查询语句
	//>= 0：获取的设备个数
	int WMI_DeviceQuery(DEVIDE_TYPE iQueryType, DEVICE_PROPERTY* properties, int iSize);
protected:
	std::string mOriMAC;
	std::string mHDD;
	std::string mMainbord;
	std::string mCPU;
	std::string mBIOS;
	std::string mMainbordType;
	std::string mCurMAC;
	volatile bool mDone;
};

#endif