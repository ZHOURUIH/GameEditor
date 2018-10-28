#ifndef _ANT_PLUS_DATA_RIDING_CADENCE_SPEED_H_
#define _ANT_PLUS_DATA_RIDING_CADENCE_SPEED_H_

#include "ANTPlusDataRiding.h"

class ANTPlusPacket;
class ANTPlusDataRidingCadenceSpeed : public ANTPlusDataRiding
{
public:
	ANTPlusDataRidingCadenceSpeed(ANTPlusPacket* packet);
	virtual float getSpeed(float perimeter);
	virtual int getCadence();
	virtual bool isSameDataValue(ANTPlusDataRiding* newData);
};

#endif