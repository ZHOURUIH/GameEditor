#ifndef _TX_SHARE_MEMORY_SERVER_H_
#define _TX_SHARE_MEMORY_SERVER_H_

#ifdef _WINDOWS
#include <aclapi.h>

#pragma warning(disable:4996)

#define DEFAULT_FILENAME NULL    // 默認的文件名
#define DEFAULT_MAPNAME  "Local//_FZD_MAP_"   // 默認的共享內存名
#define DEFAULT_MAPSIZE  (0xFFFF + 1) * 100  // 默認的共享內存大小
const DWORD NETRGUSER_CFM_CODE = 0x1211DBFF; // 校驗碼, 用於命令數據
const DWORD NETRGUSER_CMD_NONE = 0;   // 初始化指令碼, 無指令

#define ERROR_LEN    256    // 錯誤描述長度
#define ERROR_INVALID_CMDCODE 0xE00001FF  // 已經存在完全一樣的共享內存
#define ERROR_NO_MAPFILE 0xE00002FF  // 未分配共享內存文件
#define ERROR_INVALID_CFMCODE 0xE00003FF  // 校驗碼不匹配

#pragma pack(1)
// 用於存儲命令數據的內存文件格式
typedef struct _tagDATA_HEADER
{
	DWORD dwConfirmCode; // 校驗碼
	DWORD nCommandCode;  // 指令識別碼
	DWORD dwDataSize;  // 數據的大小
	BYTE  dwReserved[19]; // 保留
	BYTE  bInfo[1];   // 數據起始地址
	_tagDATA_HEADER()
	{
		dwConfirmCode = NETRGUSER_CFM_CODE;
		nCommandCode = NETRGUSER_CMD_NONE;
		dwDataSize = 0;
		ZeroMemory(dwReserved, 19);
		ZeroMemory(bInfo, 1);
	}
}DATA_HEADER, *LPDATA_HEADER;
typedef DWORD(WINAPI *PSetEntriesInAcl)(unsigned long, PEXPLICIT_ACCESS, PACL, PACL*);

#pragma pack()

//////////////////////////////////////////////////////////////////////
// 類定義，共享內存服務端
class txShareMemoryServer
{
public:
	txShareMemoryServer();
	virtual ~txShareMemoryServer();
	txShareMemoryServer(char *szFileName, char *szMapName, DWORD dwSize);
	bool Create(char *szFileName = DEFAULT_FILENAME, char *szMapName = DEFAULT_MAPNAME, DWORD dwSize = DEFAULT_MAPSIZE); // 新建共享內存
	LPVOID GetBuffer();      // 獲取內存文件指針
	DWORD GetSize(){ return m_dwSize; }      // 獲取內存文件大小
	void Destory();       // 銷毀已有的共享內存
	bool WriteCmdData(DWORD nCommandCode, DWORD dwDataSize, const LPVOID pBuf); // 寫入命令數據
protected:
	void _Init();    // 初始化參數
	bool _IsWinNTLater();  // 判斷當前操作系統
protected:
	PSetEntriesInAcl m_fnpSetEntriesInAcl;
	HANDLE m_hFile;   // 映射文件句柄
	HANDLE m_hFileMap;   // 內存文件句柄
	LPVOID m_lpFileMapBuffer; // 緩衝區指針
	char *m_pFileName;  // 映射文件名
	char *m_pMapName;  // 內存文件名
	DWORD m_dwSize;   // 緩衝區大小
	bool m_bCreateFlag;  // 是否已創建共享內存
	DWORD   m_dwLastError;  // 錯誤代碼
};
#endif
#endif
