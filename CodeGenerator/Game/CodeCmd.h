#ifndef _CODE_CMD_H_
#define _CODE_CMD_H_

#include "CodeUtility.h"

class CodeCmd : public CodeUtility
{
public:
	static void generateCmdCode(string filePath, string headerPath);
protected:
	//c++
	static void generateCppCmdTotalHeaderFile(const myVector<string>& cmdList, string filePath);
	static void generateStringDefineCmd(const myVector<string>& cmdList, string filePath);
};

#endif