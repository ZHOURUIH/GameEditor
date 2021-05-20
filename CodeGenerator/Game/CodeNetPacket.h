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
	static void generateCppPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath, int packetVersion);
	static void generateStringDefinePacket(const myVector<string>& packetList, string filePath);
	static void generateCppCSPacketFileHeader(const PacketInfo& packetInfo, string filePath);
	static void generateCppCSPacketFileSource(const PacketInfo& packetInfo, string filePath);
	static void generateCppSCPacketFile(const PacketInfo& packetInfo, string filePath);
	static void updateOldFormatPackteFile(const PacketInfo& packetInfo, string fullPath);
	static int findPacketVersion(const string& filePath);
	//c#
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& packetList, string filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, string filePath, int packetVersion);
	static void generateCSharpPacketFile(const PacketInfo& packetInfo, const string& csFileHotfixPath, const string& csFileGamePath, const string& scFileHotfixPath, const string& scFileGamePath);
	static void findUsingListCustomCode(const string& packetName, const string& fullPath, myVector<string>& usingList, myVector<string>& customList);
};

#endif