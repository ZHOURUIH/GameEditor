#include "DataTemplate.h"
#include "txUtility.h"
#include "DataDefine.h"
#include "DataBase.h"

void DataTemplate::destroy()
{
	int paramCount = mDataParameterList.size();
	for (int i = 0; i < paramCount; ++i)
	{
		TRACE_DELETE_ARRAY(mDataParameterList[i].mDataPtr);
	}
}

bool DataTemplate::configParam(const std::vector<std::vector<std::string> >& paramList)
{
	int listSize = paramList.size();
	for (int i = 0; i < listSize; ++i)
	{
		const std::string& describe = paramList[i][0];
		const std::string& paramType = paramList[i][1];
		int typeSize = Data::getTypeSize(paramType);
		if (typeSize > 0)
		{
			if (txUtility::endWith(paramType, "*"))
			{
				int count = txUtility::stringToInt(paramList[i][2]);
				char* param = TRACE_NEW_ARRAY(char, typeSize * count, param);
				pushArrayParam(param, paramType, count, describe);
			}
			else
			{
				char* param = TRACE_NEW_ARRAY(char, typeSize, param);
				pushParam(param, paramType, describe);
			}
		}
		else
		{
			ENGINE_ERROR("type error!");
			return false;
		}
	}
	// 配置完表格模板后清零数据参数，并计算数据大小
	zeroParams();
	return true;
}

bool DataTemplate::getConfigParam(std::vector<std::string>& paramList)
{
	int paramCount = mDataParameterList.size();
	for (int i = 0; i < paramCount; ++i)
	{
		const DataParameter& dataParameter = mDataParameterList[i];
		std::string paramLine;
		paramLine += dataParameter.mDescribe + "\t";
		bool isArray = txUtility::endWith(dataParameter.mDataType, "*");
		if (isArray)
		{
			paramLine += dataParameter.mDataType + "\t";
			int arrayElementCount = dataParameter.mDataSize / Data::getTypeSize(dataParameter.mDataType);
			paramLine += txUtility::intToString(arrayElementCount);
		}
		else
		{
			paramLine += dataParameter.mDataType;
		}
		paramList.push_back(paramLine);
	}
	return true;
}

Data* DataTemplate::createData() const
{
	Data* data = TRACE_NEW(Data, data, mType);
	int paramCount = mDataParameterList.size();
	for (int i = 0; i < paramCount; ++i)
	{
		const DataParameter& param = mDataParameterList[i];
		char* dataPtr = TRACE_NEW_ARRAY(char, param.mDataSize, dataPtr);
		if (txSerializedData::isArrayType(param.mDataType))
		{
			data->pushArrayParam(dataPtr, param.mDataType, param.mDataSize / Data::getTypeSize(param.mDataType), param.mDescribe);
		}
		else
		{
			data->pushParam(dataPtr, param.mDataType, param.mDescribe);
		}
	}
	// 添加完参数后清零并计算数据大小
	data->zeroParams();
	return data;
}

void DataTemplate::destroyData(Data* data) const
{
	if (data == NULL || data->getType() != mType)
	{
		ENGINE_ERROR("destroy wrong data!");
		return;
	}
	int paramCount = data->getParameterList().size();
	for (int i = 0; i < paramCount; ++i)
	{
		TRACE_DELETE_ARRAY(const_cast<char*>(data->getParameterList()[i].mDataPtr));
	}
	TRACE_DELETE(data);
}

void DataTemplate::addTemplateParam(const std::string& type, const std::string& describe, int paramPos, std::vector<Data*> dataList)
{
	int typeSize = Data::getTypeSize(type);
	char* param = TRACE_NEW_ARRAY(char, typeSize, param);
	addParam(param, type, describe, paramPos);

	int dataCount = dataList.size();
	for (int i = 0; i < dataCount; ++i)
	{
		if (dataList[i]->getType() != mType)
		{
			ENGINE_ERROR("error : wrong data type!");
			break;
		}
		char* dataParam = TRACE_NEW_ARRAY(char, typeSize, dataParam);
		dataList[i]->addParam(dataParam, type, describe, paramPos);
	}
}

void DataTemplate::addTemplateArrayParam(const std::string& type, int count, const std::string& describe, int paramPos, std::vector<Data*> dataList)
{
	int typeSize = Data::getTypeSize(type);
	char* param = TRACE_NEW_ARRAY(char, typeSize * count, param);
	addArrayParam(param, type, count, describe, paramPos);

	int dataCount = dataList.size();
	for (int i = 0; i < dataCount; ++i)
	{
		if (dataList[i]->getType() != mType)
		{
			ENGINE_ERROR("error : wrong data type!");
			break;
		}
		char* dataParam = TRACE_NEW_ARRAY(char, typeSize * count, dataParam);
		dataList[i]->addArrayParam(dataParam, type, count, describe, paramPos);
	}
}

void DataTemplate::deleteTemplateParam(int paramPos, std::vector<Data*> dataList)
{
	if (paramPos >= 0 && paramPos < (int)mDataParameterList.size())
	{
		TRACE_DELETE(mDataParameterList[paramPos].mDataPtr);
		deleteParam(paramPos);

		int dataCount = dataList.size();
		for (int i = 0; i < dataCount; ++i)
		{
			if (dataList[i]->getType() != mType)
			{
				ENGINE_ERROR("error : wrong data type!");
				break;
			}
			TRACE_DELETE_ARRAY(const_cast<char*>(dataList[i]->getParameterList()[paramPos].mDataPtr));
			dataList[i]->deleteParam(paramPos);
		}
	}
}

void DataTemplate::moveTemplateParam(int paramPos, int destPos, std::vector<Data*> dataList)
{
	if (paramPos >= 0 && paramPos < (int)mDataParameterList.size() && destPos >= 0 && destPos < (int)mDataParameterList.size())
	{
		moveParam(paramPos, destPos);

		int dataCount = dataList.size();
		for (int i = 0; i < dataCount; ++i)
		{
			if (dataList[i]->getType() != mType)
			{
				ENGINE_ERROR("error : wrong data type!");
				break;
			}
			dataList[i]->moveParam(paramPos, destPos);
		}
	}
}