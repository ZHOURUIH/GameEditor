#include "txSerializedData.h"
#include "txUtility.h"
#include "DataBase.h"

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
std::map<std::string, int> txSerializedData::mTypeSize;

#define ADD_TYPE_SIZE(t) mTypeSize.insert(std::make_pair(typeid(t).name(), sizeof(t)));
#define ADD_TYPE_PTR_SIZE(t) mTypeSize.insert(std::make_pair(typeid(t*).name(), sizeof(t)));

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
	for (int i = 0; i < parameterCount; ++i)
	{
		ret = ret && readByte(mDataParameterList[i].mDataPtr, pBuffer, bufferOffset, bufferSize, mDataParameterList[i].mDataSize);
	}
	return ret;
}

bool txSerializedData::write(char* pBuffer, const int& bufferSize)
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

bool txSerializedData::writeData(const std::string& dataString, const int& paramIndex)
{
	if (paramIndex < 0 || paramIndex >= (int)mDataParameterList.size())
	{
		return false;
	}
	const DataParameter& dataParam = mDataParameterList[paramIndex];
	const std::string& paramType = dataParam.mDataType;
	if (paramType == mIntType)
	{
		*(int*)(dataParam.mDataPtr) = txUtility::stringToInt(dataString);
	}
	else if (paramType == mShortType)
	{
		*(short*)(dataParam.mDataPtr) = (short)txUtility::stringToInt(dataString);
	}
	else if (paramType == mFloatType)
	{
		*(float*)(dataParam.mDataPtr) = txUtility::stringToFloat(dataString);
	}
	else if (paramType == mCharArrayType)
	{
		memset(dataParam.mDataPtr, 0, dataParam.mDataSize);
		int copySize = TX_MIN((int)dataString.length(), dataParam.mDataSize - 1);
		memcpy(dataParam.mDataPtr, dataString.c_str(), copySize);
	}
	else if (paramType == mShortArrayType)
	{
		std::vector<std::string> valueList;
		txUtility::split(dataString, ";", &valueList);
		int valueCount = valueList.size();
		int arrayLen = dataParam.mDataSize / getTypeSize(paramType);
		for (int i = 0; i < arrayLen; ++i)
		{
			if (i < valueCount)
			{
				((short*)(dataParam.mDataPtr))[i] = (short)txUtility::stringToInt(valueList[i]);
			}
			else
			{
				((short*)(dataParam.mDataPtr))[i] = 0;
			}
		}
	}
	else if (paramType == mIntArrayType)
	{
		std::vector<std::string> valueList;
		txUtility::split(dataString, ";", &valueList);
		int valueCount = valueList.size();
		int arrayLen = dataParam.mDataSize / getTypeSize(paramType);
		for (int i = 0; i < arrayLen; ++i)
		{
			if (i < valueCount)
			{
				((int*)(dataParam.mDataPtr))[i] = txUtility::stringToInt(valueList[i]);
			}
			else
			{
				((int*)(dataParam.mDataPtr))[i] = 0;
			}
		}
	}
	else if (paramType == mFloatArrayType)
	{
		std::vector<std::string> valueList;
		txUtility::split(dataString, ";", &valueList);
		int valueCount = valueList.size();
		int arrayLen = dataParam.mDataSize / getTypeSize(paramType);
		for (int i = 0; i < arrayLen; ++i)
		{
			if (i < valueCount)
			{
				((float*)(dataParam.mDataPtr))[i] = txUtility::stringToFloat(valueList[i]);
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

std::string txSerializedData::getValueString(const int& paramIndex)
{
	if (paramIndex < 0 || paramIndex >= (int)mDataParameterList.size())
	{
		return "";
	}
	const DataParameter& dataParam = mDataParameterList[paramIndex];
	std::string dataString;
	if (dataParam.mDataType == mIntType)
	{
		dataString = txUtility::intToString(*((int*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mShortType)
	{
		dataString = txUtility::intToString((int)*((short*)dataParam.mDataPtr));
	}
	else if (dataParam.mDataType == mFloatType)
	{
		dataString = txUtility::floatToString(*((float*)dataParam.mDataPtr), 4);
	}
	else if (dataParam.mDataType == mCharArrayType)
	{
		dataString = dataParam.mDataPtr;
	}
	else if (dataParam.mDataType == mShortArrayType)
	{
		int intCount = dataParam.mDataSize / sizeof(short);
		for (int i = 0; i < intCount; ++i)
		{
			dataString += txUtility::intToString(*(short*)(dataParam.mDataPtr + i * sizeof(short)));
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
			dataString += txUtility::intToString(*(int*)(dataParam.mDataPtr + i * sizeof(int)));
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
			dataString += txUtility::floatToString(*(float*)(dataParam.mDataPtr + i * sizeof(float)), 4);
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
	for (int i = 0; i < parameterCount; ++i)
	{
		memset(mDataParameterList[i].mDataPtr, 0, mDataParameterList[i].mDataSize);
		mDataSize += mDataParameterList[i].mDataSize;
	}
}

bool txSerializedData::readStringList(const std::vector<std::string>& dataList)
{
	int curIndex = 0;
	int parameterCount = mDataParameterList.size();
	for (int i = 0; i < parameterCount; ++i)
	{
		if (curIndex >= (int)dataList.size())
		{
			return false;
		}
		if (!writeData(dataList[curIndex], i))
		{
			return false;
		}
		++curIndex;
	}
	return true;
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

void txSerializedData::addArrayParam(const char* param, const std::string& type, int count, const std::string& describe, int paramPos)
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

void txSerializedData::deleteParam(int paramPos)
{
	if (paramPos >= 0 && paramPos < (int)mDataParameterList.size())
	{
		mDataSize -= mDataParameterList[paramPos].mDataSize;
		mDataParameterList.erase(mDataParameterList.begin() + paramPos);
	}
}

void txSerializedData::moveParam(int paramPos, int destPos)
{
	if (paramPos >= 0 && paramPos < (int)mDataParameterList.size() && destPos >= 0 && destPos < (int)mDataParameterList.size())
	{
		DataParameter tempData = mDataParameterList[paramPos];
		mDataParameterList.erase(mDataParameterList.begin() + paramPos);
		mDataParameterList.insert(mDataParameterList.begin() + destPos, tempData);
	}
}

bool txSerializedData::isArrayType(const std::string& type)
{
	return txUtility::endWith(type, "*");
}