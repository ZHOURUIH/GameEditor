#ifndef _CODE_CMD_H_
#define _CODE_CMD_H_

#include "CodeUtility.h"

class CodeCmd : public CodeUtility
{
public:
	static void generateCmdCode();
protected:
	//c++
	static void generateCppCmdTotalHeaderFile(const myVector<string>& cmdList, string filePath);
	static void generateStringDefineCmd(const myVector<string>& cmdList, string filePath);
protected:
	const static string cppHeaderPath;
};

#endif