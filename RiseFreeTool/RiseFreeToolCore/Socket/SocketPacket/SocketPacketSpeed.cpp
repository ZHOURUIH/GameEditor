#include "SocketPacketSpeed.h"

#ifdef _USE_SOCKET_UDP

#include "EditorCoreUtility.h"
#include "CommandHeader.h"
#include "SpeedDataManager.h"

void SocketPacketSpeed::execute()
{
	// [10 --- 16)
	if ((mData[0] == (unsigned char)0xFE && mData[11] == (unsigned char)0xFF))
	{
		// 检查校验位 【1 - 7】中1的个数之和等于第10位
		int sum = 0;
		for (int i = 1; i < 8; ++i)
		{
			sum += BinaryUtility::crc_check(mData[i]);
		}
		// 校验正确
		if (sum == mData[10])
		{
			// [0]	  [1]		  [2] [3][4]		 [5][6]	  [7]   [8]		    [9]			[10]		[11]
			// FE	  00(包类型)  00   功率			  速度	   00   FF      机器号[0-19]	crc		     FF
			// 3-4使用功率来作为速度(不使用5-6的速度值)
			int power = mData[4] * 256 + mData[3];
			int rotateSpeed = mData[6] * 256 + mData[5];
		}
	}
}

#endif