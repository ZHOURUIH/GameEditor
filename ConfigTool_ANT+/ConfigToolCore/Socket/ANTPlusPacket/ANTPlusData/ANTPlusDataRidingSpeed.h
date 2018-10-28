#ifndef _ANT_PLUS_DATA_RIDING_SPEED_H_
#define _ANT_PLUS_DATA_RIDING_SPEED_H_

#include "ANTPlusDataRiding.h"

class ANTPlusPacket;
class ANTPlusDataRidingSpeed : public ANTPlusDataRiding
{
public:
	ANTPlusDataRidingSpeed(ANTPlusPacket* packet);
	virtual float getSpeed(float perimeter);
	virtual bool isSameDataValue(ANTPlusDataRiding* newData);
};

#endif