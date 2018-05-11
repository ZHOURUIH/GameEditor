#include "CommandHeader.h"
#include "USBManager.h"

void CommandUSBManagerOpenDevice::execute()
{
	USBManager* usbManager = static_cast<USBManager*>(mReceiver);
	usbManager->openDevice();
}

std::string CommandUSBManagerOpenDevice::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}