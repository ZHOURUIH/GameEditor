#include "DataBase.h"
#include "Data.h"
#include "DataTemplate.h"
#include "txUtility.h"

errorMessage DataBase::mErrorCallback = NULL;

void DataBase::init(const bool& loadAllData)
{
	initDataTemplate();
	if (loadAllData)
	{
		loadAllDataFromFile();
	}
}

void DataBase::initDataTemplate()
{
	std::vector<std::string> fileList;
	std::vector<std::string> patterns;
	patterns.push_back(TEMPLATE_SUFFIX);
	txUtility::findFiles(txUtility::getAvailableResourcePath(DATA_TEMPLATE_PATH).c_str(), fileList, patterns);
	int fileCount = fileList.size();
	for (int i = 0; i < fileCount; ++i)
	{
		readTemplateConfig(txUtility::getFileName(fileList[i]));
	}
}

void DataBase::destroyDataTemplate()
{
	std::map<std::string, DataTemplate*>::iterator iterTemplate = mDataTemplateList.begin();
	std::map<std::string, DataTemplate*>::iterator iterTemplateEnd = mDataTemplateList.end();
	for (; iterTemplate != iterTemplateEnd; ++iterTemplate)
	{
		TRACE_DELETE(iterTemplate->second);
	}
	mDataTemplateList.clear();
}

bool DataBase::deleteData(const std::string& type, const int& index)
{
	std::map<std::string, std::vector<Data*> >::iterator iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (index >= (int)iter->second.size())
		{
			return false;
		}
		const DataTemplate* dataTemplate = getDataTemplate(type);
		if (dataTemplate == NULL)
		{
			return false;
		}
		dataTemplate->destroyData(iter->second[index]);
		iter->second.erase(iter->second.begin() + index);
		return true;
	}
	return false;
}

void DataBase::loadAllDataFromFile()
{
	// 读取配置文件，获得需要加载的所有数据列表
	// 遍历每一个文件名，加载相应的文件
	std::vector<std::string> fileList;
	std::vector<std::string> patterns;
	patterns.push_back(DATA_SUFFIX);
	txUtility::findFiles(txUtility::getAvailableResourcePath(GAME_DATA_PATH).c_str(), fileList, patterns);
	int listSize = fileList.size();
	for(int i = 0; i < listSize; ++i)
	{
		loadData(fileList[i], true, false);
	}
}

void DataBase::destroyAllData()
{
	std::map<std::string, std::vector<Data*> >::iterator iterStructList = mDataStructList.begin();
	std::map<std::string, std::vector<Data*> >::iterator iterStructListEnd = mDataStructList.end();
	for(; iterStructList != iterStructListEnd; ++iterStructList)
	{
		const DataTemplate* dataTemplate = getDataTemplate(iterStructList->first);
		if (dataTemplate != NULL)
		{
			int dataCount = iterStructList->second.size();
			for (int i = 0; i < dataCount; ++i)
			{
				dataTemplate->destroyData((iterStructList->second)[i]);
			}
			iterStructList->second.clear();
		}
	}
	mDataStructList.clear();
}

void DataBase::destroyData(const std::string& type)
{
	std::map<std::string, std::vector<Data*> >::iterator iterStructList = mDataStructList.find(type);
	if (iterStructList != mDataStructList.end())
	{
		const DataTemplate* dataTemplate = getDataTemplate(iterStructList->first);
		if (dataTemplate != NULL)
		{
			int dataCount = iterStructList->second.size();
			for (int i = 0; i < dataCount; ++i)
			{
				dataTemplate->destroyData((iterStructList->second)[i]);
			}
			iterStructList->second.clear();
		}
		mDataStructList.erase(iterStructList);
	}
}

void DataBase::loadData(const std::string& filePath, const bool& forceCover, const bool& reloadTemplate)
{
	// 如果该数据已经存在,并且需要覆盖,则先删除数据
	std::string dataType = txUtility::getFileNameNoSuffix(filePath);
	std::map<std::string, std::vector<Data*> >::iterator iterDataStruct = mDataStructList.find(dataType);
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
	// 如果重新加载模板,则先销毁当前模板,然后从文件读取创建新模板
	if (reloadTemplate)
	{
		destroyTemplate(dataType);
		readTemplateConfig(dataType + TEMPLATE_SUFFIX);
	}
	const DataTemplate* dataTemplate = getDataTemplate(dataType);
	if (dataTemplate == NULL)
	{
		return;
	}

	char* fileBuffer = NULL;
	int bufferSize = 0;
	txUtility::loadFile(filePath, fileBuffer, bufferSize, false);

	// 解析文件
	if (fileBuffer != NULL)
	{
		std::vector<Data*> dataList;
		int dataSize = dataTemplate->getSize();
		int dataCount = bufferSize / dataSize;
		for (int i = 0; i < dataCount; ++i)
		{
			Data* newData = dataTemplate->createData();
			if (NULL == newData)
			{
				TRACE_DELETE_ARRAY(fileBuffer);
				return;
			}
			if (newData->read(fileBuffer + i * dataSize, dataSize))
			{
				dataList.push_back(newData);
			}
		}

		mDataStructList.insert(std::pair<std::string, std::vector<Data*> >(dataTemplate->getType(), dataList));
		TRACE_DELETE_ARRAY(fileBuffer);
	}
	// 当文件是空文件的时候,只是创建一个空的数据列表
	else
	{
		newData(dataTemplate->getType());
	}
}

bool DataBase::writeBinaryFile(const std::string& type)
{
	std::map<std::string, std::vector<Data*> >::iterator iterData = mDataStructList.find(type);
	if (iterData == mDataStructList.end())
	{
		return false;
	}

	const DataTemplate* dataTemplate = getDataTemplate(type);
	if (dataTemplate == NULL)
	{
		return false;
	}

	int dataCount = iterData->second.size();
	int dataSize = dataTemplate->getSize();
	int writeBufferSize = dataSize * dataCount;
	if (writeBufferSize > 0)
	{
		char* writeFileBuffer = TRACE_NEW_ARRAY(char, writeBufferSize, writeFileBuffer);
		for (int i = 0; i < dataCount; ++i)
		{
			Data* pData = iterData->second[i];
			pData->write(writeFileBuffer + i * dataSize, dataSize);
		}
		// 将缓冲写入文件
		txUtility::writeFile(txUtility::getAvailableResourcePath(GAME_DATA_PATH + dataTemplate->getType() + DATA_SUFFIX), writeBufferSize, writeFileBuffer);
		TRACE_DELETE_ARRAY(writeFileBuffer);
	}
	else
	{
		// 即便数据是空的,也要创建一个空的文件
		txUtility::writeFile(txUtility::getAvailableResourcePath(GAME_DATA_PATH + dataTemplate->getType() + DATA_SUFFIX), 0, NULL);
	}
	return true;
}

bool DataBase::txtToBinary(const std::string& txtFileName)
{
	std::string type = txUtility::getFileNameNoSuffix(txtFileName);

	// 查找该类型的工厂
	const DataTemplate* dataTemplate = getDataTemplate(type);
	if (dataTemplate == NULL)
	{
		ENGINE_ERROR("找不到文件对应的工厂");
		return false;
	}

	char* pFileBuffer = NULL;
	int bufferSize = 0;
	txUtility::loadFile(txUtility::getAvailableResourcePath(GAME_DATA_PATH + txtFileName), pFileBuffer, bufferSize, true);
	std::string fileBufferString(pFileBuffer);
	TRACE_DELETE_ARRAY(pFileBuffer);
	std::vector<Data*> dataList;
	std::vector<std::string> fileLine;
	txUtility::split(fileBufferString, "\r\n", &fileLine);
	int lineCount = fileLine.size();
	for (int i = 0; i < lineCount; ++i)
	{
		if (i >= DROP_LINE)
		{
			Data* pData = dataTemplate->createData();
			std::vector<std::string> paramList;
			txUtility::split(fileLine[i], "\t", &paramList);
			if (!pData->readStringList(paramList))
			{
				ENGINE_ERROR("文本文件错误");
				return false;
			}
			dataList.push_back(pData);
		}
	}
	// 将数据写入缓冲
	int dataSize = dataTemplate->getSize();
	int iconInfoListSize = dataList.size();
	int writeBufferSize = dataSize * iconInfoListSize;
	char* writeFileBuffer = TRACE_NEW_ARRAY(char, writeBufferSize, writeFileBuffer);
	for (int i = 0; i < iconInfoListSize; ++i)
	{
		dataList[i]->write(writeFileBuffer + i * dataSize, dataSize);
	}

	// 将缓冲写入文件
	txUtility::writeFile(txUtility::getAvailableResourcePath(GAME_DATA_PATH + type + DATA_SUFFIX), writeBufferSize, writeFileBuffer);
	TRACE_DELETE_ARRAY(writeFileBuffer);
	return true;
}

DataTemplate* DataBase::createDataTemplate(const std::string& type, const std::vector<std::string>& paramList)
{
	std::vector<std::vector<std::string> > params;
	int paramListSize = paramList.size();
	for (int i = 0; i < paramListSize; ++i)
	{
		std::vector<std::string> paramLine;
		txUtility::split(paramList[i], "\t", &paramLine);
		params.push_back(paramLine);
	}
	return createDataTemplate(type, params);
}

DataTemplate* DataBase::createDataTemplate(const std::string& type, const std::vector<std::vector<std::string> >& paramList)
{
	if (mDataTemplateList.find(type) == mDataTemplateList.end())
	{
		DataTemplate* data = TRACE_NEW(DataTemplate, data, type);
		// 解析参数列表
		data->configParam(paramList);
		mDataTemplateList.insert(std::make_pair(type, data));
		return data;
	}
	else
	{
		return NULL;
	}
}

Data* DataBase::createData(const std::string& type)
{
	const DataTemplate* dataTemplate = getDataTemplate(type);
	if (dataTemplate != NULL)
	{
		return dataTemplate->createData();
	}
	return NULL;
}

void DataBase::destroyData(Data* data)
{
	if (data == NULL)
	{
		return;
	}
	const DataTemplate* dataTemplate = getDataTemplate(data->getType());
	if (dataTemplate != NULL)
	{
		return dataTemplate->destroyData(data);
	}
}

int DataBase::getDataCount(const std::string& type)
{
	std::map<std::string, std::vector<Data*> >::iterator iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		return (int)iter->second.size();
	}
	return 0;
}

Data* DataBase::queryData(const std::string& type, int index)
{
	std::map<std::string, std::vector<Data*> >::iterator iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (index < (int)iter->second.size())
		{
			return (iter->second)[index];
		}
	}
	return NULL;
}

bool DataBase::addData(const std::string& type, Data* data, int pos)
{
	std::map<std::string, std::vector<Data*> >::iterator iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		if (pos < 0)
		{
			iter->second.push_back(data);
		}
		else if (pos >= 0 && pos <= (int)iter->second.size())
		{
			iter->second.insert(iter->second.begin() + pos, data);
		}
		return true;
	}
	return false;
}

bool DataBase::newData(const std::string& type)
{
	std::map<std::string, std::vector<Data*> >::iterator iter = mDataStructList.find(type);
	if (iter != mDataStructList.end())
	{
		return false;
	}
	mDataStructList.insert(std::make_pair(type, std::vector<Data*>()));
	return true;
}

void DataBase::readTemplateConfig(const std::string& fileName)
{
	char* buffer = NULL;
	int bufferSize = 0;
	if (!txUtility::loadFile(txUtility::getAvailableResourcePath(DATA_TEMPLATE_PATH + fileName), buffer, bufferSize, true))
	{
		return;
	}
	std::vector<std::string> paramList;
	txUtility::split(buffer, "\r\n", &paramList);
	createDataTemplate(txUtility::getFileNameNoSuffix(fileName), paramList);
	TRACE_DELETE_ARRAY(buffer);
}

void DataBase::destroyTemplate(const std::string& templateName)
{
	std::map<std::string, DataTemplate*>::iterator iterTemplate = mDataTemplateList.find(templateName);
	if (iterTemplate != mDataTemplateList.end())
	{
		TRACE_DELETE(iterTemplate->second);
		mDataTemplateList.erase(iterTemplate);
	}
}

void DataBase::writeTemplateConfig(const std::string& type)
{
	std::map<std::string, DataTemplate*>::iterator iterTemplate = mDataTemplateList.find(type);
	if (iterTemplate != mDataTemplateList.end())
	{
		std::vector<std::string> paramList;
		iterTemplate->second->getConfigParam(paramList);
		std::string fileData;
		int paramCount = paramList.size();
		for (int i = 0; i < paramCount; ++i)
		{
			fileData += paramList[i];
			if (i != paramCount - 1)
			{
				fileData += "\r\n";
			}
		}
		txUtility::writeFile(txUtility::getAvailableResourcePath(DATA_TEMPLATE_PATH + type + TEMPLATE_SUFFIX), fileData.length(), fileData.c_str());
	}
}

void DataBase::addTemplateParam(const std::string& templateName, const std::string& type, const std::string& describe, int paramPos)
{
	std::map<std::string, DataTemplate*>::iterator itrTemplate = mDataTemplateList.find(templateName);
	std::map<std::string, std::vector<Data*> >::iterator iterData = mDataStructList.find(templateName);
	if (itrTemplate != mDataTemplateList.end() && iterData != mDataStructList.end())
	{
		DataTemplate* dataTemplate = itrTemplate->second;
		dataTemplate->addTemplateParam(type, describe, paramPos, iterData->second);
	}
}

void DataBase::addTemplateArrayParam(const std::string& templateName, const std::string& type, int count, const std::string& describe, int paramPos)
{
	std::map<std::string, DataTemplate*>::iterator itrTemplate = mDataTemplateList.find(templateName);
	std::map<std::string, std::vector<Data*> >::iterator iterData = mDataStructList.find(templateName);
	if (itrTemplate != mDataTemplateList.end() && iterData != mDataStructList.end())
	{
		DataTemplate* dataTemplate = itrTemplate->second;
		dataTemplate->addTemplateArrayParam(type, count, describe, paramPos, iterData->second);
	}
}

void DataBase::deleteTemplateParam(const std::string& templateName, int paramPos)
{
	std::map<std::string, DataTemplate*>::iterator itrTemplate = mDataTemplateList.find(templateName);
	std::map<std::string, std::vector<Data*> >::iterator iterData = mDataStructList.find(templateName);
	if (itrTemplate != mDataTemplateList.end() && iterData != mDataStructList.end())
	{
		DataTemplate* dataTemplate = itrTemplate->second;
		dataTemplate->deleteTemplateParam(paramPos, iterData->second);
	}
}

void DataBase::moveTemplateParam(const std::string& templateName, int paramPos, int destPos)
{
	std::map<std::string, DataTemplate*>::iterator itrTemplate = mDataTemplateList.find(templateName);
	std::map<std::string, std::vector<Data*> >::iterator iterData = mDataStructList.find(templateName);
	if (itrTemplate != mDataTemplateList.end() && iterData != mDataStructList.end())
	{
		DataTemplate* dataTemplate = itrTemplate->second;
		dataTemplate->moveTemplateParam(paramPos, destPos, iterData->second);
	}
}