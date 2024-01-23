using System;
using static FileUtility;
using static StringUtility;

public class Config
{
	public static string mClientProjectPath;
	public static string mExcelPath;
	public static string mExcelDataHotFixPath;
	public static string mExcelTableHotFixPath;
	public static string mExcelBytesPath;
	public static string mHotFixCommonPath;
	public static bool parse(string configPath)
	{
		openTxtFileLines(configPath, out string[] lines);
		if(lines == null)
		{
			Console.WriteLine("找不到配置文件ExcelConverterConfig.txt,或者配置文件内容为空");
			return false;
		}
		for(int i = 0; i < lines.Length; ++i)
		{
			if (lines[i].StartsWith("//"))
			{
				continue;
			}
			string[] param = split(lines[i], true, "=");
			if (param.Length != 2)
			{
				Console.WriteLine("配置文件读取错误:" + lines[i]);
				return false;
			}
			if (param[0] == "ProjectPath")
			{
				mClientProjectPath = param[1];
				validPath(ref mClientProjectPath);
				rightToLeft(ref mClientProjectPath);
			}
			if (param[0] == "ExcelPath")
			{
				mExcelPath = param[1];
				validPath(ref mExcelPath);
				rightToLeft(ref mExcelPath);
			}
		}
		if (isEmpty(mClientProjectPath))
		{
			Console.WriteLine("路径解析错误,找不到ProjectPath的配置");
			return false;
		}
		if (isEmpty(mExcelPath))
		{
			Console.WriteLine("路径解析错误,找不到ExcelPath的配置");
			return false;
		}
		string hotfixGamePath = mClientProjectPath + "Assets/Scripts/HotFix/";
		mExcelDataHotFixPath = hotfixGamePath + "DataBase/Excel/Data/";
		mExcelTableHotFixPath = hotfixGamePath + "DataBase/Excel/Table/";
		mExcelBytesPath = mClientProjectPath + "Assets/GameResources/Excel/";
		mHotFixCommonPath = hotfixGamePath + "Common/";
		return true;
	}
}