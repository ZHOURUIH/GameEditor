#ifndef _TOOL_CORE_UTILITY_H_
#define _TOOL_CORE_UTILITY_H_

#include <string>

#include "txUtility.h"

class ToolCoreUtility
{
public:
	static void logError(std::string info);
	static void logInfo(std::string info);
	static float HWSToStepFrequency(float haradwareValue, float speedRatio);
	static float stepFrequencyToHWS(float frequency, float speedRatio);
	static float stepFrequencyToMS(float frequency);
	float MSToStepFrequency(float speedMS);
	float MSToHWS(float speed, float speedRatio);
	float HWSToMS(float hwsSpeed, float speedRatio);
};

#endif