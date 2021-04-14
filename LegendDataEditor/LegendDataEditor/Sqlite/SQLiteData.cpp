#include "SQLiteData.h"

myMap<uint, SQLiteFunction*> SQLiteData::mFunctionList;

void SQLiteData::initFunctionList()
{
	mFunctionList.insert(mIntType, new SQLiteFunction(parseInt, insertInt, updateInt));
	mFunctionList.insert(mUIntType, new SQLiteFunction(parseUInt, insertUInt, updateUInt));
	mFunctionList.insert(mBoolType, new SQLiteFunction(parseBool, insertBool, updateBool));
	mFunctionList.insert(mCharType, new SQLiteFunction(parseChar, insertChar, updateChar));
	mFunctionList.insert(mByteType, new SQLiteFunction(parseByte, insertByte, updateByte));
	mFunctionList.insert(mShortType, new SQLiteFunction(parseShort, insertShort, updateShort));
	mFunctionList.insert(mUShortType, new SQLiteFunction(parseUShort, insertUShort, updateUShort));
	mFunctionList.insert(mFloatType, new SQLiteFunction(parseFloat, insertFloat, updateFloat));
	mFunctionList.insert(mByteListType, new SQLiteFunction(parseByteList, insertByteList, updateByteList));
	mFunctionList.insert(mUShortListType, new SQLiteFunction(parseUShortList, insertUShortList, updateUShortList));
	mFunctionList.insert(mIntListType, new SQLiteFunction(parseIntList, insertIntList, updateIntList));
	mFunctionList.insert(mUIntListType, new SQLiteFunction(parseUIntList, insertUIntList, updateUIntList));
	mFunctionList.insert(mFloatListType, new SQLiteFunction(parseFloatList, insertFloatList, updateFloatList));
	mFunctionList.insert(mVector2IntType, new SQLiteFunction(parseVector2Int, insertVector2Int, updateVector2Int));
	mFunctionList.insert(mVector2UShortType, new SQLiteFunction(parseVector2UShort, insertVector2UShort, updateVector2UShort));
	mFunctionList.insert(mStringType, new SQLiteFunction(parseString, insertString, updateString));
}

void SQLiteData::parse(SQLiteDataReader* reader)
{
	FOR_VECTOR(mParameters)
	{
		TableParam& param = mParameters[i];
		if (param.mPointer == NULL)
		{
			continue;
		}
		SQLiteFunction* function = mFunctionList.get(param.mTypeHashCode, NULL);
		if (function == NULL)
		{
			//ERROR("SQLite参数类型错误 : " + string(param.mTypeName));
			continue;
		}
		function->mParseFunction(param.mPointer, reader, i);
	}
	END(mParameters);
}
void SQLiteData::insert(char* valueString, int size) const
{
	valueString[0] = '\0';
	uint paramCount = mParameters.size();
	FOR_I(paramCount)
	{
		const TableParam& param = mParameters[i];
		if (param.mPointer == NULL)
		{
			continue;
		}
		SQLiteFunction* function = mFunctionList.get(param.mTypeHashCode, NULL);
		if (function == NULL)
		{
			//ERROR("SQLite参数类型错误 : " + string(param.mTypeName));
			continue;
		}
		function->mInsertFunction(valueString, size, param.mPointer, i != paramCount - 1u);
	}
}
void SQLiteData::update(char* updateString, int size) const
{
	updateString[0] = '\0';
	uint paramCount = mParameters.size();
	FOR_I(paramCount)
	{
		const TableParam& param = mParameters[i];
		if (param.mPointer == NULL)
		{
			continue;
		}
		SQLiteFunction* function = mFunctionList.get(param.mTypeHashCode, NULL);
		if (function == NULL)
		{
			//ERROR("SQLite参数类型错误 : " + string(param.mTypeName));
			continue;
		}
		function->mUpdateFunction(updateString, size, param.mCol, param.mPointer, i != paramCount - 1u);
	}
}