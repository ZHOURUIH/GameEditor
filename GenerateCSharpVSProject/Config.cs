using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using static Utility;

public class Config
{
	public static Dictionary<string, Dictionary<string, string>> mParamMap = new Dictionary<string, Dictionary<string, string>>();
	public static bool init(string path)
	{
		byte[] bytes = null;
		try
		{
			using (FileStream configFile = new FileStream(path, FileMode.Open))
			{
				bytes = new byte[configFile.Length];
				configFile.Read(bytes, 0, (int)configFile.Length);
			}
		}
		catch
		{
			Console.WriteLine("找不到配置文件:" + path);
			return false;
		}
		string rootPathKey = "RootPath";
		string[] lines = removeAll(Encoding.UTF8.GetString(bytes), '\r').Split('\n');
		if (!lines[0].StartsWith(rootPathKey + "="))
		{
			Console.WriteLine("第一行必须是" + rootPathKey);
			return false;
		}
		string[] tempPair = lines[0].Split('=');
		if (tempPair.Length != 2)
		{
			Console.WriteLine(rootPathKey + "配置错误");
			return false;
		}
		string rootPath = tempPair[1];
		lines[0] = null;

		string curFlag = null;
		foreach (string line in lines)
		{
			if (line == null || line.Length == 0)
			{
				continue;
			}
			if (line[0] == '[' && line[line.Length - 1] == ']')
			{
				curFlag = line;
				mParamMap.Add(curFlag, new Dictionary<string, string>());
				continue;
			}
			if (curFlag == null)
			{
				continue;
			}
			string[] paramPair = line.Split('=');
			if (paramPair.Length != 2)
			{
				continue;
			}
			// 将RootPath替换为配置的值
			mParamMap[curFlag].Add(paramPair[0], paramPair[1].Replace("{" + rootPathKey + "}", rootPath));
		}
		foreach (var item in mParamMap)
		{
			if (!item.Value.ContainsKey("ProjectFilePath"))
			{
				Console.WriteLine("配置文件错误");
				return false;
			}
		}
		return true;
	}
}