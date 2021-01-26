#ifndef _CODE_FRAME_SYSTEM_H_
#define _CODE_FRAME_SYSTEM_H_

#include "CodeUtility.h"

class CodeFrameSystem : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateHeaderFile(const myVector<string>& frameSystemList, 
								   const myVector<string>& factoryManagerList, 
								   const myVector<string>& classPoolList, 
								   string filePath);
	static void generateSystemDefineFile(const myVector<string>& frameSystemList,
										const myVector<string>& factoryManagerList,
										const myVector<string>& classPoolList,
										string filePath);
protected:
	const static string cppHeaderPath;
};

#endif