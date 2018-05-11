#include "CommandHeader.h"
#include "EditorCore.h"
#include "GymInfo.h"

void CommandGymInfoNotifyRegisteRet::execute()
{
	GymInfo* gymInfo = static_cast<GymInfo*>(mReceiver);
	gymInfo->mRegisted = mRegisteRet;
	gymInfo->mRegisteCode = mRegisteCode;
	gymInfo->mGymName = mGymName;
	gymInfo->mRegisteDate = mRegisteDate;
	// 成功后将注册码写入注册表
	if (mRegisteRet)
	{
		EditorCoreUtility::writeRegistryValue(COMPANY_NAME, GAME_NAME, gymInfo->mRegisteCode, KEY_NAME);
	}
	txVector<std::string> params;
	params.push_back(mErrorInfo);
	mEditorCore->sendEvent(CET_GYM_REGISTED, params);
}