#include "txUtility.h"
#include "txSerializedData.h"

bool txSerializedData::read(char* pBuffer, int bufferSize)
{
	int bufferOffset = 0;
	bool ret = true;
	int parameterCount = mDataParameterList.size();
	for (int i = 0; i < parameterCount; ++i)
	{
		ret = ret && readByte(mDataParameterList[i].mDataPtr, pBuffer, bufferOffset, bufferSize, mDataParameterList[i].mDataSize);
	}
	return ret;
}

bool txSerializedData::write(char* pBuffer, int bufferSize)
{
	int curWriteSize = 0;
	bool ret = true;
	int parameterCount = mDataParameterList.size();
	for (int i = 0; i < parameterCount; ++i)
	{
		ret = ret && writeByte(pBuffer, mDataParameterList[i].mDataPtr, curWriteSize, bufferSize, mDataParameterList[i].mDataSize);
	}
	return ret;
}

bool txSerializedData::writeData(std::string dataString, int paramIndex)
{
	if (paramIndex < 0 || paramIndex >= (int)mDataParameterList.size())
	{
		return false;
	}
	DataParameter dataParam = mDataParameterList[paramIndex];
	std::string paramType = dataParam.mDataType;
	if (paramType == typeid(int).name())
	{
		*(int*)(dataParam.mDataPtr) = StringUtility::stringToInt(dataString);
	}
	else if (paramType == typeid(short).name())
	{
		*(short*)(dataParam.mDataPtr) = StringUtility::stringToInt(dataString);
	}
	else if (paramType == typeid(float).name())
	{
		*(float*)(dataParam.mDataPtr) = StringUtility::stringToFloat(dataString);
	}
	else if (paramType == typeid(char*).name())
	{
		memset(dataParam.mDataPtr, 0, dataParam.mDataSize);
		int copySize = TX_MIN((int)dataString.length(), dataParam.mDataSize - 1);
		memcpy(dataParam.mDataPtr, dataString.c_str(), copySize);
	}
	else if (paramType == typeid(int*).name())
	{
		txVector<std::string> valueList;
		StringUtility::split(dataString, ";", valueList);
		int valueCount = valueList.size();
		valueCount = TX_MIN(valueCount, 2);
		for (int i = 0; i < valueCount; ++i)
		{
			((int*)(dataParam.mDataPtr))[i] = StringUtility::stringToInt(valueList[i]);
		}
	}
	return true;
}

bool txSerializedData::writeData(char* buffer, int bufferSize, int paramIndex)
{
	if (buffer == NULL || paramIndex < 0 || paramIndex >= (int)mDataParameterList.size())
	{
		return false;
	}
	if (bufferSize < mDataParameterList[paramIndex].mDataSize)
	{
		return false;
	}
	memcpy(mDataParameterList[paramIndex].mDataPtr, buffer, mDataParameterList[paramIndex].mDataSize);
	return true;
}

std::string txSerializedData::getValueString(int paramIndex)
{
	DataParameter dataParam = mDataParameterList[paramIndex];
	std::string dataString;
	if (dataParam.mDataType == typeid(int).name())
	{
		dataString = StringUtility::intToString(*((int*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == typeid(short).name())
	{
		dataString = StringUtility::intToString((int)*((short*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == typeid(float).name())
	{
		dataString = StringUtility::floatToString(*((float*)dataParam.mDataPtr), 2);
	}
	else if (dataParam.mDataType == typeid(char*).name())
	{
		dataString = dataParam.mDataPtr;
	}
	else if (dataParam.mDataType == typeid(int*).name())
	{
		int intCount = dataParam.mDataSize / sizeof(int);
		for (int i = 0; i < intCount; ++i)
		{
			dataString += StringUtility::intToString(*(int*)(dataParam.mDataPtr + i * sizeof(int)));
			if (i + 1 < intCount)
			{
				dataString += ";";
			}
		}
	}
	return dataString;
}

int txSerializedData::getSize()
{
	int dataSize = 0;
	int parameterCount = mDataParameterList.size();
	for (int i = 0; i < parameterCount; ++i)
	{
		dataSize += mDataParameterList[i].mDataSize;
	}
	return dataSize;
}

void txSerializedData::zeroParams()
{
	int parameterCount = mDataParameterList.size();
	for (int i = 0; i < parameterCount; ++i)
	{
		memset(mDataParameterList[i].mDataPtr, 0, mDataParameterList[i].mDataSize);
	}
}

bool txSerializedData::readStringList(txVector<std::string>& dataList)
{
	int curIndex = 0;
	int parameterCount = mDataParameterList.size();
	for (int i = 0; i < parameterCount; ++i)
	{
		if (curIndex >= (int)dataList.size())
		{
			return false;
		}
		DataParameter paramter = mDataParameterList[i];
		if (paramter.mDataType == typeid(int).name())
		{
			*(int*)(paramter.mDataPtr) = atoi(dataList[curIndex].c_str());
		}
		else if (paramter.mDataType == typeid(short).name())
		{
			*(short*)(paramter.mDataPtr) = atoi(dataList[curIndex].c_str());
		}
		else if (paramter.mDataType == typeid(float).name())
		{
			*(float*)(paramter.mDataPtr) = (float)atof(dataList[curIndex].c_str());
		}
		else if (paramter.mDataType == typeid(char*).name())
		{
			memset(mDataParameterList[i].mDataPtr, 0, mDataParameterList[i].mDataSize);
			int copySize = TX_MIN((int)dataList[curIndex].length(), mDataParameterList[i].mDataSize - 1);
			memcpy(mDataParameterList[i].mDataPtr, dataList[curIndex].c_str(), copySize);
		}
		else if (paramter.mDataType == typeid(int*).name())
		{
			txVector<std::string> breakVec;
			StringUtility::split(dataList[curIndex], ";", breakVec);
			int size = breakVec.size();
			size = TX_MIN(size, 2);

			for (int j = 0; j < size; ++j)
			{
				((int*)(paramter.mDataPtr))[j] = atoi(breakVec[j].c_str());
			}
		}
		++curIndex;
	}
	return true;
}