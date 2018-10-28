#ifndef _ANT_PLUS_DATA_RIDINNG_HEAR_RATE_H_
#define _ANT_PLUS_DATA_RIDINNG_HEAR_RATE_H_

#include "ANTPlusDataRiding.h"

class ANTPlusPacket;
class ANTPlusDataRidingHeartRate : public ANTPlusDataRiding
{
public:
	ANTPlusDataRidingHeartRate(ANTPlusPacket* packet);
	virtual int getHeartRate();
};

#endif