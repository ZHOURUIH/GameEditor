#ifndef _TX_REGISTER_TOOL_H_
#define _TX_REGISTER_TOOL_H_

#include "CommonDefine.h"
#include "EditorCoreBase.h"

class txRegisterTool : public EditorCoreBase
{
public:
	txRegisterTool(){}
	virtual ~txRegisterTool(){ destroy(); }
	void init();
	void destroy(){}
	// 生成申请码
	std::string generateRequestCode();
	// 生成注册码
	std::string generateRegisteCode(const std::string& requestCode, const std::string& encodeKey);
protected:
	void encode(std::string& str, const std::string& encodeKey);
protected:
};

#endif