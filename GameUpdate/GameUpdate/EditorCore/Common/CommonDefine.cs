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
	public static string FILE_PATH = "dndl_Data/StreamingAssets/Config/";
}
// 游戏中的公共变量定义
public enum GAME_DEFINE_FLOAT
{
	GDF_NONE,
	// 应用程序配置参数
	GDF_APPLICATION_MIN,
	GDF_FULL_SCREEN,				// 是否全屏,0为窗口模式,1为全屏,2为无边框窗口
	GDF_SCREEN_WIDTH,				// 分辨率的宽
	GDF_SCREEN_HEIGHT,				// 分辨率的高
	GDF_ADAPT_SCREEN,               // 屏幕自适应的方式,0为基于锚点的自适应,可以根据不同分辨率调整布局排列,1为简单拉伸,2为多屏拼接后复制显示
	GDF_SCREEN_COUNT,				// 显示屏数量,用于多屏横向组合为高分辨率,只能在GDF_ADAPT_SCREEN为2的情况下使用
	GDF_USE_FIXED_TIME,				// 是否将每帧的时间固定下来
	GDF_FIXED_TIME,					// 每帧的固定时间,单位秒
	GDF_VSYNC,						// 垂直同步,0为关闭垂直同步,1为开启垂直同步
	GDF_APPLICATION_MAX,

	// 框架配置参数
	GDF_FRAME_MIN,
	GDF_SOCKET_PORT,                // socket端口
	GDF_BROADCAST_PORT,             // 广播端口
	GDF_LOAD_RESOURCES,             // 游戏加载资源的路径,0代表在Resources中读取,1代表从AssetBundle中读取
	GDF_LOG_LEVEL,                  // 日志输出等级
	GDF_ENABLE_KEYBOARD,            // 是否响应键盘按键
	GDF_FRAME_MAX,

	// 游戏配置参数
	GDF_GAME_MIN,
	GDF_WIRELESS_PORT,              // 无线设备的端口
	GDF_LOAD_ASYNC,                 // 是否异步加载资源
	GDF_KICK_OUT_TIME,				// 超时未操作踢出玩家的时限
	GDF_RUNNING_COUNT_DOWN,			// 游戏开始倒计时
	GDF_CIRCLE_COUNT,               // 车轮转的圈数,全局速比
	GDF_COACH_KICK_OUT_TIME,		// 教练的超时踢出时间
	GDF_ALL_MATCH_MAX_LAST_TIME,	// 全部匹配的持续时间,当达到这个持续时间时才会出现全部匹配字样
	GDF_SEND_FRICTION_INTERVAL,		// 阻力包的发送间隔
	GDF_READ_POWER,					// 是否读取功率来计算速度,不为0则读取速度包中的功率字节,为0则读取速度字节
	GDF_VOLUME,						// 游戏整体音量
	GDF_DIFFICULTY_LEVEL,			// 当前难度等级
	GDF_DEFAULT_FRICTION,			// 默认阻力值
	GDF_FIRST_MODE,					// 进入匹配模式或实景模式时默认进入的骑行模式
	GDF_RACE_TIME,                  // 十人里程赛比赛时间,单位秒
	GDF_FAKE_HEART_RATE,			// 是否使用自动计算的心率数据
	GDF_HEART_RATE_KICK_OUT_TIME,   // 心率超时踢出的超时时间
	GDF_BINARY_CODE_X,				// 当前二维码X的值
	GDF_BINARY_CODE_Y,				// 当前二维码Y的值
	GDF_UPLOAD_LOG,                 // 是否上传日志
	GDF_ENABLE_WIRELESS_CADENCE,    // 是否启用无线设备的踏频数据,无线设备的踏频数据不能与有线设备的踏频数据同时处理
	GDF_VIRTUAL_RIDE_SPEED_SCALE,	// 虚拟骑行模式下角色速度的缩放
	GDF_NORMAL_FRICTION,            // 平地的正常阻力
	GDF_MIN_UPHILL_ANGLE,           // 最小上坡角度,符号为负表示向上
	GDF_MAX_UPHILL_ANGLE,           // 最大上坡角度,符号为负表示向上
	GDF_MIN_DOWNHILL_ANGLE,         // 最小下坡角度,符号为正表示向下
	GDF_MAX_DOWNHILL_ANGLE,         // 最大下坡角度,符号为正表示向下
	GDF_MIN_UPHILL_FRICTION,        // 最小上坡角度对应的阻力值
	GDF_MAX_UPHILL_FRICTION,        // 最大上坡角度对应的阻力值
	GDF_MIN_DOWNHILL_FRICTION,      // 最小下坡角度对应的阻力值
	GDF_MAX_DOWNHILL_FRICTION,      // 最大下坡角度对应的阻力值
	GDF_GAME_MAX,
};
public enum GAME_DEFINE_STRING
{
	GDS_NONE,
	// 应用程序配置参数
	GDS_APPLICATION_MIN,
	GDS_APPLICATION_MAX,
	// 框架配置参数
	GDS_FRAME_MIN,
	GDS_FRAME_MAX,
	// 游戏配置参数
	GDS_GAME_MIN,
	GDS_REGISTE_CODE,					// 程序注册码
	GDS_REAL_SPEED_TO_VIDEO_SPEED,		// 视频播放速度的计算公式,其中使用i来代替实际速度
	GDS_CIRCLE_TIME_MATCH,				// 匹配模式下各个速度等级下计时器旋转一圈的时间,单位秒,由于速度等级为4个等级,所以每组有4个数字,难度等级为3个等级,所以有三组
	GDS_CIRCLE_TIME_COMBAT,				// 对战模式下每个难度等级下一圈的时间,相同难度下一圈的时间相同,与速度等级无关
	GDS_SPEED_LEVEL_RANGE,				// 各个速度等级的速度区间,单位为踏频,第一速度区间为第0个数字到第1个数字之间,后面依次,由于速度等级为4个等级,则中间临界值有三个数字,难度等级为3个等级,所以有三组
	GDS_LOADING_TIP,                    // 加载界面显示的提示文字,中间以分号分隔开来
	GDS_REWARDS,						// 十人里程赛的奖励
	GDS_MODE_COUNT,                     // 设置界面中的人数
	GDS_SPEED_RATIO,                    // 学员速比,支持两种格式,a0,a1,a2:value表示a0,a1,a2机器号的转速比为value,a-b:value表示从a到b机器号的转速比为value,没有单独设置的机器号使用默认转速比
	GDS_CALCULATE_ROLL,					// 角色在转弯过程中滚动角的计算公式
	GDS_GAME_MAX,
};