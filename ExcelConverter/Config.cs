using System;

public class Config : FileUtility
{
	public static string SCRIPT_PATH;
	public static string DATA_PATH;
	public static string EXCEL_PATH;
	public static bool parse(string configPath)
	{
		openTxtFileLines(configPath, out string[] lines);
		if(lines == null)
		{
			Console.WriteLine("找不到配置文件Config.txt");
			return false;
		}
		for(int i = 0; i < lines.Length; ++i)
		{
			string[] param = split(lines[i], true, "=");
			if (param.Length != 2)
			{
				Console.WriteLine("配置文件读取错误:" + lines[i]);
				return false;
			}
			if (param[0] == "ScriptPath")
			{
				SCRIPT_PATH = param[1];
				validPath(ref SCRIPT_PATH);
				rightToLeft(ref SCRIPT_PATH);
			}
			else if(param[0] == "DataPath")
			{
				DATA_PATH = param[1];
				validPath(ref DATA_PATH);
				rightToLeft(ref DATA_PATH);
			}
			else if (param[0] == "ExcelPath")
			{
				EXCEL_PATH = param[1];
				validPath(ref EXCEL_PATH);
				rightToLeft(ref EXCEL_PATH);
			}
		}
		return true;
	}
}