#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "Utility.h"

struct PacketMember
{
	bool mIsArray;
	myVector<string> mArrayLengthMacro;	// 多个常量只能相乘
	string mTypeName;
	string mMemberName;
	bool mVariableLength;
};

struct PacketInfo
{
	myVector<PacketMember> mMemberList;
	string mPacketName;
	bool mShowInfo;
	bool mHotFix;
};

enum class SQLITE_OWNER
{
	BOTH,
	CLIENT_ONLY,
	SERVER_ONLY,
};

struct SQLiteMember
{
	SQLITE_OWNER mOwner;
	string mTypeName;
	string mMemberName;
};

struct SQLiteInfo
{
	myVector<SQLiteMember> mMemberList;
	SQLITE_OWNER mOwner;
	string mSQLiteName;
	bool mHotFix;
};

struct MySQLMember
{
	string mTypeName;
	string mMemberName;
	bool mUTF8;
};

struct MySQLInfo
{
	myVector<MySQLMember> mMemberList;
	string mMySQLClassName;
	string mMySQLTableName;
};

#endif