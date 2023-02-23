#ifndef _CODE_FRAME_SYSTEM_H_
#define _CODE_FRAME_SYSTEM_H_

#include "CodeUtility.h"

class CodeFrameSystem : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateSystemDefineFile(const myVector<string>& frameSystemList,
										const myVector<string>& factoryManagerList,
										const myVector<string>& classPoolList,
										const string& filePath);
protected:
};

#endif