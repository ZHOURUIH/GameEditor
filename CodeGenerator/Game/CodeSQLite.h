#ifndef _CODE_SQLITE_H_
#define _CODE_SQLITE_H_

#include "CodeUtility.h"

class CodeSQLite : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFilePath);
	static void generateCppSQLiteTableFile(const SQLiteInfo& sqliteInfo, string tableFilePath);
	static void generateCppSQLiteTotalHeaderFile(const myVector<SQLiteInfo>& sqliteList, string filePath);
	static void generateCppSQLiteRegisteFile(const myVector<SQLiteInfo>& sqliteList, string filePath);
	static void generateCppSQLiteInstanceDeclare(const myVector<SQLiteInfo>& sqliteList, string filePath);
	static void generateCppSQLiteSTLPoolRegister(const myVector<SQLiteInfo>& sqliteList, string filePath);
	static void generateCppSQLiteInstanceClear(const myVector<SQLiteInfo>& sqliteList, string filePath);
	//c#
	static void generateCSharpSQLiteDataFile(const SQLiteInfo& sqliteInfo, string dataFileGamePath, string dataFileHotFixPath);
	static void generateCSharpSQLiteTableFile(const SQLiteInfo& sqliteInfo, string tableFileGamePath, string tableFileHotFixPath);
	static void generateCSharpSQLiteRegisteFileFile(const myVector<SQLiteInfo>& sqliteInfo, string fileHotFixPath, string fileGamePath);
	static void generateCSharpSQLiteDeclare(const myVector<SQLiteInfo>& sqliteInfo, string fileHotFixPath, string fileGamePath);
protected:
};

#endif