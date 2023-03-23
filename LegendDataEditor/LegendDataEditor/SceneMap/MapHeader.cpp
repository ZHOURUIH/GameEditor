#include "MapHeader.h"
#include "txSerializer.h"

void MapHeader::parseHeader(char* buffer, int bufferSize, int& offset)
{
	txSerializer serializer(buffer, bufferSize);
	serializer.setIndex(offset);
	serializer.read(mWidth);
	serializer.read(mHeight);
	serializer.readBuffer(mTitle, TITLE_LENGTH);
	serializer.readBuffer(mUpdateTime, UPDATE_TIME_LENGTH);
	serializer.readBuffer(mReserved, RESERVED_LENGTH);
	offset = serializer.getIndex();
}

void MapHeader::saveHeader(txSerializer* serializer)
{
	serializer->write(mWidth);
	serializer->write(mHeight);
	serializer->writeBuffer(mTitle, TITLE_LENGTH);
	serializer->writeBuffer(mUpdateTime, UPDATE_TIME_LENGTH);
	serializer->writeBuffer(mReserved, RESERVED_LENGTH);
}

string MapHeader::toString()
{
	string str;
	str += StringUtility::intToString(mWidth);
	str += " ";
	str += StringUtility::intToString(mHeight);
	return str;
}

void MapHeader::parseString(const string& str)
{
	myVector<string> vec;
	StringUtility::split(str.c_str(), " ", vec);
	if (vec.size() != 2)
	{
		cout << "错误:文本格式的地图文件头读取错误" << endl;
		return;
	}
	mWidth = StringUtility::stringToInt(vec[0]);
	mHeight = StringUtility::stringToInt(vec[1]);
}