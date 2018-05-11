using System;
using System.Text;
using System.Management;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Net.NetworkInformation;

public enum DEVICE_TYPE
{
	DT_ORIGINAL_MAC,	// 网卡原生MAC地址
	DT_HDD,				// 硬盘序列号
	DT_MAINBORD,		// 主板序列号
	DT_CPU,				// CPU ID
	DT_BIOS,			// BIOS序列号
	DT_MAINBORD_TYPE,	// 主板型号
	DT_CUR_MAC,			// 网卡当前MAC地址
};

public class Query
{
	public DEVICE_TYPE mDeviceType;
	public string mSelect;
	public string mProperty;
	public Query(DEVICE_TYPE d, string s, string p)
	{
		mDeviceType = d;
		mSelect = s;
		mProperty = p;
	}
}

public class HardwareInfo
{
	List<Query> mQueryList;
	protected string mOriMAC;		// 网卡原生MAC地址
	protected string mHDD;			// 硬盘序列号
	protected string mMainbord;		// 主板序列号
	protected string mCPU;			// CPU ID
	protected string mBIOS;			// BIOS序列号
	protected string mMainbordType;	// 主板型号
	protected string mCurMAC;		// 网卡当前MAC地址
	public HardwareInfo() 
	{
		mQueryList = new List<Query>();
		mQueryList.Add(new Query((DEVICE_TYPE)0, "SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", "PNPDeviceID"));
		mQueryList.Add(new Query((DEVICE_TYPE)1, "SELECT * FROM Win32_DiskDrive WHERE (SerialNumber IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')", "SerialNumber"));
		mQueryList.Add(new Query((DEVICE_TYPE)2, "SELECT * FROM Win32_BaseBoard WHERE (SerialNumber IS NOT NULL)", "SerialNumber"));
		mQueryList.Add(new Query((DEVICE_TYPE)3, "SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)", "ProcessorId"));
		mQueryList.Add(new Query((DEVICE_TYPE)4, "SELECT * FROM Win32_BIOS WHERE (SerialNumber IS NOT NULL)", "SerialNumber"));
		mQueryList.Add(new Query((DEVICE_TYPE)5, "SELECT * FROM Win32_BaseBoard WHERE (Product IS NOT NULL)", "Product"));
		mQueryList.Add(new Query((DEVICE_TYPE)6, "SELECT * FROM Win32_NetworkAdapter WHERE (MACAddress IS NOT NULL) AND (NOT (PNPDeviceID LIKE 'ROOT%'))", "MACAddress"));
	}
	public void init()
	{
		// 网卡原生MAC地址
		mOriMAC = getHardwareInfo(mQueryList[0]);
		// 硬盘序列号
		mHDD = getHardwareInfo(mQueryList[1]);
		// 主板序列号
		mMainbord = getHardwareInfo(mQueryList[2]);
		// CPU ID
		mCPU = getHardwareInfo(mQueryList[3]);
		// BIOS序列号
		mBIOS = getHardwareInfo(mQueryList[4]);
		// 主板型号
		mMainbordType = getHardwareInfo(mQueryList[5]);
		// 网卡当前MAC地址
		mCurMAC = getHardwareInfo(mQueryList[6]);
	}
	public string getOriMAC() { return mOriMAC; }
	public string getHDD() { return mHDD; }
	public string getMainbord() { return mMainbord; }
	public string getCPU() { return mCPU; }
	public string getBIOS() { return mBIOS; }
	public string getMainboardType() { return mMainbordType; }
	public string getCurMAC() { return mCurMAC; }
	//---------------------------------------------------------------------------------------------------------------------------------
	protected string getHardwareInfo(Query q)
	{
		string value = "";
		if (q.mDeviceType != DEVICE_TYPE.DT_ORIGINAL_MAC)
		{
			ManagementObjectSearcher searcher = new ManagementObjectSearcher(q.mSelect);
			var hardInfos = searcher.Get();
			foreach (var hardInfo in hardInfos)
			{
				if (hardInfo.Properties[q.mProperty] != null)
				{
					value = hardInfo.Properties[q.mProperty].Value.ToString();
					break;
				}
			}
		}
		value = processInfo(q.mDeviceType, value);
		return value;
	}
	protected string getNetID()
	{
		NetworkInterface[] nis = NetworkInterface.GetAllNetworkInterfaces();
		foreach (NetworkInterface ni in nis)
		{
			return ni.GetPhysicalAddress().ToString();
		}
		return "";
	}
	protected string processInfo(DEVICE_TYPE type, string value)
	{
		if (type == DEVICE_TYPE.DT_ORIGINAL_MAC)
		{
			return getNetID();
		}
		else if (type == DEVICE_TYPE.DT_HDD)
		{
			if (value.Length == 40)    // InterfaceType = "IDE"  
			{
				// 需要将16进制编码串转换为字符串  
				byte[] szBuf = new byte[32];
				bool ret = true;
				for (int i = 0; i < 20; ++i)
				{
					// 将16进制字符转换为高4位  
					byte ch = (byte)value[i * 2];
					byte b;
					if ((ch >= '0') && (ch <= '9'))
					{
						b = (byte)(ch - '0');
					}
					else if ((ch >= 'A') && (ch <= 'F'))
					{
						b = (byte)(ch - 'A' + 10);
					}
					else if ((ch >= 'a') && (ch <= 'f'))
					{
						b = (byte)(ch - 'a' + 10);
					}
					else
					{   // 非法字符
						ret = false;
						break;
					}

					b <<= 4;

					// 将16进制字符转换为低4位  
					ch = (byte)value[i * 2 + 1];
					if ((ch >= '0') && (ch <= '9'))
					{
						b += (byte)(ch - '0');
					}
					else if ((ch >= 'A') && (ch <= 'F'))
					{
						b += (byte)(ch - 'A' + 10);
					}
					else if ((ch >= 'a') && (ch <= 'f'))
					{
						b += (byte)(ch - 'a' + 10);
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
					szBuf[20] = 0;
					value = Encoding.Default.GetString(szBuf);
				}
			}

			// 每2个字符互换位置
			char[] tempArray = value.ToCharArray();
			for (int i = 0; i < tempArray.Length; i += 2)
			{
				if(i + 1 >= tempArray.Length)
				{
					break;
				}
				char temp = tempArray[i];
				tempArray[i] = tempArray[i + 1];
				tempArray[i + 1] = temp;
			}
			value = new string(tempArray);

			// 去掉空格
			value = value.Replace(" ", "");
			// 移除末尾所有的0
			int len = value.Length;
			for (int i = 0; i < value.Length; ++i)
			{
				if(value[i] == 0)
				{
					len = i;
					break;
				}
			}
			value = value.Substring(0, len);
 			return value;
		}
		else if (type == DEVICE_TYPE.DT_CUR_MAC)
		{
			return value.Replace(":", "");
		}
		else
		{
			return value.Replace(" ", "");
		}
	}
}