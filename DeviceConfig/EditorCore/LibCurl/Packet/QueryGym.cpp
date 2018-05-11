#include "QueryGym.h"
#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"
#include "CommandHeader.h"
#include "EditorCore.h"
#include "GymInfo.h"

void QueryGym::write(std::string& stream)
{
	stream = "GymID=" + mRegisteCode;
}

void QueryGym::execute(const std::string& value)
{
	Json::Reader reader;
	Json::Value arrVal;

	// 0表示成功,1表示无法连接服务器,2表示出现错误
	int result = 0;
	bool registed = false;
	std::string gymName;
	std::string registeDate;
	// 解析JSON格式数据
	if (!reader.parse(value, arrVal) || !arrVal.isObject())
	{
		result = 1;
	}
	else
	{
		if (arrVal["result"].asString() == "success")
		{
			// 如果成功会返回健身房的名字
			if (mEditorCore->getGymInfo()->mRegisteCode != arrVal["GymID"].asString())
			{
				result = 2;
			}
			else
			{
				registed = true;
				gymName = txUtility::UTF8ToANSI(arrVal["GymName"].asString());
				registeDate = arrVal["Date"].asString();
			}
		}
	}
	CommandGymInfoNotifyQueryRet cmdQueryRet(COMMAND_PARAM);
	cmdQueryRet.mResult = result;
	cmdQueryRet.mRegisted = registed;
	cmdQueryRet.mGymName = gymName;
	cmdQueryRet.mRegisteDate = registeDate;
	mCommandSystem->pushCommand(&cmdQueryRet, mEditorCore->getGymInfo());
}