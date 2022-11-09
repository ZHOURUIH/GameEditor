#include "CSPacketMove.h"
#include "SCPacketOtherMove.h"
#include "UDPServer.h"

void CSPacketMove::read(char* buffer, int bufferSize, int& index)
{
	base::read(buffer, bufferSize, index);
	mPlayerID = Utility::read<int>(buffer, bufferSize, index);
	mPosX = Utility::read<float>(buffer, bufferSize, index);
	mPosY = Utility::read<float>(buffer, bufferSize, index);
}

void CSPacketMove::execute(llong token)
{
	auto* otherMovePacket = static_cast<SCPacketOtherMove*>(UDPServer::createPacket(PACKET_TYPE::SC_OTHER_MOVE));
	otherMovePacket->mPlayerID = mPlayerID;
	otherMovePacket->mPosX = mPosX;
	otherMovePacket->mPosY = mPosY;
	// 给其他玩家广播消息
	UDPServer::get()->broadcastPacket(otherMovePacket, token);
	delete otherMovePacket;
}