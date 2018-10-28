#ifndef _ANT_PLUS_PACKET_RIDING_HEART_RATE_H_
#define _ANT_PLUS_PACKET_RIDING_HEART_RATE_H_

#include "ANTPlusPacket.h"

class ANTPlusDataRidingHeartRate;
class ANTPlusPacketRidingHeartRate : public ANTPlusPacket
{
public:
	ANTPlusDataRidingHeartRate* mDataRidingHeartRate;				// Êý¾ÝÇø
public:
	ANTPlusPacketRidingHeartRate(ANT_PLUS_PACKET_TYPE type);
protected:
	virtual void checkANTPlusData();
	virtual void writeReceiveLog();
	virtual void writeUpdateLog();
};

#endif