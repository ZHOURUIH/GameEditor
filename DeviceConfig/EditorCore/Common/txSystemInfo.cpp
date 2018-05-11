#include "txSystemInfo.h"
#include "txUtility.h"

void txSystemInfo::init()
{
	mDone = false;
	HANDLE handle = CreateThread(NULL, 0, deviceInfoThread, this, 0, NULL);
	CloseHandle(handle);
}

DWORD WINAPI txSystemInfo::deviceInfoThread(LPVOID lpParameter)
{
	txSystemInfo* systemInfo = static_cast<txSystemInfo*>(lpParameter);
	DEVICE_PROPERTY propOriMAC;
	if (systemInfo->WMI_DeviceQuery(DT_ORIGINAL_MAC, &propOriMAC, 1) > 0)
	{
		systemInfo->mOriMAC = propOriMAC.szProperty;
	}
	DEVICE_PROPERTY propHDD;
	if (systemInfo->WMI_DeviceQuery(DT_HDD, &propHDD, 1) > 0)
	{
		systemInfo->mHDD = propHDD.szProperty;
	}
	DEVICE_PROPERTY propMainbord;
	if (systemInfo->WMI_DeviceQuery(DT_MAINBORD, &propMainbord, 1) > 0)
	{
		systemInfo->mMainbord = propMainbord.szProperty;
	}
	DEVICE_PROPERTY propCPU;
	if (systemInfo->WMI_DeviceQuery(DT_CPU, &propCPU, 1) > 0)
	{
		systemInfo->mCPU = propCPU.szProperty;
	}
	DEVICE_PROPERTY propBIOS;
	if (systemInfo->WMI_DeviceQuery(DT_BIOS, &propBIOS, 1) > 0)
	{
		systemInfo->mBIOS = propBIOS.szProperty;
	}
	DEVICE_PROPERTY propMainbordType;
	if (systemInfo->WMI_DeviceQuery(DT_MAINBORD_TYPE, &propMainbordType, 1) > 0)
	{
		systemInfo->mMainbordType = propMainbordType.szProperty;
	}
	DEVICE_PROPERTY propCurMAC;
	if (systemInfo->WMI_DeviceQuery(DT_CUR_MAC, &propCurMAC, 1) > 0)
	{
		systemInfo->mCurMAC = propCurMAC.szProperty;
	}
	systemInfo->mDone = true;
	return 0;
}

BOOL txSystemInfo::WMI_DoWithPNPDeviceID(const char* PNPDeviceID, char* MacAddress, int uSize)
{
	char   DevicePath[MAX_PATH];
	BOOL    isOK = FALSE;

	// 生成设备路径名  
	StringCchCopyA(DevicePath, MAX_PATH, "\\\\.\\");
	StringCchCatA(DevicePath, MAX_PATH, PNPDeviceID);
	StringCchCatA(DevicePath, MAX_PATH, "#{ad498944-762f-11d0-8dcb-00c04fc3358c}");

	// 将“PNPDeviceID”中的“/”替换成“#”，以获得真正的设备路径名  
	std::replace(DevicePath + 4, DevicePath + 4 + strlen(PNPDeviceID), '\\', '#');

	// 获取设备句柄  
	HANDLE  hDeviceFile = CreateFileA(DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (hDeviceFile != INVALID_HANDLE_VALUE)
	{
		ULONG   dwID;
		BYTE    ucData[8];
		DWORD   dwByteRet;

		// 获取网卡原生MAC地址  
		dwID = OID_802_3_PERMANENT_ADDRESS;
		isOK = DeviceIoControl(hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL);
		if (isOK)
		{
			// 将字节数组转换成16进制字符串  
			for (int i = 0; i < (int)dwByteRet; ++i)
			{
				StringCchPrintfA(MacAddress + (i << 1), uSize - (i << 1), "%02X", ucData[i]);
			}
			MacAddress[dwByteRet << 1] = '\0';  // 写入字符串结束标记  
		}
		CloseHandle(hDeviceFile);
	}

	return isOK;
}

BOOL txSystemInfo::WMI_DoWithHarddiskSerialNumber(char* serialNumber, int uSize)
{
	int iLen = (int)strlen(serialNumber);
	if (iLen == 40)    // InterfaceType = "IDE"  
	{
		// 需要将16进制编码串转换为字符串  
		char szBuf[32];
		bool ret = true;
		for (int i = 0; i < 20; ++i)
		{   // 将16进制字符转换为高4位  
			char ch = serialNumber[i * 2];
			unsigned char b;
			if ((ch >= '0') && (ch <= '9'))
			{
				b = ch - '0';
			}
			else if ((ch >= 'A') && (ch <= 'F'))
			{
				b = ch - 'A' + 10;
			}
			else if ((ch >= 'a') && (ch <= 'f'))
			{
				b = ch - 'a' + 10;
			}
			else
			{   // 非法字符
				ret = false;
				break;
			}

			b <<= 4;

			// 将16进制字符转换为低4位  
			ch = serialNumber[i * 2 + 1];
			if ((ch >= '0') && (ch <= '9'))
			{
				b += ch - '0';
			}
			else if ((ch >= 'A') && (ch <= 'F'))
			{
				b += ch - 'A' + 10;
			}
			else if ((ch >= 'a') && (ch <= 'f'))
			{
				b += ch - 'a' + 10;
			}
			else
			{   // 非法字符
				ret = false;
				break;
			}
			szBuf[i] = b;
		}

		if (ret)
		{
			// 转换成功  
			szBuf[20] = '\0';
			StringCchCopyA(serialNumber, uSize, szBuf);
			iLen = strlen(serialNumber);
		}
	}

	// 每2个字符互换位置  
	for (int i = 0; i < iLen; i += 2)
	{
		if (i + 1 >= iLen)
		{
			break;
		}
		std::swap(serialNumber[i], serialNumber[i + 1]);
	}

	// 去掉空格  
	std::remove(serialNumber, serialNumber + strlen(serialNumber) + 1, ' ');
	return TRUE;
}

BOOL txSystemInfo::WMI_DoWithProperty(DEVIDE_TYPE iQueryType, char* szProperty, int uSize)
{
	BOOL isOK = TRUE;
	if (iQueryType == DT_ORIGINAL_MAC)
	{
		isOK = WMI_DoWithPNPDeviceID(szProperty, szProperty, uSize);
	}
	else if (iQueryType == DT_HDD)
	{
		isOK = WMI_DoWithHarddiskSerialNumber(szProperty, uSize);
	}
	else if (iQueryType == DT_CUR_MAC)
	{
		std::remove(szProperty, szProperty + strlen(szProperty) + 1, ':');
	}
	else
	{
		std::remove(szProperty, szProperty + strlen(szProperty) + 1, ' ');
	}
	return isOK;
}

// 基于Windows Management Instrumentation（Windows管理规范）  
int txSystemInfo::WMI_DeviceQuery(DEVIDE_TYPE iQueryType, DEVICE_PROPERTY* properties, int iSize)
{
	// 判断查询类型是否支持  
	if ((iQueryType < 0) || (iQueryType >= sizeof(szWQLQuery) / sizeof(WQL_QUERY)))
	{
		return -1;  // 查询类型不支持  
	}

	// 初始化COM  
	HRESULT hres = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		return -2;
	}

	// 设置COM的安全认证级别  
	hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hres) && hres != RPC_E_TOO_LATE)
	{
		CoUninitialize();
		return -2;
	}

	// 获得WMI连接COM接口  
	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID*>(&pLoc));
	if (FAILED(hres))
	{
		CoUninitialize();
		return -2;
	}

	// 通过连接接口连接WMI的内核对象名"ROOT\\CIMV2"  
	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(_bstr_t("ROOT\\CIMV2"), NULL, NULL, NULL, 0, NULL, NULL, &pSvc);
	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		return -2;
	}

	// 设置请求代理的安全级别  
	hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -2;
	}

	// 通过请求代理来向WMI发送请求  
	IEnumWbemClassObject *pEnumerator = NULL;
	hres = pSvc->ExecQuery(bstr_t("WQL"), bstr_t(szWQLQuery[iQueryType].szSelect), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return -3;
	}
	INT iTotal = 0;
	// 循环枚举所有的结果对象    
	while (pEnumerator)
	{
		if ((properties != NULL) && (iTotal >= iSize))
		{
			break;
		}
		IWbemClassObject *pclsObj = NULL;
		ULONG uReturn = 0;
		pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if (uReturn == 0)
		{
			break;
		}

		if (properties != NULL)
		{   // 获取属性值  
			VARIANT vtProperty;

			VariantInit(&vtProperty);
			pclsObj->Get(szWQLQuery[iQueryType].szProperty, 0, &vtProperty, NULL, NULL);
			size_t len = wcslen(vtProperty.bstrVal) + 1;
			size_t converted = 0;
			char* cStr = TRACE_NEW_ARRAY(char, len, cStr);
			wcstombs_s(&converted, cStr, len, vtProperty.bstrVal, _TRUNCATE);
			StringCchCopyA(properties[iTotal].szProperty, DEV_PROP_LEN, cStr);
			TRACE_DELETE_ARRAY(cStr);
			VariantClear(&vtProperty);

			// 对属性值做进一步的处理  
			if (WMI_DoWithProperty(iQueryType, properties[iTotal].szProperty, DEV_PROP_LEN))
			{
				++iTotal;
			}
		}
		else
		{
			++iTotal;
		}

		pclsObj->Release();
	}

	// 释放资源  
	pEnumerator->Release();
	pSvc->Release();
	pLoc->Release();
	CoUninitialize();
	return iTotal;
}