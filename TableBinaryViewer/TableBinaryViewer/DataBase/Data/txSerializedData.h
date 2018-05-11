#ifndef _TX_SERIALIZED_DATA_H_
#define _TX_SERIALIZED_DATA_H_

#include "DataDefine.h"

struct DataParameter
{
	char* mDataPtr;			// 数据块内存指针
	int mDataSize;			// 数据块大小,如果是数组类型,则是整个数组的大小
	std::string mDataType;	// 如果是数组,则类型是带*的
	std::string mDescribe;	// 数据块描述
	DataParameter()
	{
		mDataPtr = NULL;
		mDataSize = 0;
	}
	DataParameter(char* data, int dataSize, const std::string& dataType, const std::string& describe)
	{
		mDataPtr = data;
		mDataSize = dataSize;
		mDataType = dataType;
		mDescribe = describe;
	}
};

class txSerializedData
{
public:
	txSerializedData();
	virtual ~txSerializedData() { mDataParameterList.clear(); }
	virtual bool read(char* pBuffer, const int& bufferSize);
	virtual bool write(char* pBuffer, const int& bufferSize);
	virtual bool writeData(const std::string& dataString, const int& paramIndex);
	virtual bool writeData(char* buffer, const int& bufferSize, const int& paramIndex);
	std::string getValueString(const int& paramIndex);
	bool readStringList(const std::vector<std::string>& dataList);
	const int& getSize() const { return mDataSize; }
	static bool readByte(char* dest, char* source, int& curSourceOffset, const int& sourceSize, const int& readSize);
	static bool writeByte(char* dest, char* source, int& destOffset, const int& destSize, const int& writeSize);
	const std::vector<DataParameter>& getParameterList() const { return mDataParameterList; }
	// 在子类构造中调用
	virtual void fillParams() = 0;
	void zeroParams();
	template<typename T>
	void pushParam(const T& param, const std::string& describe)
	{
		addParam((char*)&param, typeid(T).name(), describe, -1);
	}
	template<typename T>
	void pushArrayParam(const T* param, const int& count, const std::string& describe)
	{
		addArrayParam((char*)param, typeid(T*).name(), count, describe, -1);
	}
	void pushParam(const char* param, const std::string& type, const std::string& describe)
	{
		addParam(param, type, describe, -1);
	}
	void pushArrayParam(const char* param, const std::string& type, const int& count, const std::string& describe)
	{
		addArrayParam(param, type, count, describe, -1);
	}
	void addParam(const char* param, const std::string& type, const std::string& describe, int paramPos);
	void addArrayParam(const char* param, const std::string& type, int count, const std::string& describe, int paramPos);
	void deleteParam(int paramPos);
	void moveParam(int paramPos, int destPos);
	// 获得数组类型的大小时,是返回单个元素的大小
	static int getTypeSize(const std::string& type)
	{
		std::map<std::string, int>::iterator iterSize = mTypeSize.find(type);
		if (iterSize != mTypeSize.end())
		{
			return iterSize->second;
		}
		return 0;
	}
	static bool isArrayType(const std::string& type);
public:
	static std::string mCharType;
	static std::string mUCharType;
	static std::string mShortType;
	static std::string mIntType;
	static std::string mFloatType;
	static std::string mUCharArrayType;
	static std::string mCharArrayType;
	static std::string mShortArrayType;
	static std::string mIntArrayType;
	static std::string mFloatArrayType;
	static std::map<std::string, int> mTypeSize;	// 各个基础数据类型的大小,如果是数组,则是单个元素的大小
public:
	std::vector<DataParameter> mDataParameterList;
	int mDataSize;
};

#endif