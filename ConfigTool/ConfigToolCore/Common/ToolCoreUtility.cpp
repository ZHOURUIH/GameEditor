#include "ToolCoreUtility.h"
#include "ConfigToolCore.h"

void ToolCoreUtility::logError(std::string info)
{
	ConfigToolCore::getSingletonPtr()->logError(info);
}

void ToolCoreUtility::logInfo(std::string info)
{
	ConfigToolCore::getSingletonPtr()->logInfo(info);
}

// 硬件值转换为踏频(踏频单位为次/分钟)
float ToolCoreUtility::HWSToStepFrequency(float haradwareValue, float speedRatio)
{
	if (txUtility::isFloatZero(haradwareValue))
	{
		return 0.0f;
	}
	if (haradwareValue >= 450.0f)
	{
		haradwareValue = 450.0f - 1;
	}
	return 1.0f / ((450.0f - haradwareValue) * 5.0f / 1000.0f) * 60.0f / speedRatio;
}

float ToolCoreUtility::stepFrequencyToHWS(float frequency, float speedRatio)
{
	if (frequency <= 0.0f)
	{
		return 0.0f;
	}
	return 450.0f - 1.0f / (frequency * speedRatio / 60.0f) * 1000.0f / 5.0f;
}

float ToolCoreUtility::stepFrequencyToMS(float frequency)
{
	if (frequency <= 0.0f)
	{
		return 0.0f;
	}
	float speed = frequency / 60.0f * 4.0f;
	txUtility::clamp(speed, 0.0f, 20.0f);
	// 假设每踩踏一圈,前进4米
	return speed;
}

float ToolCoreUtility::MSToStepFrequency(float speedMS)
{
	if (speedMS < 0.0f)
	{
		return 0.0f;
	}
	return speedMS / 4.0f * 60.0f;
}

float ToolCoreUtility::MSToHWS(float speed, float speedRatio)
{
	return stepFrequencyToHWS(MSToStepFrequency(speed), speedRatio);
}

float ToolCoreUtility::HWSToMS(float hwsSpeed, float speedRatio)
{
	return stepFrequencyToMS(HWSToStepFrequency(hwsSpeed, speedRatio));
}