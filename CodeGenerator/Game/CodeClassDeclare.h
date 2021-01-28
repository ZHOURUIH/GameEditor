#ifndef _CODE_CLASS_DECLARE_H_
#define _CODE_CLASS_DECLARE_H_

#include "CodeUtility.h"

class CodeClassDeclare : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppFrameClassDeclare(const myVector<string>& classList, string filePath);
	static void generateCppGameClassDeclare(const myVector<string>& classList, string filePath);
protected:
	const static string cppFrameDeclarePath;
	const static string cppGameDeclarePath;
};

#endif