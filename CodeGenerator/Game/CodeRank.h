#ifndef _CODE_RANK_H_
#define _CODE_RANK_H_

#include "CodeUtility.h"

class CodeRank : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateRegister(const myVector<string>& list, const string& headerPath);
protected:
};

#endif