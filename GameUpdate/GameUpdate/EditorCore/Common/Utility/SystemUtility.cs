using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

public class SystemUtility
{
	public static string getRegistryValue(string path, string keyStr)
	{
		string value = "";
		RegistryKey key = Registry.CurrentUser.CreateSubKey(path);
		string codeValue = key.GetValue(keyStr) as string;
		if (codeValue != null)
		{
			value = key.GetValue(keyStr) as string;
		}
		key.Close();
		return value;
	}
	public static List<string> getAllProcess()
	{
		List<string> list = new List<string>();
		Process[] processList = Process.GetProcesses();
		foreach(var item in processList)
		{
			list.Add(item.MainModule.ModuleName);
		}
		return list;
	}
	public static bool isProgressRunning(string name, bool isFullPath = false)
	{
		Process[] processList = Process.GetProcesses();
		foreach (var item in processList)
		{
			if(isFullPath)
			{
				if(item.MainWindowHandle != IntPtr.Zero && item.MainModule.ModuleName == name)
				{
					return true;
				}
			}
			else
			{
				if (item.ProcessName == name)
				{
					return true;
				}
			}
		}
		return false;
	}
	public static bool launchExecutable(string path, string args = "",  bool waitForExit = false)
	{
		if(!FileUtility.isFileExist(path))
		{
			return false;
		}
		Process p = null;
		if(args != "")
		{
			p = Process.Start(path, args);
		}
		else
		{
			p = Process.Start(path);
		}
		if(waitForExit)
		{
			p.WaitForExit();
		}
		return true;
	}
}