#ifndef _GAME_DEFINE_H_
#define _GAME_DEFINE_H_

#include "Utility.h"

// SQLite���������Ķ�
enum class SQLITE_OWNER : byte
{
	NONE,				// �����ڿͻ��˻��߷�����,������������
	BOTH,				// �ͻ��˺ͷ����������õ�
	CLIENT_ONLY,		// ���ͻ�����
	SERVER_ONLY,		// ����������
};

// SQLite������Server�����Ĳ�
enum class SQLITE_SERVER_OWNER : byte
{
	NONE,				// ��Чֵ
	GAME,				// Game��ʹ�õı���
	GAME_CORE,			// GameCore��ʹ�õı���
};

// MySQL���������Ĳ�
enum class MYSQL_SERVER_OWNER : byte
{
	NONE,				// ��Чֵ
	GAME,				// Game��ʹ�õı���
	GAME_CORE,			// GameCore��ʹ�õı���
};

// ��Ϣ����Server�����Ĳ�
enum class PACKET_OWNER : byte
{
	NONE,				// ��Чֵ
	GAME,				// Game��ʹ�õ���Ϣ
	GAME_CORE,			// GameCore��ʹ�õ���Ϣ
};

struct PacketMember
{
	string mTypeName;
	string mMemberName;
};

struct PacketStruct
{
	myVector<PacketMember> mMemberList;	// �ṹ���Ա
	string mStructName;					// �ṹ������
	string mComment;					// �ṹ��ע��
	PACKET_OWNER mOwner;				// ����Game�㻹��GameCore��
	bool mHotFix;						// ��Ϣ�ڿͻ����Ƿ���ȸ�
};

struct PacketInfo
{
	myVector<PacketMember> mMemberList;
	string mPacketName;					// ��Ϣ����
	string mComment;					// ��Ϣע��
	bool mShowInfo;						// �Ƿ���ʾ������Ϣ
	bool mHotFix;						// ��Ϣ�ڿͻ����Ƿ���ȸ�
	bool mUDP;							// �Ƿ�ͨ��UDP����
	PACKET_OWNER mOwner;				// ����Game�㻹��GameCore��
};

struct SQLiteMember
{
	SQLITE_OWNER mOwner;
	string mTypeName;
	string mMemberName;
	string mComment;
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
	myVector<MySQLMember> mMemberList;	// ����ID�ֶε��ֶ���Ϣ�б�
	myVector<string> mIndexList;		// �����б�
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