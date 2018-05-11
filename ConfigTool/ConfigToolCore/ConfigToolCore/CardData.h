#ifndef _CARD_DATA_H_
#define _CARD_DATA_H_

#include <map>

#include "txCommandReceiver.h"
#include "CommonDefine.h"

class CardData
{
public:
	static const int mDataSize = 21;
	unsigned char mData[mDataSize];
	std::wstring mName;
	long mCardID;
	int mMachineIndex;

	CardData()
	{
		memset(mData, 0, mDataSize);
		mName = L"";
		mCardID = 0;
		mMachineIndex = 0;
	}
	CardData(unsigned char* data, std::wstring name, long cardID, int machineIndex)
	{
		memcpy(mData, data, mDataSize);
		mName = name;
		mCardID = cardID;
		mMachineIndex = machineIndex;
	}
	CardData& operator = (CardData& that)
	{
		memcpy(mData, that.mData, mDataSize);
		mName = that.mName;
		mCardID = that.mCardID;
		mMachineIndex = that.mMachineIndex;
		return *this;
	}
};

#endif