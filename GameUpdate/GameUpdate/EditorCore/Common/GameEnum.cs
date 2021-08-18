using System;
using System.Collections.Generic;

public enum CORE_EVENT_TYPE : byte
{
	ERROR_LOG,
	INFO_LOG,
	START_DOWNLOAD_LIST_FILE,   // 正在下载列表文件
	DOWNLOADING_LIST_FILE,      // 列表文件下载进度,参数1表示下载百分比,范围0-1
	START_DOWNLOAD,				// 参数1为文件名
	FINISH_DOWNLOAD,			// 参数1为文件名
	START_GENERATE_LOCAL_FILE,  // 参数1文件总数量
	GENERATING_LOCAL_FILE,      // 参数1文件总数量, 参数2已经查找的文件数量
	FINISH_GENERATE_LOCAL_FILE,
	UPDATING_FILE,				// 正在更新所有文件
	UPDATING_PROGRESS,			// 更新文件的进度,参数1是进度百分比,范围0-1
	UPDATE_DONE,
	CANCEL_UPDATE,
	NOTHING_UPDATE,
	MAX,
};

public enum UPGRADE_STATE : byte
{
	NONE,
	DOWNLOADING_FILE_LIST,
	PARSING_REMOTE_FILE_LIST,
	GENERATE_LOCAL_FILE,
	GENERATE_MODIFIED_FILE,
	DOWNLOADING_REMOTE_FILE,
	UPDATE_LOCAL_FILE,
	US_DONE,
};

public enum WRITE_MODE : byte
{
	AUTO_WRITE,	// 接收到下载数据时就自动写入文件
	WRITE_FINISH,// 只有在下载完毕时才写入文件
	DONT_WRITE, // 只保存在内存,不写入文件
}

public enum WRITE_RESULT : byte
{ 
	SUCCESS,		// 成功
	MD5_ERROR,		// md5校验失败
	RENAME_ERROR,	// 重命名失败
	WRITE_ERROR,	// 写入文件失败
}