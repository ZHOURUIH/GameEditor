#include "Utility.h"
#include "CodeGenerator.h"

void main()
{
	string cppHeaderPath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/Socket/PacketHeader";
	string cppPacketDefinePath = "E:/Github/MicroLegend_Server/MicroLegend_Server/Game/Socket";
	string csHeaderPath = "E:/Github/MicroLegend/Assets/Scripts/Game/Socket/PacketHeader";
	string csPacketDefinePath = "E:/Github/MicroLegend/Assets/Scripts/Game/Socket";
	CodeGenerator::generatePacketCode(cppHeaderPath, csHeaderPath, cppPacketDefinePath, csPacketDefinePath);
	system("pause");
}