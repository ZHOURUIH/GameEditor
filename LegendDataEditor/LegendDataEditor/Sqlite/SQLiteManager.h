//#ifndef _SQLITE_MANAGER_H_
//#define _SQLITE_MANAGER_H_
//
//#include "FrameComponent.h"
//#include "SQLiteTable.h"
//
//// 一个sqlite文件只允许有一个表格,且名字与文件名相同
//class SQLiteManager : public FrameComponent
//{
//	BASE_CLASS(FrameComponent);
//public:
//	myMap<string, SQLiteTableBase*> mSQliteList;
//public:
//	SQLiteManager(){}
//	virtual ~SQLiteManager() { destroy(); }
//	void init() override;
//	void destroy();
//	SQLiteTableBase* getSQLite(const string& name) { return mSQliteList.get(name, NULL); }
//	void addSQLiteTable(SQLiteTableBase* table, const char* tableName);
//};
//
//#endif