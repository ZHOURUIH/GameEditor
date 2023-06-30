#pragma once

#include "SystemUtility.h"

class FrameUtility : public SystemUtility
{
public:
	template<typename T>
	static bool arrayContains(T* list, uint count, const T& value)
	{
		if (list == NULL)
		{
			return false;
		}
		FOR_I(count)
		{
			if (list[i] == value)
			{
				return true;
			}
		}
		return false;
	}
};