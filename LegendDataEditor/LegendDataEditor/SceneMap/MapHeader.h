#ifndef _MAP_HEADER_H_
#define _MAP_HEADER_H_

#include "ServerDefine.h"

const int TITLE_LENGTH = 17;
const int UPDATE_TIME_LENGTH = 8;
const int RESERVED_LENGTH = 23;

class txSerializer;
class MapHeader
{
public:
	unsigned short mWidth;
	unsigned short mHeight;
	char mTitle[TITLE_LENGTH];
	char mUpdateTime[UPDATE_TIME_LENGTH];
	char mReserved[RESERVED_LENGTH];
public:
	void parseHeader(char* buffer, int bufferSize, int& offset);
	int getHeaderLength()
	{
		return sizeof(mWidth) + sizeof(mHeight) + TITLE_LENGTH + UPDATE_TIME_LENGTH + RESERVED_LENGTH;
	}
	void saveHeader(txSerializer* serializer);
};

#endif