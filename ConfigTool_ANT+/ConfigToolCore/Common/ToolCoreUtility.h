#ifndef _TOOL_CORE_UTILITY_H_
#define _TOOL_CORE_UTILITY_H_

#include "Utility.h"

class ToolCoreUtility
{
public:
	static void logError(std::string info);
	static void logInfo(std::string info);
	static float HWSToStepFrequency(float haradwareValue, float speedRatio);
	static float stepFrequencyToHWS(float frequency, float speedRatio);
	static float stepFrequencyToMS(float frequency);
	static float MSToStepFrequency(float speedMS);
	static float MSToHWS(float speed, float speedRatio);
	static float HWSToMS(float hwsSpeed, float speedRatio);
	static int toDeviceID(char* idBytes);
	static std::string deviceTypeToName(char deviceType);
};

#endif