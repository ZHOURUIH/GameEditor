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
	unsigned short mWidth = 0;
	unsigned short mHeight = 0;
	char mTitle[TITLE_LENGTH] = {0};
	char mUpdateTime[UPDATE_TIME_LENGTH] = {0};
	char mReserved[RESERVED_LENGTH] = {0};
public:
	void parseHeader(char* buffer, int bufferSize, int& offset);
	int getHeaderLength()
	{
		return sizeof(mWidth) + sizeof(mHeight) + TITLE_LENGTH + UPDATE_TIME_LENGTH + RESERVED_LENGTH;
	}
	void saveHeader(txSerializer* serializer);
	string toString();
	void parseString(const string& str);
};

#endif