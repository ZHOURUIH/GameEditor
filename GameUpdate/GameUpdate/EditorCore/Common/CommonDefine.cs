using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public delegate long TimeoutCallback();
public delegate bool DownloadingCallback(byte[] pBuffer, int nSize);
public delegate void StartCallback(string fileName, long totalSize);
public delegate void FinishCallback(string fileName, bool success);
public delegate void CommandCallback(object user_data, Command cmd);

public enum CORE_EVENT_TYPE
{
	CET_ERROR_LOG,
	CET_INFO_LOG,
	CET_START_DOWNLOAD_VERSION,     // 正在下载版本号文件
	CET_START_DOWNLOAD_LIST_FILE,   // 正在下载列表文件
	CET_DOWNLOADING_LIST_FILE,      // 列表文件下载进度,参数1表示下载百分比,范围0-1
	CET_NEW_VERSION,        // 参数1为是否有新版本可以更新
	CET_START_DOWNLOAD,     // 参数1为文件名
	CET_FINISH_DOWNLOAD,    // 参数1为文件名
	CET_START_GENERATE_LOCAL_FILE,  // 参数1文件总数量
	CET_GENERATING_LOCAL_FILE,      // 参数1文件总数量, 参数2已经查找的文件数量
	CET_FINISH_GENERATE_LOCAL_FILE,
	CET_UPDATING_FILE,      // 正在更新所有文件
	CET_UPDATING_PROGRESS,  // 更新文件的进度,参数1是进度百分比,范围0-1
	CET_UPDATE_DONE,
	CET_CANCEL_UPDATE,
	CET_NOTHING_UPDATE,
	CET_START_UPLOAD,       // 参数1是文件名
	CET_UPLOADING_FILE,     // 参数1是文件名
	CET_FINISH_UPLOAD,      // 参数1是文件名,参数2是是否成功
	CET_ALL_UPLOADED,
	CET_STOP_PROGRAM,       // 退出程序
	CET_MAX,
};
public class CommonDefine
{
	public static int BIG_FILE_SIZE = 1024 * 1024 * 16;     // 大文件大小,文件大小大于等于该大小时认为是大文件
	public static string GAME_NAME = "dndl.exe";           // 固定的游戏名称,即使版本更新也不会修改
	public static string VERSION = "Version";				// 版本文件名
	public static string FILELIST = "FileList";				// 文件列表名
	public static string TEMP_PATH = "Temp_Data/";          // 下载临时目录
	public static string TEMP_FILE_EXTENSION = ".download";	// 正在下载的临时文件后缀名
	public static string REMOTE_URL = "http://app1.taxingtianji.com/gameResource/dndl/";
	public static string UPLOAD_URL = "http://app1.taxingtianji.com/wechat/gamelogtxt/php_info.php";   // 要给服务器上传错误日志的地址
	//const std::string UPLOAD_URL = "http://localhost/php_info.php";								// 本地测试的地址
}