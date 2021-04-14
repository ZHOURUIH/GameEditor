//#include "SQLiteManager.h"
//
//void SQLiteManager::init()
//{
//	SQLiteData::initFunctionList();
//}
//
//void SQLiteManager::destroy()
//{
//	FOREACH(iter, mSQliteList)
//	{
//		DELETE(iter->second);
//	}
//	END(mSQliteList);
//}
//
//void SQLiteManager::addSQLiteTable(SQLiteTableBase* table, const char* tableName)
//{
//	table->setTableName(tableName);
//	table->init(string(GD::SQLITE_PATH) + tableName + ".db");
//	mSQliteList.insert(table->getTableName(), table);
//}