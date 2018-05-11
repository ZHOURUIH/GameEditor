#include "CommandHeader.h"
#include "USBManager.h"

void CommandUSBManagerCloseDevice::execute()
{
	USBManager* usbManager = static_cast<USBManager*>(mReceiver);
	usbManager->closeDevice();
}

std::string CommandUSBManagerCloseDevice::showDebugInfo()
{
	COMMAND_DEBUG(DEBUG_EMPTY);
}