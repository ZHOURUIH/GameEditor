#ifndef _DATA_KEY_H_
#define _DATA_KEY_H_

#include "CommonDefine.h"
#include "EditorCoreLog.h"

class DataKey
{
public:
	DataKey(unsigned char cmdID)
	{
		mKey = 0;
		mValueLength = 0;
		mValue = NULL;
		mCmdID = cmdID;
	}
	virtual ~DataKey()
	{
		TRACE_DELETE_ARRAY(mValue);
	}
	bool setData(unsigned char* data, const int& count)
	{
		if (count < getTotalLength())
		{
			return false;
		}
		int offset = 0;
		unsigned char lastKey = 0;
		BinaryUtility::read(lastKey, data, offset, true);
		// 如果key不正确说明这一段数据不能由该key来解析
		if (mKey != lastKey)
		{
			return false;
		}
		BinaryUtility::read(mValueLength, data, offset, true);
		if (count - offset >= mValueLength)
		{
			mValue = TRACE_NEW_ARRAY(unsigned char, mValueLength, mValue);
			BinaryUtility::readBuffer(mValue, data, mValueLength, offset);
		}
		if (mValue != NULL && mValueLength >= getRealLength())
		{
			parseValue();
		}
		return true;
	}
	int getTotalLength() { return mValueLength + sizeof(mKey) + sizeof(mValueLength); }
	virtual void copy(DataKey* key)
	{
		if (mKey != key->mKey)
		{
			EDITOR_CORE_ERROR_DELAY("copy key error! key is different!");
			return;
		}
		if (mValueLength != key->mValueLength)
		{
			mValueLength = key->mValueLength;
			TRACE_DELETE_ARRAY(mValue);
			mValue = TRACE_NEW_ARRAY(unsigned char, mValueLength, mValue);
		}
		memcpy(mValue, key->mValue, mValueLength);
	}
	virtual int getRealLength() = 0;
	virtual void parseValue() = 0;
	virtual DataKey* createKey() = 0;
public:
	unsigned char mCmdID;		// 所属命令的ID
	unsigned char mKey;
	unsigned char mValueLength;
	unsigned char* mValue;
};

// 电机状态数据
class MachineKey : public DataKey
{
public:
	MachineKey(unsigned char cmdID)
		:DataKey(cmdID)
	{
		mKey = KEY_MACHINE;
	}
	virtual void copy(DataKey* other)
	{
		MachineKey* otherKey = static_cast<MachineKey*>(other);
		DataKey::copy(otherKey);
		mPower = otherKey->mPower;
		mRPM = otherKey->mRPM;
		mAngle = otherKey->mAngle;
		mKeyStatus = otherKey->mKeyStatus;
	}
	virtual void parseValue()
	{
		int offset = 0;
		BinaryUtility::read(mPower, mValue, offset, true);
		BinaryUtility::read(mRPM, mValue, offset, true);
		// 最高位为0,角度为正
		if ((mValue[offset] & 0x80) == 0)
		{
			BinaryUtility::read(mAngle, mValue, offset, true);
		}
		// 最高位为1,角度为负
		else
		{
			mValue[offset] &= 0x7f;
			BinaryUtility::read(mAngle, mValue, offset, true);
			mAngle = -mAngle;
		}
		BinaryUtility::read(mKeyStatus, mValue, offset, true);
	}
	bool isKeyDown(int index)
	{
		return (mKeyStatus & (0x00000001 << index)) != 0;
	}
	virtual int getRealLength(){ return sizeof(mPower)+sizeof(mRPM)+sizeof(mAngle)+sizeof(mKeyStatus); }
	virtual DataKey* createKey()
	{
		MachineKey* key = TRACE_NEW(MachineKey, key, mCmdID);
		return key;
	}
public:
	unsigned short mPower;		// 功率
	unsigned short mRPM;		// 转速
	short mAngle;				// 最高位为方向标志位,最高位为1表示向左转,为0表示向右转,剩下的15位为转过的角度
	unsigned char mKeyStatus;	// 低4位表示按键状态,为1表示按下,为0表示未按下
};

// 电机阻力数据
class FrictionKey : public DataKey
{
public:
	FrictionKey(unsigned char cmdID)
		:DataKey(cmdID)
	{
		mKey = KEY_FRICTION;
	}
	virtual void copy(DataKey* other)
	{
		FrictionKey* otherKey = static_cast<FrictionKey*>(other);
		DataKey::copy(otherKey);
		mFriction = otherKey->mFriction;
	}
	virtual void parseValue()
	{
		int offset = 0;
		BinaryUtility::read(mFriction, mValue, offset, true);
	}
	virtual int getRealLength(){ return sizeof(mFriction); }
	virtual DataKey* createKey()
	{
		FrictionKey* key = TRACE_NEW(FrictionKey, key, mCmdID);
		return key;
	}
public:
	unsigned char mFriction;	// 电机阻力大小
};

#endif