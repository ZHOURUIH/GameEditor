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
	static void generateCppPacketDeclareFile(const PacketInfo& packetInfo, string csFilePath, string scFilePath);
	static void generateStringDefinePacket(const myVector<string>& packetList, string filePath);
	static void generateCppCSPacketFile(const string& packetName, string filePath);
	static void generateCppSCPacketFile(const string& packetName, string filePath);
	//c#
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpDecalreFile(const PacketInfo& packetInfo, string csFilePath, string scFilePath);
	static void generateCSharpSCPacketFile(const string& packetName, string filePath);
protected:
};

#endif