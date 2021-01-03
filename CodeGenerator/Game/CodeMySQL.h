#ifndef _CODE_MYSQL_H_
#define _CODE_MYSQL_H_

#include "CodeUtility.h"

class CodeMySQL : public CodeUtility
{
public:
	static void generateMySQLCode(string cppDataPath, string cppTablePath, string cppStringDefinePath);
protected:
	//c++
	static void generateCppMySQLDataFile(const MySQLInfo& mysqlInfo, string filePath);
	static void generateCppMySQLTableFile(const MySQLInfo& mysqlInfo, string filePath);
	static void generateCppMySQLTotalHeaderFile(const myVector<MySQLInfo>& mysqlList, string filePath);
	static void generateCppMySQLRegisteFile(const myVector<MySQLInfo>& mysqlList, string filePath);
	static void generateStringDefineMySQL(const myVector<MySQLInfo>& mysqlList, string filePath);
	static void generateMySQLClassDeclare(const myVector<MySQLInfo>& mysqlList, string filePath);
	static void generateMySQLInstanceDeclare(const myVector<MySQLInfo>& mysqlList, string filePath);
};

#endif