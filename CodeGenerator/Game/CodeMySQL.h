#ifndef _CODE_MYSQL_H_
#define _CODE_MYSQL_H_

#include "CodeUtility.h"

class CodeMySQL : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, const string& filePath);
	static void generateCppMySQLTableFile(const MySQLInfo& mysqlInfo, const string& filePath);
	static void generateCppMySQLRegisteFile(const myVector<MySQLInfo>& mysqlList, const string& filePath);
	static void generateStringDefineMySQL(const myVector<MySQLInfo>& mysqlList, const string& stringDefineFile);
	static void generateMySQLInstanceDeclare(const myVector<MySQLInfo>& mysqlList, const string& gameBaseHeaderFile);
	static void generateMySQLInstanceDefine(const myVector<MySQLInfo>& mysqlList, const string& gameBaseSourceFile);
	static void generateMySQLInstanceClear(const myVector<MySQLInfo>& mysqlList, const string& gameBaseSourceFile);
protected:
};

#endif