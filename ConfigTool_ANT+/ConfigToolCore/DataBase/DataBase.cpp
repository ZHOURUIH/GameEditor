#include "DataBase.h"
#include "DataFactory.h"
#include "Utility.h"
#include "DataRegisteDevice.h"
#include "ConfigToolCore.h"
#include "CTEventSystem.h"

txMap<DATA_TYPE, DataFactoryBase*> DataBase::mDataFactoryList;
txMap<std::string, DATA_TYPE> DataBase::mDataFileDefine;
txMap<DATA_TYPE, std::string> DataBase::mDataDefineFile;

void DataBase::init()
{
	initDataFactory();
	setDataPath("./", true);
}

void DataBase::setDataPath(const std::string& path, const bool& loadAllData)
{
	mDataPath = path;
	if (loadAllData)
	{
		loadAllDataFromFile();
	}
}

void DataBase::initDataFactory()
{
	addDataFactory<DataRegisteDevice>(DT_REGISTE_DEVICE, TOSTRING(DataRegisteDevice));
}

void DataBase::addDataFactoryToList(DataFactoryBase* factory)
{
	mDataFactoryList.insert(factory->getType(), factory);
}

void DataBase::destroyDataFactory()
{
	auto iterDataFactoryList = mDataFactoryList.begin();
	auto iterDataFactoryListEnd = mDataFactoryList.end();
	for (; iterDataFactoryList != iterDataFactoryListEnd; ++iterDataFactoryList)
	{
		TRACE_DELETE(iterDataFactoryList->second);
	}
	mDataFactoryList.clear();
}

bool DataBase::addData(const DATA_TYPE& type, Data* data, const int& pos)
{
	if (data == NULL)
	{
		return false;
	}
	if (data->getType() != type)
	{
		return false;
	}
	auto iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (pos == -1)
		{
			iter->second.push_back(data);
		}
		else if (pos >= 0 && pos <= (int)iter->second.size())
		{
			iter->second.insert(iter->second.begin() + pos, data);
		}
	}
	else
	{
		std::vector<Data*> dataList;
		dataList.push_back(data);
		mDataStructList.insert(type, dataList);
	}
	return true;
}

bool DataBase::deleteData(const DATA_TYPE& type, const int& index)
{
	auto iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (index >= (int)iter->second.size())
		{
			return false;
		}
		DataFactoryBase* dataFactory = getDataFactory(type);
		if (dataFactory == NULL)
		{
			return false;
		}
		dataFactory->destroyData(iter->second[index]);
		iter->second.erase(iter->second.begin() + index);
		return true;
	}
	return false;
}

Data* DataBase::createData(const DATA_TYPE& type)
{
	DataFactoryBase* factory = getDataFactory(type);
	if (factory != NULL)
	{
		return factory->createData();
	}
	return NULL;
}

void DataBase::loadAllDataFromFile()
{
	if (mDataPath == EMPTY_STRING)
	{
		TOOL_CORE_INFO("请先设置表格路径!");
		return;
	}
	// 读取配置文件，获得需要加载的所有数据列表
	// 遍历每一个文件名，加载相应的文件
	txVector<std::string> fileList;
	FileUtility::findFiles(mDataPath, fileList, ".bytes");
	int listSize = fileList.size();
	for(int i = 0; i < listSize; ++i)
	{
		loadData(fileList[i], true);
	}
}

void  DataBase::destroyAllData()
{
	auto iterStructList = mDataStructList.begin();
	auto iterStructListEnd = mDataStructList.end();
	for(; iterStructList != iterStructListEnd; ++iterStructList)
	{
		DataFactoryBase* factory = getDataFactory(iterStructList->first);
		if (factory != NULL)
		{
			int dataCount = iterStructList->second.size();
			for (int i = 0; i < dataCount; ++i)
			{
				factory->destroyData((iterStructList->second)[i]);
			}
			iterStructList->second.clear();
		}
		else
		{
			TOOL_CORE_ERROR("error : can not find data factory : %d, DataBase::destroyAllData", (int)iterStructList->first);
		}
	}
	mDataStructList.clear();
}

void DataBase::destroyData(const DATA_TYPE& type)
{
	auto iterStructList = mDataStructList.find(type);
	if (iterStructList != mDataStructList.end())
	{
		clearData(type);
		mDataStructList.erase(iterStructList);
	}
}

void DataBase::clearData(const DATA_TYPE& type)
{
	auto iterStructList = mDataStructList.find(type);
	if (iterStructList != mDataStructList.end())
	{
		DataFactoryBase* factory = getDataFactory(iterStructList->first);
		if (factory != NULL)
		{
			int dataCount = iterStructList->second.size();
			for (int i = 0; i < dataCount; ++i)
			{
				factory->destroyData((iterStructList->second)[i]);
			}
			iterStructList->second.clear();
		}
	}
}

void DataBase::loadData(const std::string& filePath, const bool& forceCover)
{
	// 根据文件名查找工厂类型
	std::string fileName = StringUtility::getFileNameNoSuffix(filePath);
	auto iterFileDefine = mDataFileDefine.find(fileName);
	if(iterFileDefine == mDataFileDefine.end())
	{
		TOOL_CORE_ERROR("error : can not find data file define, file name : %s, filePath : %s", fileName.c_str(), filePath.c_str());
		return;
	}

	// 如果该数据已经存在,并且需要覆盖,则先删除数据
	auto iterDataStruct = mDataStructList.find(iterFileDefine->second);
	if (iterDataStruct != mDataStructList.end())
	{
		if (forceCover)
		{
			destroyData(iterDataStruct->first);
		}
		else
		{
			return;
		}
	}

	// 查找工厂
	DataFactoryBase* factory = getDataFactory(iterFileDefine->second);
	if(factory == NULL)
	{
		TOOL_CORE_ERROR("error : can not find factory, type : %d, filename : %s, filePath : %s", (int)iterFileDefine->second, fileName.c_str(), filePath.c_str());
		return;
	}

	// 打开文件
	int fileSize = 0;
	char* fileBuffer = FileUtility::openBinaryFile(filePath, &fileSize);

	// 解析文件
	std::vector<Data*> dataList;
	const int& dataSize = factory->getDataSize();
	int dataCount = fileSize / dataSize;
	for (int i = 0; i < dataCount; ++i)
	{
		Data* newData = factory->createData();
		if(NULL == newData)
		{
			TRACE_DELETE_ARRAY(fileBuffer);
			TOOL_CORE_ERROR("error : can not create data ,type : %d", factory->getType());
			return;
		}
		if(newData->read(fileBuffer + i * dataSize, dataSize))
		{
			dataList.push_back(newData);
		}
	}

	mDataStructList.insert(factory->getType(), dataList);
	TRACE_DELETE_ARRAY(fileBuffer);
}

bool DataBase::writeBinaryFile(const DATA_TYPE& type)
{
	if (mDataPath == EMPTY_STRING)
	{
		TOOL_CORE_INFO("请先设置表格路径!");
		return false;
	}
	auto iterData = mDataStructList.find(type);
	if (iterData == mDataStructList.end())
	{
		return false;
	}

	DataFactoryBase* factory = getDataFactory(type);
	if (factory == NULL)
	{
		TOOL_CORE_ERROR("error : can not find data factory! type : %d", type);
		return false;
	}

	int dataCount = iterData->second.size();
	const int& dataSize = factory->getDataSize();
	int writeBufferSize = dataSize * dataCount;
	char* writeFileBuffer = TRACE_NEW_ARRAY(char, writeBufferSize, writeFileBuffer);
	for (int i = 0; i < dataCount; ++i)
	{
		Data* pData = iterData->second[i];
		pData->write(writeFileBuffer + i * dataSize, dataSize);
	}

	// 将缓冲写入文件
	auto iterDataDefine = mDataDefineFile.find(type);
	if (iterDataDefine == mDataDefineFile.end())
	{
		TOOL_CORE_ERROR("error : can not find data type in data define file list! type : %d", type);
		return false;
	}
	FileUtility::writeFile(mDataPath + iterDataDefine->second + ".bytes", writeBufferSize, writeFileBuffer);
	TRACE_DELETE_ARRAY(writeFileBuffer);
	return true;
}
