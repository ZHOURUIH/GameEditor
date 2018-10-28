#ifndef _ANT_PLUS_PACKET_RIDING_CADENCE_H_
#define _ANT_PLUS_PACKET_RIDING_CADENCE_H_

#include "ANTPlusPacket.h"

class ANTPlusDataRidingCadence;
class ANTPlusPacketRidingCadence : public ANTPlusPacket
{
public:
	ANTPlusDataRidingCadence* mDataRidingCadence;				// Êý¾ÝÇø
public:
	ANTPlusPacketRidingCadence(ANT_PLUS_PACKET_TYPE type);
protected:
	virtual void checkANTPlusData();
	virtual void writeReceiveLog();
	virtual void writeUpdateLog();
};

#endif