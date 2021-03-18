#ifndef _CODE_OBJECT_ITEM_H_
#define _CODE_OBJECT_ITEM_H_

#include "CodeUtility.h"

class CodeObjectItem : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateStringDefineObjectItem(const myVector<string>& objectItemList, string filePath);
protected:
};

#endif