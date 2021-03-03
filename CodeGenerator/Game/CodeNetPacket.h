#ifndef _CODE_NET_PACKET_H_
#define _CODE_NET_PACKET_H_

#include "CodeUtility.h"

class CodeNetPacket : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppPacketTotalHeaderFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateStringDefinePacket(const myVector<string>& packetList, string filePath);
	static void generateCppCSPacketFileHeader(const PacketInfo& packetInfo, string filePath);
	static void generateCppCSPacketFileSource(const PacketInfo& packetInfo, string filePath);
	static void generateCppSCPacketFile(const PacketInfo& packetInfo, string filePath);
	static void updateOldFormatPackteFile(const PacketInfo& packetInfo, string fullPath);
	//c#
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpDecalreFile(const PacketInfo& packetInfo, string csFilePath, string scFilePath);
	static void generateCSharpSCPacketFile(const string& packetName, string filePath);
protected:
};

#endif