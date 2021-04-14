#ifndef _SQLITE_DATA_H_
#define _SQLITE_DATA_H_

#include "ServerBase.h"
#include "SQLiteTable.h"

class TableParam
{
public:
	void* mPointer;
	const char* mCol;
	uint mTypeHashCode;
	const char* mTypeName;
	TableParam(void* pointer, const char* col, uint typeHashCode, const char* typeName)
	{
		mPointer = pointer;
		mCol = col;
		mTypeHashCode = typeHashCode;
		mTypeName = typeName;
	}
};

class SQLiteFunction
{
public:
	SQLiteParseFunction mParseFunction;
	SQLiteInsertFunction mInsertFunction;
	SQLiteUpdateFunction mUpdateFunction;
public:
	SQLiteFunction(SQLiteParseFunction parse, SQLiteInsertFunction insert, SQLiteUpdateFunction update)
	{
		mParseFunction = parse;
		mInsertFunction = insert;
		mUpdateFunction = update;
	}
};

class SQLiteData : public ServerBase
{
	BASE_CLASS(ServerBase);
protected:
	myVector<TableParam> mParameters;
	static myMap<uint, SQLiteFunction*> mFunctionList;
public:
	static void initFunctionList();
	void parse(SQLiteDataReader* reader);
	void insert(char* valueString, int size) const;
	void update(char* updateString, int size) const;
	template<typename T>
	void registeParam(T& param, const char* col)
	{
		mParameters.push_back(TableParam(&param, col, (uint)typeid(param).hash_code(), typeid(param).name()));
	}
protected:
	// parse
	static void parseInt(void* ptr, SQLiteDataReader* reader, int index) { *((int*)ptr) = reader->getInt(index); }
	static void parseUInt(void* ptr, SQLiteDataReader* reader, int index) { *((uint*)ptr) = reader->getInt(index); }
	static void parseBool(void* ptr, SQLiteDataReader* reader, int index) { *((bool*)ptr) = reader->getInt(index) != 0; }
	static void parseChar(void* ptr, SQLiteDataReader* reader, int index) { *((char*)ptr) = reader->getInt(index); }
	static void parseByte(void* ptr, SQLiteDataReader* reader, int index) { *((byte*)ptr) = reader->getInt(index); }
	static void parseShort(void* ptr, SQLiteDataReader* reader, int index) { *((short*)ptr) = reader->getInt(index); }
	static void parseUShort(void* ptr, SQLiteDataReader* reader, int index) { *((ushort*)ptr) = reader->getInt(index); }
	static void parseFloat(void* ptr, SQLiteDataReader* reader, int index) { *((float*)ptr) = reader->getFloat(index); }
	static void parseByteList(void* ptr, SQLiteDataReader* reader, int index) { stringToBytes(reader->getString(index), *((myVector<byte>*)ptr)); }
	static void parseUShortList(void* ptr, SQLiteDataReader* reader, int index) { stringToUShorts(reader->getString(index), *((myVector<ushort>*)ptr)); }
	static void parseIntList(void* ptr, SQLiteDataReader* reader, int index) { stringToInts(reader->getString(index), *((myVector<int>*)ptr)); }
	static void parseUIntList(void* ptr, SQLiteDataReader* reader, int index) { stringToUInts(reader->getString(index), *((myVector<uint>*)ptr)); }
	static void parseFloatList(void* ptr, SQLiteDataReader* reader, int index) { stringToFloats(reader->getString(index), *((myVector<float>*)ptr)); }
	static void parseVector2Int(void* ptr, SQLiteDataReader* reader, int index) { *((Vector2Int*)ptr) = stringToVector2Int(reader->getString(index)); }
	static void parseVector2UShort(void* ptr, SQLiteDataReader* reader, int index) { *((Vector2UShort*)ptr) = stringToVector2UShort(reader->getString(index)); }
	static void parseString(void* ptr, SQLiteDataReader* reader, int index) { reader->getString(index, *((string*)ptr)); }
	// insert
	static void insertInt(char* str, int size, void* ptr, bool notLastOne) { appendValueInt(str, size, *((int*)ptr), notLastOne); }
	static void insertUInt(char* str, int size, void* ptr, bool notLastOne) { appendValueInt(str, size, *((uint*)ptr), notLastOne); }
	static void insertBool(char* str, int size, void* ptr, bool notLastOne) { appendValueInt(str, size, *((bool*)ptr) ? 1 : 0, notLastOne); }
	static void insertChar(char* str, int size, void* ptr, bool notLastOne) { appendValueInt(str, size, *((char*)ptr), notLastOne); }
	static void insertByte(char* str, int size, void* ptr, bool notLastOne) { appendValueInt(str, size, *((byte*)ptr), notLastOne); }
	static void insertShort(char* str, int size, void* ptr, bool notLastOne) { appendValueInt(str, size, *((short*)ptr), notLastOne); }
	static void insertUShort(char* str, int size, void* ptr, bool notLastOne) { appendValueInt(str, size, *((ushort*)ptr), notLastOne); }
	static void insertFloat(char* str, int size, void* ptr, bool notLastOne) { appendValueFloat(str, size, *((float*)ptr), notLastOne); }
	static void insertByteList(char* str, int size, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<byte>*)ptr);
		appendValueBytes(str, size, list.data(), list.size(), notLastOne);
	}
	static void insertUShortList(char* str, int size, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<ushort>*)ptr);
		appendValueUShorts(str, size, list.data(), list.size(), notLastOne);
	}
	static void insertIntList(char* str, int size, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<int>*)ptr);
		appendValueInts(str, size, list.data(), list.size(), notLastOne);
	}
	static void insertUIntList(char* str, int size, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<uint>*)ptr);
		appendValueUInts(str, size, list.data(), list.size(), notLastOne);
	}
	static void insertFloatList(char* str, int size, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<float>*)ptr);
		appendValueFloats(str, size, list.data(), list.size(), notLastOne);
	}
	static void insertVector2Int(char* str, int size, void* ptr, bool notLastOne) { appendValueVector2Int(str, size, *((Vector2Int*)ptr), notLastOne); }
	static void insertVector2UShort(char* str, int size, void* ptr, bool notLastOne) { appendValueVector2UShort(str, size, *((Vector2UShort*)ptr), notLastOne); }
	static void insertString(char* str, int size, void* ptr, bool notLastOne) { appendValueString(str, size, ((string*)ptr)->c_str(), true, notLastOne); }
	// update
	static void updateInt(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateInt(str, size, col, *((int*)ptr), notLastOne); }
	static void updateUInt(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateInt(str, size, col, *((uint*)ptr), notLastOne); }
	static void updateBool(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateInt(str, size, col, *((bool*)ptr) ? 1 : 0, notLastOne); }
	static void updateChar(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateInt(str, size, col, *((char*)ptr), notLastOne); }
	static void updateByte(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateInt(str, size, col, *((byte*)ptr), notLastOne); }
	static void updateShort(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateInt(str, size, col, *((short*)ptr), notLastOne); }
	static void updateUShort(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateInt(str, size, col, *((ushort*)ptr), notLastOne); }
	static void updateFloat(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateFloat(str, size, col, *((float*)ptr), notLastOne); }
	static void updateByteList(char* str, int size, const char* col, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<byte>*)ptr);
		appendUpdateBytes(str, size, col, list.data(), list.size(), notLastOne);
	}
	static void updateUShortList(char* str, int size, const char* col, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<ushort>*)ptr);
		appendUpdateUShorts(str, size, col, list.data(), list.size(), notLastOne);
	}
	static void updateIntList(char* str, int size, const char* col, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<int>*)ptr);
		appendUpdateInts(str, size, col, list.data(), list.size(), notLastOne);
	}
	static void updateUIntList(char* str, int size, const char* col, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<uint>*)ptr);
		appendUpdateUInts(str, size, col, list.data(), list.size(), notLastOne);
	}
	static void updateFloatList(char* str, int size, const char* col, void* ptr, bool notLastOne)
	{
		auto& list = *((myVector<float>*)ptr);
		appendUpdateFloats(str, size, col, list.data(), list.size(), notLastOne);
	}
	static void updateVector2Int(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateVector2Int(str, size, col, *((Vector2Int*)ptr), notLastOne); }
	static void updateVector2UShort(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateVector2UShort(str, size, col, *((Vector2UShort*)ptr), notLastOne); }
	static void updateString(char* str, int size, const char* col, void* ptr, bool notLastOne) { appendUpdateString(str, size, col, ((string*)ptr)->c_str(), true, notLastOne); }
};

#endif