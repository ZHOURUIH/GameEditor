#ifndef _DATA_BASE_H_
#define _DATA_BASE_H_

#include "txCommandReceiver.h"

#include "DataDefine.h"
#include "DataFactory.h"
#include "ToolCoreBase.h"
#include "CommonDefine.h"

class Data;
class DataFactoryBase;
class DataBase : public txCommandReceiver, public ToolCoreBase
{
public:
	DataBase()
	:txCommandReceiver(TOSTRING(DataBase))
	{}
	virtual ~DataBase() { destroy(); }
	// 初始化所有数据
	void init();
	void setDataPath(const std::string& path, const bool& loadAllData);
	virtual void initDataFactory();
	void destroyDataFactory();
	Data* createData(const DATA_TYPE& type);
	void loadAllDataFromFile();
	void destroyAllData();
	void destroyData(const DATA_TYPE& type);
	void clearData(const DATA_TYPE& type);
	void loadData(const std::string& filePath, const bool& forceCover);
	bool writeBinaryFile(const DATA_TYPE& type);
	virtual void update(const float& elapsedTime){}
	void destroy()
	{
		// 先销毁数据
		destroyAllData();
		// 再销毁工厂
		destroyDataFactory();
		mDataFileDefine.clear();
		mDataDefineFile.clear();
	}
	// 得到数据列表
	bool getDataList(const DATA_TYPE& type, std::vector<Data*>& dataList)
	{
		auto iter = mDataStructList.find(type);
		if (iter == mDataStructList.end())
		{
			return false;
		}
		dataList = iter->second;
		return true;
	}
	// 得到数据数量
	int getDataCount(const DATA_TYPE& type)
	{
		auto iter = mDataStructList.find(type);
		if (iter != mDataStructList.end())
		{
			return (int)iter->second.size();
		}
		return 0;
	}
	// 查询数据
	Data* queryData(const DATA_TYPE& type, const int& index)
	{
		auto iter = mDataStructList.find(type);
		if (iter != mDataStructList.end())
		{
			if (index < (int)iter->second.size())
			{
				return (iter->second)[index];
			}
		}
		return NULL;
	}
	bool addData(const DATA_TYPE& type, Data* data, const int& pos = -1);
	bool deleteData(const DATA_TYPE& type, const int& index);
	// 根据数据名得到数据定义
	const std::string& getDataNameByDataType(const DATA_TYPE& type)
	{
		auto iter = mDataDefineFile.find(type);
		if (iter != mDataDefineFile.end())
		{
			return iter->second;
		}
		return EMPTY_STRING;
	}
	// 根据数据定义得到数据名
	DATA_TYPE getDataTypeByDataName(const std::string& name)
	{
		auto iter = mDataFileDefine.find(name);
		if (iter != mDataFileDefine.end())
		{
			return iter->second;
		}
		return DT_MIN;
	}
protected:
	template<typename T>
	void addDataFactory(const DATA_TYPE& type, const std::string& dataName)
	{
		T data(type);
		mDataFileDefine.insert(dataName, type);
		mDataDefineFile.insert(type, dataName);
		DataFactoryBase* factory = DataFactoryBase::createDataFactory<DataFactory<T> >(type, data.getSize());
		addDataFactoryToList(factory);
	}
	void addDataFactoryToList(DataFactoryBase* factory);
	DataFactoryBase* getDataFactory(const DATA_TYPE& type)
	{
		auto itrFind = mDataFactoryList.find(type);
		if (itrFind != mDataFactoryList.end())
		{
			return itrFind->second;
		}
		return NULL;
	}
protected:
	txMap<DATA_TYPE, std::vector<Data*> > mDataStructList;
	static txMap<DATA_TYPE, DataFactoryBase*> mDataFactoryList;
	static txMap<std::string, DATA_TYPE> mDataFileDefine;
	static txMap<DATA_TYPE, std::string> mDataDefineFile;
	std::string mDataPath;
};

#endif
