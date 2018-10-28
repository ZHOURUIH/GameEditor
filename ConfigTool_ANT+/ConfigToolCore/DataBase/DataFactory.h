#ifndef _DATA_FACTORY_H_
#define _DATA_FACTORY_H_

#include "DataDefine.h"
#include "txMemoryTrace.h"

#include "Data.h"
#include "Utility.h"
#include "ToolCoreLog.h"

class DataFactoryBase
{
public:
	DataFactoryBase(const DATA_TYPE& type, const int& dataSize)
		:
		mType(type),
		mDataSize(dataSize)
	{}
	virtual ~DataFactoryBase()
	{
		;
	}
	virtual Data* createData() = 0;
	template<class T>
	static DataFactoryBase* createDataFactory(const DATA_TYPE& type, const int& dataSize)
	{
		T* newDataFactory = TRACE_NEW(T, newDataFactory, type, dataSize);
		return newDataFactory;
	}
	void destroyData(Data*& data)
	{
		TRACE_DELETE(data);
	}
	const DATA_TYPE& getType() { return mType; }
	const int& getDataSize() { return mDataSize; }
protected:
	DATA_TYPE mType;
	int mDataSize;
};

template<typename T>
class DataFactory : public DataFactoryBase
{
public:
	DataFactory(const DATA_TYPE& type, const int& dataSize)
		:
		DataFactoryBase(type, dataSize)
	{}
	virtual Data* createData()
	{
		T* newData = TRACE_NEW(T, newData, mType);
		return newData;
	}
};

#endif
