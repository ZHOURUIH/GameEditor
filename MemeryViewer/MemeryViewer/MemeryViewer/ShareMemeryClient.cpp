/******************************************************************
++  File Name : FFMClass.cpp
++  Description: 共享內存類
---------------------------------------------------------------
++  Author:  Fei ZhaoDong
++  Create time: 2004/3/25 上午 10:00:00
++  Version:     1.0
++  Modifier:
++   Activities:
++  Update List: 2004/3/29 下午 02:59:45
*******************************************************************/

#include "ShareMemeryClient.h"

CFFMClient::CFFMClient()
{
	m_dwLastError = 0;
	_Init();
}
CFFMClient::~CFFMClient()
{
	Destory();
}
CFFMClient::CFFMClient(DWORD dwAccess, char *szMapName, DWORD dwSize)
{
	_Init();
	if (!Open(dwAccess, szMapName, dwSize))
	{
		Destory();
	}
}
// 初始化參數
void CFFMClient::_Init()
{
	m_hFileMap = NULL;
	m_lpFileMapBuffer = NULL;
	m_pMapName = NULL;
	m_bOpenFlag = FALSE;
}
// 關閉當前對內存文件的訪問
void CFFMClient::Destory()
{
	if (m_lpFileMapBuffer)
	{
		UnmapViewOfFile(m_lpFileMapBuffer);
		m_lpFileMapBuffer = NULL;
	}
	if (m_hFileMap)
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}
	if (m_pMapName)
	{
		free(m_pMapName);
		m_pMapName = NULL;
	}
	_Init();
}
// 打開一個內存文件
BOOL CFFMClient::Open(DWORD dwAccess, char *szMapName, DWORD dwSize)
{
	if (m_bOpenFlag)
	{
		Destory();
	}
	if (szMapName != NULL)
	{
		m_pMapName = _strdup(szMapName);
	}
	else
	{
		m_pMapName = _strdup(DEFAULT_MAPNAME);
	}
	m_hFileMap = OpenFileMappingA(dwAccess, TRUE, m_pMapName);
	if (NULL == m_hFileMap)
	{
		m_dwLastError = GetLastError();
		Destory();
		return FALSE;
	}
	m_lpFileMapBuffer = MapViewOfFile(m_hFileMap, dwAccess, 0, 0, dwSize);
	if (NULL == m_lpFileMapBuffer)
	{
		m_dwLastError = GetLastError();
		Destory();
		return FALSE;
	}
	m_bOpenFlag = TRUE;
	return TRUE;
}
// 獲取當前內存文件的指針
LPVOID CFFMClient::GetBuffer()
{
	return (m_lpFileMapBuffer) ? (m_lpFileMapBuffer) : (NULL);
}
// 讀取命令數據大小
BOOL CFFMClient::GetCmdDataSize(DWORD *pDataSize)
{
	*pDataSize = 0;
	LPDATA_HEADER pHeader = (LPDATA_HEADER)GetBuffer();
	if (NULL == pHeader)
	{
		m_dwLastError = ERROR_NO_MAPFILE;
		SetLastError(ERROR_NO_MAPFILE);
		return FALSE;
	}
	if (NETRGUSER_CFM_CODE != pHeader->dwConfirmCode)
	{
		m_dwLastError = ERROR_INVALID_CFMCODE;
		SetLastError(ERROR_INVALID_CFMCODE);
		return FALSE;
	}
	if (NETRGUSER_CMD_NONE == pHeader->nCommandCode)
	{
		m_dwLastError = ERROR_INVALID_CMDCODE;
		SetLastError(ERROR_INVALID_CMDCODE);
		return FALSE;
	}
	*pDataSize = pHeader->dwDataSize;
	return TRUE;
}
// 讀取命令數據
BOOL CFFMClient::ReadCmdData(DWORD dwCommandCode, DWORD dwBufSize, LPVOID pOutBuf)
{
	LPDATA_HEADER pHeader = (LPDATA_HEADER)GetBuffer();
	if (NULL == pHeader)
	{
		m_dwLastError = ERROR_NO_MAPFILE;
		SetLastError(ERROR_NO_MAPFILE);
		return FALSE;
	}
	if (NETRGUSER_CFM_CODE != pHeader->dwConfirmCode)
	{
		m_dwLastError = ERROR_INVALID_CFMCODE;
		SetLastError(ERROR_INVALID_CFMCODE);
		return FALSE;
	}
	if (NETRGUSER_CMD_NONE == pHeader->nCommandCode)
	{
		m_dwLastError = ERROR_INVALID_CMDCODE;
		SetLastError(ERROR_INVALID_CMDCODE);
		return FALSE;
	}
	if (pHeader->dwDataSize > dwBufSize)
	{
		m_dwLastError = ERROR_BUFFER_OVERFLOW;
		SetLastError(ERROR_BUFFER_OVERFLOW);
		return FALSE;
	}
	if (pHeader->nCommandCode != dwCommandCode)
	{
		m_dwLastError = ERROR_INVALID_CMDCODE;
		SetLastError(ERROR_INVALID_CMDCODE);
		return FALSE;
	}
	ZeroMemory(pOutBuf, dwBufSize);
	// 拷貝數據到緩衝區
	memcpy(pOutBuf, pHeader->bInfo, pHeader->dwDataSize);
	return TRUE;
}
BOOL CFFMClient::WriteCmdData(DWORD memSize, DWORD nCommandCode, DWORD dwDataSize, const LPVOID pBuf)
{
	if (!memSize)
		memSize = DEFAULT_MAPSIZE;
	m_dwSize = memSize;
	// 檢驗數據的合理性
	if (NULL == GetBuffer())
	{
		m_dwLastError = ERROR_NO_MAPFILE;
		SetLastError(ERROR_NO_MAPFILE);
		return FALSE;
	}
	if (NETRGUSER_CMD_NONE == nCommandCode)
	{
		m_dwLastError = ERROR_INVALID_CMDCODE;
		SetLastError(ERROR_INVALID_CMDCODE);
		return FALSE;
	}
	if (dwDataSize > 0 && pBuf == NULL)
	{
		m_dwLastError = ERROR_INVALID_USER_BUFFER;
		SetLastError(ERROR_INVALID_USER_BUFFER);
		return FALSE;
	}
	if (dwDataSize + sizeof(DATA_HEADER) > GetSize())
	{
		m_dwLastError = ERROR_BUFFER_OVERFLOW;
		SetLastError(ERROR_BUFFER_OVERFLOW);
		return FALSE;
	}
	// 填寫數據結構
	// 文件頭
	DATA_HEADER dataHeader;
	dataHeader.nCommandCode = nCommandCode;
	dataHeader.dwDataSize = dwDataSize;
	ZeroMemory(GetBuffer(), GetSize());
	memcpy(GetBuffer(), &dataHeader, sizeof(DATA_HEADER));
	// 數據塊
	LPDATA_HEADER pData = (LPDATA_HEADER)GetBuffer();
	memcpy(pData->bInfo, pBuf, dwDataSize);
	return TRUE;
}
// 獲取內存文件大小
DWORD CFFMClient::GetSize()
{
	return m_dwSize;
}
