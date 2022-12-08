#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "Utility.h"

struct PacketMember
{
	bool mIsArray;
	myVector<string> mArrayLengthMacro;	// 多个常量只能相乘
	string mTypeName;
	string mMemberName;
};

struct PacketInfo
{
	myVector<PacketMember> mMemberList;
	string mPacketName;					// 消息包名
	string mComment;					// 消息注释
	bool mShowInfo;						// 是否显示调试信息
	bool mServerExecuteInMain;			// 服务器接收的CS消息是否在子线程执行
	bool mHotFix;						// 消息在客户端是否可热更
	bool mUDP;							// 是否通过UDP传输
};

enum class SQLITE_OWNER : byte
{
	NONE,				// 不属于客户端或者服务器,仅表格辅助作用
	BOTH,				// 客户端和服务器都会用到
	CLIENT_ONLY,		// 仅客户端用
	SERVER_ONLY,		// 仅服务器用
};

struct SQLiteMember
{
	SQLITE_OWNER mOwner;
	string mTypeName;
	string mMemberName;
	string mComment;
};

struct SQLiteInfo
{
	myVector<SQLiteMember> mMemberList;
	SQLITE_OWNER mOwner;
	string mSQLiteName;
	string mComment;
	bool mHotFix;
};

struct MySQLMember
{
	string mTypeName;
	string mMemberName;
	string mComment;
	bool mUTF8;
};

struct MySQLInfo
{
	myVector<MySQLMember> mMemberList;	// 不带ID字段的字段信息列表
	myVector<string> mIndexList;		// 索引列表
	string mMySQLClassName;
	string mMySQLTableName;
	string mComment;
};

#endif