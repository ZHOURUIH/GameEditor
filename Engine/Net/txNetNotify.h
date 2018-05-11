#ifndef	__txNetNotify_H__
#define	__txNetNotify_H__

class txNetPacket;

class txNetNotify
{
public:
	virtual ~txNetNotify(void)
	{
	}

public:
	virtual void onDisconnected() = 0;
	virtual void onConnected() = 0;
	virtual void onRecvPacket(txNetPacket *pPack) = 0;
};

#endif
