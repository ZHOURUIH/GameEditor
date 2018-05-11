#ifndef _TX_BASE_PROPERTY_H_
#define _TX_BASE_PROPERTY_H_

#include "txProperty.h"
#include "txPoint.h"
#include "txEngineDefine.h"
#include "Utility.h"

class txTexture;
class txIntProperty : public txProperty
{
public:
	txIntProperty()
	{
		mPropertyType = PT_INT;
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToInt(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::intToString(getValue(receiver));
	}
	virtual void setValue(const int& value, txPropertyReceiver* receiver) = 0;
	virtual int getValue(txPropertyReceiver* receiver) = 0;
};

class txFloatProperty : public txProperty
{
public:
	txFloatProperty()
	{
		mPropertyType = PT_FLOAT;
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToFloat(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::floatToString(getValue(receiver), 4);
	}
	virtual void setValue(const float& value, txPropertyReceiver* receiver) = 0;
	virtual float getValue(txPropertyReceiver* receiver) = 0;
};

class txDimProperty : public txProperty
{
public:
	txDimProperty()
	{
		mPropertyType = PT_DIM;
		mChildName[0] = "relative";
		mChildName[1] = "absolute";
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToDim(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::dimToString(getValue(receiver));
	}
	virtual void setValue(const txDim& value, txPropertyReceiver* receiver) = 0;
	virtual txDim getValue(txPropertyReceiver* receiver) = 0;
public:
	std::string mChildName[2];
	std::string mChildDescribe[2];
};

class txPointProperty : public txProperty
{
public:
	txPointProperty()
	{
		mPropertyType = PT_POINT;
		mChildName[0] = "relative x";
		mChildName[1] = "absolute x";
		mChildName[2] = "relative y";
		mChildName[3] = "absolute y";
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToPoint(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::pointToString(getValue(receiver));
	}
	virtual void setValue(const txPoint& value, txPropertyReceiver* receiver) = 0;
	virtual txPoint getValue(txPropertyReceiver* receiver) = 0;
public:
	std::string mChildName[4];
	std::string mChildDescribe[4];
};

class txBoolProperty : public txProperty
{
public:
	txBoolProperty()
	{
		mPropertyType = PT_BOOL;
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToBool(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::boolToString(getValue(receiver));
	}
	virtual void setValue(const bool& value, txPropertyReceiver* receiver) = 0;
	virtual bool getValue(txPropertyReceiver* receiver) = 0;
};

class txStringProperty : public txProperty
{
public:
	txStringProperty()
	{
		mPropertyType = PT_STRING;
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(value, receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return getValue(receiver);
	}
	virtual void setValue(const std::string& value, txPropertyReceiver* receiver) = 0;
	virtual std::string getValue(txPropertyReceiver* receiver) = 0;
};

class txVector2Property : public txProperty
{
public:
	txVector2Property()
	{
		mPropertyType = PT_VECTOR2;
		mChildName[0] = "x";
		mChildName[1] = "y";
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToVector2(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::vector2ToString(getValue(receiver));
	}
	virtual void setValue(const VECTOR2& value, txPropertyReceiver* receiver) = 0;
	virtual VECTOR2 getValue(txPropertyReceiver* receiver) = 0;
public:
	std::string mChildName[2];
	std::string mChildDescribe[2];
};

class txVector3Property : public txProperty
{
public:
	txVector3Property()
	{
		mPropertyType = PT_VECTOR3;
		mChildName[0] = "x";
		mChildName[1] = "y";
		mChildName[2] = "z";
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToVector3(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::vector3ToString(getValue(receiver));
	}
	virtual void setValue(const VECTOR3& value, txPropertyReceiver* receiver) = 0;
	virtual VECTOR3 getValue(txPropertyReceiver* receiver) = 0;
public:
	std::string mChildName[3];
	std::string mChildDescribe[3];
};

class txVector4Property : public txProperty
{
public:
	txVector4Property()
	{
		mPropertyType = PT_VECTOR4;
		mChildName[0] = "x";
		mChildName[1] = "y";
		mChildName[2] = "z";
		mChildName[3] = "w";
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(txStringUtility::stringToVector4(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return txStringUtility::vector4ToString(getValue(receiver));
	}
	virtual void setValue(const VECTOR4& value, txPropertyReceiver* receiver) = 0;
	virtual VECTOR4 getValue(txPropertyReceiver* receiver) = 0;
public:
	std::string mChildName[4];
	std::string mChildDescribe[4];
};

typedef txTexture* PTexture;
class txTextureProperty : public txProperty
{
public:
	txTextureProperty();
	virtual void set(const std::string& value, txPropertyReceiver* receiver);
	virtual std::string get(txPropertyReceiver* receiver);
	virtual void setValue(const PTexture& value, txPropertyReceiver* receiver) = 0;
	virtual PTexture getValue(txPropertyReceiver* receiver) = 0;
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void refreshTextureList();
protected:
	static txVector<std::string> mChoices;
};

template<typename T>
class txEnumProperty : public txProperty
{
#define ADD_ENUM(t)							\
mEnumVlaueList.insert(t, TOSTRING(t));		\
mEnumNameList.insert(TOSTRING(t), t);
public:
	txEnumProperty()
	{
		mPropertyType = PT_ENUM;
	}
	virtual void set(const std::string& value, txPropertyReceiver* receiver)
	{
		setValue(getEnumValueByName(value), receiver);
	}
	virtual std::string get(txPropertyReceiver* receiver)
	{
		return getEnumNameByValue(getValue(receiver));
	}
	virtual void setValue(const T& value, txPropertyReceiver* receiver) = 0;
	virtual T getValue(txPropertyReceiver* receiver) = 0;
	txMap<T, std::string>& getEnumList() { return mEnumVlaueList; }
	T getEnumValueByName(const std::string& name)
	{
		auto itrFind = mEnumNameList.find(name);
		if (mEnumNameList.end() != itrFind)
		{
			return itrFind->second;
		}
		return (T)0;
	}
	const std::string& getEnumNameByValue(T value)
	{
		auto itrFind = mEnumVlaueList.find(value);
		if (mEnumVlaueList.end() != itrFind)
		{
			return itrFind->second;
		}
		return EMPTY_STRING;
	}
protected:
	txMap<T, std::string> mEnumVlaueList;
	txMap<std::string, T> mEnumNameList;
};

template<typename PROPERTY, typename TYPE, typename CLASS>
class txRealProperty : public PROPERTY
{
public:
	virtual void setValue(const TYPE& value, txPropertyReceiver* receiver){ setRealValue(value, static_cast<CLASS*>(receiver)); }
	virtual TYPE getValue(txPropertyReceiver* receiver){ return getRealValue(static_cast<CLASS*>(receiver)); }
	virtual void setRealValue(const TYPE& value, CLASS* realClass) = 0;
	virtual TYPE getRealValue(CLASS* realClass) = 0;
};

#endif