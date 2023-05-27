#ifndef _CODE_NET_PACKET_H_
#define _CODE_NET_PACKET_H_

#include "CodeUtility.h"

class CodeNetPacket : public CodeUtility
{
public:
	static void generate();
protected:
	//c++
	static void generateCppGamePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath);
	static void generateCppGameCorePacketDefineFile(const myVector<PacketInfo>& packetList, const string& filePath);
	static void generateCppGamePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion);
	static void generateCppGameCorePacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath);
	static void generateStringDefinePacket(const myVector<string>& packetList, const string& stringDefineFile);
	static void generateCppCSPacketFileHeader(const PacketInfo& packetInfo, const string& filePath);
	static void generateCppCSPacketFileSource(const PacketInfo& packetInfo, const string& filePath);
	static void generateCppSCPacketFileHeader(const PacketInfo& packetInfo, const string& filePath);
	static int findPacketVersion(const string& filePath);
	static void findCppIncludeCustomCode(const string& packetName, const string& fullPath, myVector<string>& includeList, myVector<string>& customList);
	//c#
	static void generateCSharpPacketDefineFile(const myVector<PacketInfo>& gamePacketList, const myVector<PacketInfo>& gameCorePacketList, const string& filePath);
	static void generateCSharpPacketRegisteFile(const myVector<PacketInfo>& packetList, const string& filePath, int packetVersion);
	static void generateCSharpPacketFile(const PacketInfo& packetInfo, const string& csFileHotfixPath, const string& csFileGamePath, const string& scFileHotfixPath, const string& scFileGamePath);
	static void findCSharpUsingListCustomCode(const string& packetName, const string& fullPath, myVector<string>& usingList, myVector<string>& customList);
};

#endif