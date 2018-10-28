#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"
#include "CommandHeader.h"
#include "SetupDevice.h"
#include "StringUtility.h"
#include "DeviceRegisteManager.h"
#include "ToolRegister.h"
#include "ConfigToolCore.h"
void SetupDevice::write(std::string& stream)
{
	// 读取注册表健身房ID
	std::string gymID = ToolRegister::readRegistryValue("北京踏行天际科技发展有限公司", "电能动力4.0", "REGISTE_CODE");
	// 编写json
	stream = "data=";
	auto iterRegiste = mDeviceInfoList.begin();
	auto iterRegisteEnd = mDeviceInfoList.end();
	StringUtility::jsonStartStruct(stream);
	StringUtility::jsonAddPair(stream, "gymID", gymID);
	StringUtility::jsonAddObject(stream, "data", "");
	StringUtility::jsonStartArray(stream);
	for (int i = 0; iterRegiste != iterRegisteEnd; (++iterRegiste, ++i))
	{
		auto deviceList = iterRegiste->second;
		auto iterDevice = deviceList.begin();
		auto iterDeviceEnd = deviceList.end();
		for (; iterDevice != iterDeviceEnd; ++iterDevice)
		{
			DeviceInfo* info = iterDevice->second;
			// 设置显示内容
			std::string deviceIDStr = StringUtility::charArrayToHexString((unsigned char*)info->mIDBytes, 4);
			std::string machineIndexStr = StringUtility::intToString(info->mMachineIndex);
			StringUtility::jsonStartStruct(stream);
			StringUtility::jsonAddPair(stream, "deviceIDStr", deviceIDStr);
			StringUtility::jsonAddPair(stream, "machineIndexStr", machineIndexStr);
			StringUtility::jsonEndStruct(stream);
		}
	}
	StringUtility::jsonEndArray(stream);
	StringUtility::jsonEndStruct(stream);
	StringUtility::removeLastComma(stream);
}

void SetupDevice::execute(const std::string& value)
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
			mConfigToolCore->sendEvent(CE_US_UNUPLOAD, StringUtility::intToString(US_UPLOADED));
		}
		else
		{
			mConfigToolCore->sendEvent(CE_UPLOAD_FAILED, StringUtility::intToString(US_UPLOAD_FAILED));
		}
	}
}
