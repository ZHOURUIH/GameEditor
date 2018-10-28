#ifndef _ANT_PLUS_PACKET_RIDING_SPEED_H_
#define _ANT_PLUS_PACKET_RIDING_SPEED_H_

#include "ANTPlusPacket.h"

class ANTPlusDataRidingSpeed;
class ANTPlusPacketRidingSpeed : public ANTPlusPacket
{
public:
	ANTPlusDataRidingSpeed* mDataRidingSpeed;				// Êý¾ÝÇø
public:
	ANTPlusPacketRidingSpeed(ANT_PLUS_PACKET_TYPE type);
protected:
	virtual void checkANTPlusData();
};

#endif