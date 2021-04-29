#include <iostream>
#include "EncryptSQLite.h"

int main()
{
	StringUtility::initStringUtility();
	MathUtility::initMathUtility();

	EncryptSQLite encrypt;
	if (!encrypt.init())
	{
		system("pause");
		return 0;
	}
	encrypt.encrypt();
	return 0;
}