#ifndef _CODE_CLASS_DECLARE_H_
#define _CODE_CLASS_DECLARE_H_

#include "CodeUtility.h"

class CodeClassDeclare : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppFrameClassDeclare(const myVector<string>& classList, const string& filePath);
	static void generateCppGameClassDeclare(const myVector<string>& classList, const string& filePath);
	static void generateCppFrameHeader(const myVector<string>& headerList, const string& filePath);
	static void generateCppGameHeader(const myVector<string>& headerList, const string& filePath);
protected:
};

#endif