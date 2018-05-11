#include "SocketPacketCard.h"

#ifdef _USE_SOCKET_UDP

#include "txUtility.h"
#include "CommandHeader.h"
#include "SpeedDataManager.h"
#include "CardData.h"

void SocketPacketCard::execute()
{
	if ((mData[0] != (unsigned char)0xFE || mData[20] != (unsigned char)0xFF))
	{
		return;
	}
	// 检查校验位 前18个字符中1的个数之和等于第19位
	int sum = 0;
	for (int i = 1; i < 19; ++i)
	{
		sum += txUtility::crc_check(mData[i]);
	}
	// 校验不正确
	if (sum != mData[19])
	{
		return;
	}
	// 0 : FE
	// 1 : 包类型
	// 2-5 : 卡号
	// 6-17 : 名字
	// 18 : 机器号
	// 19 : 校验码
	// 20 : FF
	long cardID = mData[2] << 24 | mData[3] << 16 | mData[4] << 8 | mData[5] << 0;
	wchar_t nameBuffer[13];
	memset(nameBuffer, 0, sizeof(wchar_t)* 13);
	memcpy(nameBuffer, mData + 6, 12);
	std::wstring unicodeName(nameBuffer);
	int machineIndex = (int)mData[18];

	CardData* cardData = TRACE_NEW(CardData, cardData);
	memcpy(cardData->mData, mData, cardData->mDataSize);
	cardData->mName = unicodeName;
	cardData->mCardID = cardID;
	cardData->mMachineIndex = machineIndex;
	mSpeedDataManager->setCardData(cardData);
}

#endif