#ifndef _ANT_PLUS_DATA_RIDING_CADENCE_H_
#define _ANT_PLUS_DATA_RIDING_CADENCE_H_

#include "ANTPlusDataRiding.h"

class ANTPlusPacket;
class ANTPlusDataRidingCadence : public ANTPlusDataRiding
{
public:
	ANTPlusDataRidingCadence(ANTPlusPacket* packet);
	virtual int getCadence();
	virtual bool isSameDataValue(ANTPlusDataRiding* newData);
};

#endif