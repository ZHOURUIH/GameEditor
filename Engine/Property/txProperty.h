#ifndef _TX_PROPERTY_H_
#define _TX_PROPERTY_H_

#include "Utility.h"
#include "txEngineDefine.h"
#include "txEngineBase.h"

class txPropertyReceiver;
class txProperty : public txEngineBase
{
public:
	txProperty()
		:
		mReadOnly(false),
		mAsync(false)
	{}
	virtual ~txProperty() {}
	virtual void set(const std::string& value, txPropertyReceiver* receiver) = 0;
	virtual std::string get(txPropertyReceiver* receiver) = 0;
	const bool& isReadOnly() { return mReadOnly; }
	const std::string& getName() { return mPropertyName; }
	const std::string& getPropertyDescirption() const { return mDescription; }
	const PROPERTY_TYPE& getType() { return mPropertyType; }
	const std::string& getReceiverType() { return mReceiverType; }
	const bool& isAsyc() { return mAsync; }
	void setAsync(const bool& async) { mAsync = async; }
	virtual txVector<std::string>* getChoices() { return NULL; }
	template<typename T>
	static std::string getPropertyName()
	{
		static T pro;
		return pro.mPropertyName;
	}
	template<typename T>
	static PROPERTY_TYPE getPropertyType()
	{
		static T pro;
		return pro.mPropertyType;
	}
	template<typename T>
	static std::string getRecieverType()
	{
		static T pro;
		return pro.mReceiverType;
	}
protected:
	std::string mPropertyName;
	PROPERTY_TYPE mPropertyType;
	std::string mDescription;
	bool mReadOnly;
	std::string mReceiverType;
	bool mAsync;		// 是否是在子线程中使用该属性
};

#endif
