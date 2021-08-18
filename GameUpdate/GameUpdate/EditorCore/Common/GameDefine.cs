using System;
using System.Collections.Generic;

public delegate long TimeoutCallback();
public delegate bool DownloadingCallback(byte[] pBuffer, int nSize);
public delegate void StartCallback(string fileName, long totalSize);
public delegate void FinishCallback(string fileName, bool success);
public delegate void CommandCallback(object user_data, Command cmd);

public class GameDefine
{
	public static string GAME_NAME = "我的传奇.exe";						// 固定的游戏名称,即使版本更新也不会修改
	public static string NON_ASSETS_FILE_LIST = "NonAssetsFileList";		// 文件列表名
	public static string TEMP_PATH = "Temp_Data/";							// 下载临时目录
	public static string TEMP_FILE_EXTENSION = ".download";					// 正在下载的临时文件后缀名
	public static string REMOTE_URL = "http://121.36.2.236:8081/NonGameAssets_Windows/";
}