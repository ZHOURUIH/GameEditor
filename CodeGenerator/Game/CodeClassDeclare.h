#ifndef _CODE_CLASS_DECLARE_H_
#define _CODE_CLASS_DECLARE_H_

#include "CodeUtility.h"

class CodeClassDeclare : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppFrameClassAndHeader(const string& path, const string& targetFilePath);
	static void generateCppGameClassAndHeader(const string& path, const string& targetFilePath);
	static void generateCppBattleCoreClassAndHeader(const string& path, const string& targetFilePath);
protected:
};

#endif