#include "txRegisterTool.h"
#include "txSystemInfo.h"
#include "md5.h"

// 生成申请码
std::string txRegisterTool::generateRequestCode()
{
	while (!mSystemInfo->isDone()){}
	const std::string& hdd = mSystemInfo->getHDD();
	const std::string& mainbord = mSystemInfo->getMainbord();
	const std::string& cpu = mSystemInfo->getCPU();
	const std::string& bios = mSystemInfo->getBIOS();
	const std::string& mainbordType = mSystemInfo->getMainbordType();
	std::string systemInfo = hdd + mainbord + cpu + bios + mainbordType;
	MD5 md5(systemInfo);
	// 计算MD5值
	return md5.md5();
}

// 生成注册码
std::string txRegisterTool::generateRegisteCode(const std::string& requestCode, const std::string& encodeKey)
{
	// 再次计算MD5
	MD5 md5(requestCode);
	std::string retStr = md5.md5();
	// 然后再加密
	// 再次计算加密字符的MD5
	MD5 encodeMD5(encodeKey);
	encode(retStr, encodeMD5.md5());
	return retStr;
}

void txRegisterTool::encode(std::string& str, const std::string& encodeKey)
{
	static std::string REGISTER_CODE = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUBWXYZ";
	static int CODE_LEN = REGISTER_CODE.length();
	int strLen = str.length();
	int encodeLen = encodeKey.length();
	for (int i = 0; i < strLen; ++i)
	{
		str[i] ^= encodeKey[i % encodeLen];
		str[i] = REGISTER_CODE[std::abs((str[i] + encodeKey[i % encodeLen]) + 0xff) % CODE_LEN];
	}
}