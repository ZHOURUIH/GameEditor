#include <iostream>
#include "EncryptSQLite.h"

int main()
{
	StringUtility::initStringUtility();
	MathUtility::initMathUtility();

	EncryptSQLite encrypt;
	bool result = encrypt.init();
	result = result && encrypt.encrypt();
	if (!result)
	{
		system("pause");
	}
	return 0;
}