#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "Utility.h"

// SQLite表格所属的端
enum class SQLITE_OWNER : byte
{
	NONE,				// 不属于客户端或者服务器,仅表格辅助作用
	BOTH,				// 客户端和服务器都会用到
	CLIENT_ONLY,		// 仅客户端用
	SERVER_ONLY,		// 仅服务器用
};

// SQLite表格在Server所属的层
enum class SQLITE_SERVER_OWNER : byte
{
	NONE,				// 无效值
	GAME,				// Game层使用的表格
	GAME_CORE,			// GameCore层使用的表格
};

// MySQL表格所属的层
enum class MYSQL_SERVER_OWNER : byte
{
	NONE,				// 无效值
	GAME,				// Game层使用的表格
	GAME_CORE,			// GameCore层使用的表格
};

// 消息包在Server所属的层
enum class PACKET_OWNER : byte
{
	NONE,				// 无效值
	GAME,				// Game层使用的消息
	GAME_CORE,			// GameCore层使用的消息
};

struct PacketMember
{
	string mTypeName;
	string mMemberName;
};

struct PacketStruct
{
	myVector<PacketMember> mMemberList;	// 结构体成员
	string mStructName;					// 结构体名字
	string mComment;					// 结构体注释
	PACKET_OWNER mOwner;				// 属于Game层还是GameCore层
	bool mHotFix;						// 消息在客户端是否可热更
};

struct PacketInfo
{
	myVector<PacketMember> mMemberList;
	string mPacketName;					// 消息包名
	string mComment;					// 消息注释
	bool mShowInfo;						// 是否显示调试信息
	bool mHotFix;						// 消息在客户端是否可热更
	bool mUDP;							// 是否通过UDP传输
	PACKET_OWNER mOwner;				// 属于Game层还是GameCore层
};

struct SQLiteMember
{
	SQLITE_OWNER mOwner;
	string mTypeName;
	string mMemberName;
	string mComment;
	string mEnumRealType;
	bool mIsList;
};

struct SQLiteInfo
{
	myVector<SQLiteMember> mMemberList;
	SQLITE_OWNER mOwner;
	string mSQLiteName;
	string mComment;
	bool mHotFix;
	bool mClientSQLite;
	SQLITE_SERVER_OWNER mServerOwner;
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
	MYSQL_SERVER_OWNER mOwner;
	void init(const string& className, const string& tableName, const string& comment, MYSQL_SERVER_OWNER owner)
	{
		mMemberList.clear();
		mIndexList.clear();
		mMySQLClassName = className;
		mMySQLTableName = tableName;
		mComment = comment;
		mOwner = owner;
	}
};

#endif