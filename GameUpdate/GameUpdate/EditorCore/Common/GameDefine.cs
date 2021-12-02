using System;
using System.Collections.Generic;

public delegate long TimeoutCallback();
public delegate bool DownloadingCallback(byte[] pBuffer, int nSize);
public delegate void StartCallback(string fileName, long totalSize);
public delegate void FinishCallback(string fileName, bool success);
public delegate void CommandCallback(object user_data, Command cmd);

public class GameDefine
{
	public static string GAME_NAME = "MicroLegend.exe";			// 固定的游戏名称,即使版本更新也不会修改
	public static string FILE_LIST = "FileList";				// 远端的文件列表名,只是除了游戏StreamingAssets资源以外的文件列表
	public static string TEMP_PATH = "Temp_Data/";				// 下载临时目录
	public static string TEMP_FILE_EXTENSION = ".download";     // 正在下载的临时文件后缀名
	public const string REMOTE_URL = "https://microlegend.obs.cn-north-4.myhuaweicloud.com";
#if TEST
	public const string REMOTE_FOLDER = "Launcher_Test_Windows/";
#else
	public const string REMOTE_FOLDER = "Launcher_Windows/";
#endif
	public static string DOWNLOAD_URL = REMOTE_URL + "/" + REMOTE_FOLDER;
}