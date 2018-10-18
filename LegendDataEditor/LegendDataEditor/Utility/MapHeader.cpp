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