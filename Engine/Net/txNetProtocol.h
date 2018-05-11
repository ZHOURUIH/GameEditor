#ifndef __txNetProtocol_H__
#define __txNetProtocol_H__

#include "txEngineDefine.h"

#define PACK_HEADER_SIZE	5											///每个包的包头长度5个字节
#define	MAX_CONTENTS_SIZE	2000										///每个包的内容2000个字节
#define TOTAL_PACKET_SIZE	PACK_HEADER_SIZE + MAX_CONTENTS_SIZE
#define HEADER_INDEX				0									///header	包头索引
#define COMMAND_INDEX				1									///command	功能号索引
#define LENGTH_INDEX				3									///length	数据长度索引
#define CONTENTS_INDEX			5									///contents	内容索引
#define	HEADER_SIZE				1
#define COMMAND_SIZE				2
#define LENGTH_SIZE				2
#define CONTENTS_SIZE				2000

#pragma pack(push)
#pragma pack(1)
class txNetPacket
{
public:

	txNetPacket() :
			header(0x55), command(0), length(0)
	{
		memset(contents, 0, MAX_CONTENTS_SIZE);
	}
public:
	unsigned char header; ///包头			1字节 0x55
	unsigned short command; ///功能号			2字节
	unsigned short length; ///数据长度		2字节
	unsigned char contents[MAX_CONTENTS_SIZE]; ///内容			2000字节
};
#pragma pack(pop)

#endif
