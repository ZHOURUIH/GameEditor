#ifndef _ANT_PLUS_PACKET_RIDING_CADENCE_SPEED_H_
#define _ANT_PLUS_PACKET_RIDING_CADENCE_SPEED_H_

#include "ANTPlusPacket.h"

class ANTPlusDataRidingCadenceSpeed;
class ANTPlusPacketRidingCadenceSpeed : public ANTPlusPacket
{
public:
	ANTPlusDataRidingCadenceSpeed* mDataRidingCadenceSpeed;				// Êý¾ÝÇø
public:
	ANTPlusPacketRidingCadenceSpeed(ANT_PLUS_PACKET_TYPE type);
protected:
	virtual void checkANTPlusData();
};

#endif