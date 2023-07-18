#ifndef _CODE_NET_PACKET_H_
#define _CODE_NET_PACKET_H_

#include "CodeUtility.h"

class CodeNetPacket : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateServer(const myVector<string>& gamePacketNameList, const myVector<PacketInfo>& packetInfoList, const myVector<PacketStruct>& structInfoList);
	static void generateClient(const myVector<string>& gamePacketNameList, const myVector<PacketInfo>& packetInfoList, const myVector<PacketStruct>& structInfoList);
	static void generateServerGamePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath);
	static void generateClientGamePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath);
	static void generateServerGamePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion);
	static void generateClientGamePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion);
	static void generateCppCSPacketFileHeader(const PacketInfo& packetInfo, const string& filePath);
	static void generateCppCSPacketFileSource(const PacketInfo& packetInfo, const string& filePath);
	static void generateCppSCPacketFileHeader(const PacketInfo& packetInfo, const string& filePath);
	static int findPacketVersion(const string& filePath);
	static void generateCppPacketMemberDeclare(const myVector<PacketMember>& memberList, myVector<string>& generateCodes);
	static void generateCppPacketReadWrite(const PacketInfo& packetInfo, myVector<string>& generateCodes);
	static void generateCppStruct(const PacketStruct& structInfo, const string& filePath);
};

#endif