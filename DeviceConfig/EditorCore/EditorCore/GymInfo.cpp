#include "GymInfo.h"
#include "txRegisterTool.h"
#include "CommonDefine.h"
#include "LibCurlNetManager.h"
#include "LibCurlPacketHeader.h"

GymInfo::GymInfo()
:
txCommandReceiver("GymInfo")
{
	;
}

void GymInfo::init()
{
	// 计算设备码和注册码
	txRegisterTool registerTool;
	mDeviceCode = registerTool.generateDeviceCode();
	mRegisteCode = registerTool.generateRegisteCode(mDeviceCode, REGISTE_KEY);
}

void GymInfo::queryRegiste()
{
	QueryGym* queryGym = static_cast<QueryGym*>(mLibCurlNetManager->createPacket(LCP_QUERY_GYM));
	queryGym->mRegisteCode = mRegisteCode;
	mLibCurlNetManager->sendMessage(queryGym);
}

void GymInfo::registeDevice()
{
	RegisteGym* registeGym = static_cast<RegisteGym*>(mLibCurlNetManager->createPacket(LCP_REGISTE_GYM));
	registeGym->mRegisteCode = mRegisteCode;
	registeGym->mGymName = mGymName;
	mLibCurlNetManager->sendMessage(registeGym);
}