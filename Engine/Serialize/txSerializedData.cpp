#include "txSerializedData.h"
#include "Utility.h"

std::string txSerializedData::mCharType = typeid(char).name();
std::string txSerializedData::mUCharType = typeid(unsigned char).name();
std::string txSerializedData::mIntType = typeid(int).name();
std::string txSerializedData::mShortType = typeid(short).name();
std::string txSerializedData::mFloatType = typeid(float).name();
std::string txSerializedData::mCharArrayType = typeid(char*).name();
std::string txSerializedData::mUCharArrayType = typeid(unsigned char*).name();
std::string txSerializedData::mShortArrayType = typeid(short*).name();
std::string txSerializedData::mIntArrayType = typeid(int*).name();
std::string txSerializedData::mFloatArrayType = typeid(float*).name();
txMap<std::string, int> txSerializedData::mTypeSize;

#define ADD_TYPE_SIZE(t) mTypeSize.insert(typeid(t).name(), sizeof(t));
#define ADD_TYPE_PTR_SIZE(t) mTypeSize.insert(typeid(t*).name(), sizeof(t));

txSerializedData::txSerializedData()
:
mDataSize(0)
{
	if (mTypeSize.size() == 0)
	{
		ADD_TYPE_SIZE(char);
		ADD_TYPE_SIZE(unsigned char);
		ADD_TYPE_SIZE(short);
		ADD_TYPE_SIZE(int);
		ADD_TYPE_SIZE(float);
		ADD_TYPE_PTR_SIZE(char);
		ADD_TYPE_PTR_SIZE(unsigned char);
		ADD_TYPE_PTR_SIZE(short);
		ADD_TYPE_PTR_SIZE(int);
		ADD_TYPE_PTR_SIZE(float);
	}
}

bool txSerializedData::readByte(char* dest, char* source, int& curSourceOffset, const int& sourceSize, const int& readSize)
{
	if (curSourceOffset + readSize > sourceSize)
	{
		ENGINE_ERROR("error : read byte error");
		return false;
	}
	memcpy(dest, source + curSourceOffset, readSize);
	curSourceOffset += readSize;
	return true;
}

bool txSerializedData::writeByte(char* dest, char* source, int& destOffset, const int& destSize, const int& writeSize)
{
	if (destOffset + writeSize > destSize)
	{
		ENGINE_ERROR("error : write byte error");
		return false;
	}
	memcpy(dest + destOffset, source, writeSize);
	destOffset += writeSize;
	return true;
}

bool txSerializedData::read(char* pBuffer, const int& bufferSize)
{
	int bufferOffset = 0;
	bool ret = true;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		ret = ret && readByte(mDataParameterList[i].mDataPtr, pBuffer, bufferOffset, bufferSize, mDataParameterList[i].mDataSize);
	}
	END_FOR_STL(mDataParameterList);
	return ret;
}

bool txSerializedData::write(char* pBuffer, const int& bufferSize)
{
	int curWriteSize = 0;
	bool ret = true;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		ret = ret && writeByte(pBuffer, mDataParameterList[i].mDataPtr, curWriteSize, bufferSize, mDataParameterList[i].mDataSize);
	}
	END_FOR_STL(mDataParameterList);
	return ret;
}

bool txSerializedData::writeData(const std::string& dataString, const int& paramIndex)
{
	if (!txMath::isInRange(paramIndex, 0, (int)mDataParameterList.size() - 1))
	{
		return false;
	}
	const DataParameter& dataParam = mDataParameterList[paramIndex];
	const std::string& paramType = dataParam.mDataType;
	if (paramType == mCharType)
	{
		*(dataParam.mDataPtr) = (char)txStringUtility::stringToInt(dataString);
	}
	else if (paramType == mUCharType)
	{
		*(dataParam.mDataPtr) = (unsigned char)txStringUtility::stringToInt(dataString);
	}
	else if (paramType == mIntType)
	{
		*(int*)(dataParam.mDataPtr) = txStringUtility::stringToInt(dataString);
	}
	else if (paramType == mShortType)
	{
		*(short*)(dataParam.mDataPtr) = (short)txStringUtility::stringToInt(dataString);
	}
	else if (paramType == mFloatType)
	{
		*(float*)(dataParam.mDataPtr) = txStringUtility::stringToFloat(dataString);
	}
	else if (paramType == mCharArrayType)
	{
		memset(dataParam.mDataPtr, 0, dataParam.mDataSize);
		int copySize = txMath::getMin((int)dataString.length(), dataParam.mDataSize - 1);
		memcpy(dataParam.mDataPtr, dataString.c_str(), copySize);
	}
	else if (paramType == mShortArrayType)
	{
		txVector<std::string> valueList;
		txStringUtility::split(dataString, ";", valueList);
		int valueCount = valueList.size();
		int arrayLen = dataParam.mDataSize / getTypeSize(paramType);
		for (int i = 0; i < arrayLen; ++i)
		{
			if (i < valueCount)
			{
				((short*)(dataParam.mDataPtr))[i] = (short)txStringUtility::stringToInt(valueList[i]);
			}
			else
			{
				((short*)(dataParam.mDataPtr))[i] = 0;
			}
		}
	}
	else if (paramType == mIntArrayType)
	{
		txVector<std::string> valueList;
		txStringUtility::split(dataString, ";", valueList);
		int valueCount = valueList.size();
		int arrayLen = dataParam.mDataSize / getTypeSize(paramType);
		for (int i = 0; i < arrayLen; ++i)
		{
			if (i < valueCount)
			{
				((int*)(dataParam.mDataPtr))[i] = txStringUtility::stringToInt(valueList[i]);
			}
			else
			{
				((int*)(dataParam.mDataPtr))[i] = 0;
			}
		}
	}
	else if (paramType == mFloatArrayType)
	{
		txVector<std::string> valueList;
		txStringUtility::split(dataString, ";", valueList);
		int valueCount = valueList.size();
		int arrayLen = dataParam.mDataSize / getTypeSize(paramType);
		for (int i = 0; i < arrayLen; ++i)
		{
			if (i < valueCount)
			{
				((float*)(dataParam.mDataPtr))[i] = txStringUtility::stringToFloat(valueList[i]);
			}
			else
			{
				((float*)(dataParam.mDataPtr))[i] = 0.0f;
			}
		}
	}
	return true;
}

bool txSerializedData::writeData(char* buffer, const int& bufferSize, const int& paramIndex)
{
	if (buffer == NULL || !txMath::isInRange(paramIndex, 0, (int)mDataParameterList.size() - 1))
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

std::string txSerializedData::getValueString(const int& paramIndex)
{
	if (!txMath::isInRange(paramIndex, 0, (int)mDataParameterList.size() - 1))
	{
		return EMPTY_STRING;
	}
	const DataParameter& dataParam = mDataParameterList[paramIndex];
	std::string dataString;
	if (dataParam.mDataType == mCharType)
	{
		dataString = txStringUtility::intToString(*(dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mUCharType)
	{
		dataString = txStringUtility::intToString(*((unsigned char*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mIntType)
	{
		dataString = txStringUtility::intToString(*((int*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mShortType)
	{
		dataString = txStringUtility::intToString((int)*((short*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mFloatType)
	{
		dataString = txStringUtility::floatToString(*((float*)dataParam.mDataPtr), 4);
	}
	else if (dataParam.mDataType == mCharArrayType)
	{
		char* temp = TRACE_NEW_ARRAY(char, dataParam.mDataSize + 1, temp);
		memcpy(temp, dataParam.mDataPtr, dataParam.mDataSize);
		dataString = temp;
		TRACE_DELETE_ARRAY(temp);
	}
	else if (dataParam.mDataType == mShortArrayType)
	{
		int intCount = dataParam.mDataSize / sizeof(short);
		for (int i = 0; i < intCount; ++i)
		{
			dataString += txStringUtility::intToString(*(short*)(dataParam.mDataPtr + i * sizeof(short)));
			if (i + 1 < intCount)
			{
				dataString += ";";
			}
		}
	}
	else if (dataParam.mDataType == mIntArrayType)
	{
		int intCount = dataParam.mDataSize / sizeof(int);
		for (int i = 0; i < intCount; ++i)
		{
			dataString += txStringUtility::intToString(*(int*)(dataParam.mDataPtr + i * sizeof(int)));
			if (i + 1 < intCount)
			{
				dataString += ";";
			}
		}
	}
	else if (dataParam.mDataType == mFloatArrayType)
	{
		int intCount = dataParam.mDataSize / sizeof(float);
		for (int i = 0; i < intCount; ++i)
		{
			dataString += txStringUtility::floatToString(*(float*)(dataParam.mDataPtr + i * sizeof(float)), 4);
			if (i + 1 < intCount)
			{
				dataString += ";";
			}
		}
	}
	return dataString;
}

void txSerializedData::zeroParams()
{
	// 数据内容全部清空时,也一起计算数据大小
	mDataSize = 0;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		memset(mDataParameterList[i].mDataPtr, 0, mDataParameterList[i].mDataSize);
		mDataSize += mDataParameterList[i].mDataSize;
	}
	END_FOR_STL(mDataParameterList);
}

bool txSerializedData::readStringList(const txVector<std::string>& dataList)
{
	bool ret = true;
	int curIndex = 0;
	int parameterCount = mDataParameterList.size();
	FOR_STL(mDataParameterList, int i = 0; i < parameterCount; ++i)
	{
		if (curIndex >= (int)dataList.size())
		{
			ret = false;
			break;
		}
		if (!writeData(dataList[curIndex], i))
		{
			ret = false;
			break;
		}
		++curIndex;
	}
	END_FOR_STL(mDataParameterList);
	return ret;
}

void txSerializedData::addParam(const char* param, const std::string& type, const std::string& describe, int paramPos)
{
	int typeSize = getTypeSize(type);
	if (typeSize > 0)
	{
		if (paramPos == -1)
		{
			paramPos = mDataParameterList.size();
		}
		mDataParameterList.insert(mDataParameterList.begin() + paramPos, DataParameter((char*)param, typeSize, type, describe));
		mDataSize += typeSize;
	}
	else
	{
		ENGINE_ERROR(std::string("can not find type " + type + " in type list!").c_str());
	}
}

void txSerializedData::addArrayParam(const char* param, const std::string& type, const int& count, const std::string& describe, int paramPos)
{
	int typeSize = getTypeSize(type);
	if (typeSize > 0)
	{
		if (paramPos == -1)
		{
			paramPos = mDataParameterList.size();
		}
		mDataParameterList.insert(mDataParameterList.begin() + paramPos, DataParameter((char*)param, typeSize * count, type, describe));
		mDataSize += typeSize * count;
	}
	else
	{
		ENGINE_ERROR(std::string("can not find type " + type + " in type list!").c_str());
	}
}

void txSerializedData::deleteParam(const int& paramPos)
{
	if (txMath::isInRange(paramPos, 0, (int)mDataParameterList.size() - 1))
	{
		mDataSize -= mDataParameterList[paramPos].mDataSize;
		mDataParameterList.erase(mDataParameterList.begin() + paramPos);
	}
}

void txSerializedData::moveParam(const int& paramPos, const int& destPos)
{
	if (txMath::isInRange(paramPos, 0, (int)mDataParameterList.size() - 1) && 
		txMath::isInRange(destPos, 0, (int)mDataParameterList.size() - 1))
	{
		DataParameter tempData = mDataParameterList[paramPos];
		mDataParameterList.erase(mDataParameterList.begin() + paramPos);
		mDataParameterList.insert(mDataParameterList.begin() + destPos, tempData);
	}
}

bool txSerializedData::isArrayType(const std::string& type)
{
	return txStringUtility::endWith(type, "*");
}