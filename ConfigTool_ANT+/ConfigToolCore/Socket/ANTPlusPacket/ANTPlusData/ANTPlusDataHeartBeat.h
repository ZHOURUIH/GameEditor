#ifndef _ANT_PLUS_DATA_HEART_BEAT_H_
#define _ANT_PLUS_DATA_HEART_BEAT_H_

#include "ANTPlusData.h"

class ANTPlusPacket;
class ANTPlusDataHeartBeat : public ANTPlusData
{
public:
	unsigned char mBattery;	// µÁ¡ø
	short mReserve;
public:
	ANTPlusDataHeartBeat(ANTPlusPacket* packet);
protected:
	virtual void parseData();
};

#endif