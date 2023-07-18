#pragma once

#include "CodeUtility.h"

class CodeClassDeclareAndHeader : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppGameClassAndHeader(const string& path, const string& targetFilePath, const string& declareFileName, const string& headerFileName);
protected:
};