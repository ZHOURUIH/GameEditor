#include "RegisteGym.h"
#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"
#include "EditorCore.h"
#include "CommandHeader.h"
#include "GymInfo.h"

void RegisteGym::write(std::string& stream)
{
	stream = "gamedata=";
	txUtility::jsonStartStruct(stream);
	txUtility::jsonAddPair(stream, "GymName", mGymName);
	txUtility::jsonAddPair(stream, "GymID", mRegisteCode);
	txUtility::jsonEndStruct(stream);
	txUtility::removeLastComma(stream);
}

void RegisteGym::execute(const std::string& value)
{
	Json::Reader reader;
	Json::Value arrVal;

	// 0表示成功,1表示无法连接服务器,2表示出现错误
	std::string errorInfo;
	bool registed = false;
	std::string gymName;
	std::string registeDate;
	// 解析JSON格式数据
	if (!reader.parse(value, arrVal) || !arrVal.isObject())
	{
		errorInfo = "parse error";
	}
	else
	{
		std::string retString = arrVal["result"].asString();
		if (retString == "success")
		{
			// 如果成功会返回健身房的名字
			if (mEditorCore->getGymInfo()->mRegisteCode != arrVal["GymID"].asString())
			{
				errorInfo = "gymid not match";
			}
			gymName = txUtility::UTF8ToANSI(arrVal["GymName"].asString());
			registeDate = arrVal["Date"].asString();
			registed = true;
		}
		else
		{
			errorInfo = retString;
		}
	}
	CommandGymInfoNotifyRegisteRet cmd(COMMAND_PARAM);
	cmd.mErrorInfo = errorInfo;
	cmd.mRegisteRet = registed;
	cmd.mGymName = gymName;
	cmd.mRegisteDate = registeDate;
	cmd.mRegisteCode = mEditorCore->getGymInfo()->mRegisteCode;
	mCommandSystem->pushCommand(&cmd, mEditorCore->getGymInfo());
}