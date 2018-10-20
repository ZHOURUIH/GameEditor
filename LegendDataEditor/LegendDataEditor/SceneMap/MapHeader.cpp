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