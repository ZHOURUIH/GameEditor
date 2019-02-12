#include "SQLiteTableData.h"
#include "Utility.h"
#include "SQLiteDataReader.h"

void SQLiteTableData::parse(SQLiteDataReader* reader)
{
	int count = mParameters.size();
	for (int i = 0; i < count; ++i)
	{
		if (mParameters[i].mType == typeid(string).name())
		{
			*((string*)mParameters[i].mPointer) = reader->getString(i);
		}
		else if (mParameters[i].mType == typeid(int).name())
		{
			*((int*)mParameters[i].mPointer) = reader->getInt(i);
		}
		else if (mParameters[i].mType == typeid(txVector<int>).name())
		{
			StringUtility::stringToIntArray(reader->getString(i), *((txVector<int>*)mParameters[i].mPointer));
		}
	}
}
void SQLiteTableData::insert(string& valueString) const
{
	int count = mParameters.size();
	for (int i = 0; i < count; ++i)
	{
		if (mParameters[i].mType == typeid(string).name())
		{
			StringUtility::appendValueString(valueString, *((string*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(int).name())
		{
			StringUtility::appendValueInt(valueString, *((int*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(txVector<int>).name())
		{
			StringUtility::appendValueIntArray(valueString, *((txVector<int>*)mParameters[i].mPointer));
		}
	}
}
void SQLiteTableData::update(string& updateString) const
{
	int count = mParameters.size();
	for (int i = 0; i < count; ++i)
	{
		if (mParameters[i].mType == typeid(string).name())
		{
			StringUtility::appendUpdateString(updateString, mParameters[i].mCol, *((string*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(int).name())
		{
			StringUtility::appendUpdateInt(updateString, mParameters[i].mCol, *((int*)mParameters[i].mPointer));
		}
		else if (mParameters[i].mType == typeid(txVector<int>).name())
		{
			StringUtility::appendUpdateIntArray(updateString, mParameters[i].mCol, *((txVector<int>*)mParameters[i].mPointer));
		}
	}
}