#ifndef _CODE_NET_PACKET_H_
#define _CODE_NET_PACKET_H_

#include "CodeUtility.h"

class CodeNetPacket : public CodeUtility
{
public:
	static void generatePacketCode(string cppCSDeclareFilePath, string cppSCDeclareFilePath, string csharpCSFilePath, string csharpSCFilePath, string cppPacketDefineFilePath, string csPacketDefineFilePath, string cppStringDefinePath);
protected:
	//c++
	static void generateCppPacketTotalHeaderFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCppPacketDeclareFile(const PacketInfo& packetName, string csFilePath, string scFilePath);
	static void generateStringDefinePacket(const myVector<string>& packetList, string filePath);
	//c#
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpFile(const PacketInfo& packetName, string csFilePath, string scFilePath);
};

#endif