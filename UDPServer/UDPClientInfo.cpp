#include "UDPClientInfo.h"

UDPClientInfo::~UDPClientInfo()
{
	clear();
}

void UDPClientInfo::clear()
{
	for (auto item : mSendList)
	{
		delete[] item.first;
	}
	mSendList.clear();
}