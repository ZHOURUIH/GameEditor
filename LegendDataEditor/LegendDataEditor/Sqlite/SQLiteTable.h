#ifndef _SQLITE_TABLE_H_
#define _SQLITE_TABLE_H_

#include "SQLiteTableBase.h"
#include "SelectCount.h"

template<class T>
class SQLiteTable : public SQLiteTableBase
{
protected:
	myMap<uint, T*> mDataList;
public:
	virtual ~SQLiteTable()
	{
		FOREACH(iter, mDataList)
		{
			DELETE(iter->second);
		}
		END(mDataList);
		mDataList.clear();
	}
	T* query(uint id, bool showError = true)
	{
		LOCK(mThreadLock);
		if (!mDataList.contains(id))
		{
			T* item = NEW(T, item);
			// 如果找不到则销毁,并且提示错误信息
			if (!queryData(id, *item))
			{
				if (showError)
				{
					//ERROR("can not find item id:" + intToString(id) + " in " + mTableName);
				}
				DELETE(item);
			}
			// 查找成功则加入缓存列表
			else
			{
				mDataList.insert(id, item);
			}
		}
		UNLOCK(mThreadLock);
		return mDataList.get(id, NULL);
	}
	const myMap<uint, T*>& queryAll()
	{
		// 如果没有查询过,或者数据的数量与已查询数量不一致才会查询全部
		LOCK(mThreadLock);
		if (mDataList.size() == 0 || doQueryCount() != (int)mDataList.size())
		{
			myVector<T*> list;
			queryAllData(list);
			FOR_VECTOR_CONST(list)
			{
				T*& value = list[i];
				// 没有查询过的数据才插入列表
				bool success = false;
				mDataList.tryInsert(value->mID, value, &success);
				// 已经查询过的则不再需要,直接释放
				if (!success)
				{
					DELETE(value);
				}
			}
		}
		UNLOCK(mThreadLock);
		return mDataList;
	}
	bool insert(T& data)
	{
		string valueString;
		data.insert(valueString);
		removeLastComma(valueString);
		return doInsert(valueString.c_str());
	}
	bool update(const T& data) 
	{
		string updateString;
		data.update(updateString);
		removeLastComma(updateString);
		array<char, 128> conditionStr{ 0 };
		appendConditionInt(conditionStr, ServerDefine::SQLITE_ID_COL, data.mID);
		return doUpdate(updateString.c_str(), conditionStr.data());
	}
protected:
	void queryAllData(myVector<T*>& dataList)
	{
		doSelect(dataList);
	}
	bool queryData(uint id, T& data)
	{
		array<char, 128> conditionString{ 0 };
		appendConditionInt(conditionString, ServerDefine::SQLITE_ID_COL, id);
		return doSelect(data, conditionString.data());
	}
	int doQueryCount()
	{
		array<char, 256> queryStr{ 0 };
		STRCAT3(queryStr, "SELECT count(ID) FROM ", mTableName, " WHERE ID > 0");
		SelectCount selectCount;
		parseReader(executeQuery(queryStr.data()), selectCount);
		return selectCount.mRowCount;
	}
	void doSelect(myVector<T*>& dataList, const char* conditionString = NULL)
	{
		array<char, 256> queryStr{ 0 };
		if (conditionString != NULL)
		{
			STRCAT4(queryStr, "SELECT * FROM ", mTableName, " WHERE ", conditionString);
		}
		else
		{
			STRCAT2(queryStr, "SELECT * FROM ", mTableName);
		}
		parseReader(executeQuery(queryStr.data()), dataList);
	}
	bool doSelect(T& data, const char* conditionString = NULL)
	{
		array<char, 256> queryStr{ 0 };
		if (conditionString != NULL)
		{
			STRCAT4(queryStr, "SELECT * FROM ", mTableName, " WHERE ", conditionString);
		}
		else
		{
			STRCAT2(queryStr, "SELECT * FROM ", mTableName);
		}
		return parseReader(executeQuery(queryStr.data()), data);
	}
};

#endif