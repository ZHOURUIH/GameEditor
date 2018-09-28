#include "ServerBase.h"

void ServerBase::notifyConstructDone()
{
	;
}

void ServerBase::notifyComponentDeconstruct()
{
	// 重新再获取一下所有组件
	notifyConstructDone();
}