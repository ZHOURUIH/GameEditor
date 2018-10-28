#ifndef _ANT_PLUS_PACKET_HEART_BEAT_H_
#define _ANT_PLUS_PACKET_HEART_BEAT_H_

#include "ANTPlusPacket.h"

class ANTPlusDataHeartBeat;
class ANTPlusPacketHeartBeat : public ANTPlusPacket
{
public:
	ANTPlusDataHeartBeat* mDataHeartBeat;				// Êý¾ÝÇø
public:
	ANTPlusPacketHeartBeat(ANT_PLUS_PACKET_TYPE type);
protected:
	virtual void checkANTPlusData();
};

#endif