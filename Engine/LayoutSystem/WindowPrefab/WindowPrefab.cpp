#include "WindowHeader.h"
#include "WindowProperty.h"

void WindowPrefab::savePrefab(std::string& stream)
{
	if (mPrefabName != EMPTY_STRING)
	{
		ENGINE_ERROR("error : prefab has linked a prefab!save prefab will be empty!");
		return;
	}
	txStringUtility::jsonStartArray(stream, 0, true);
	saveWindow(stream);
	txStringUtility::jsonEndArray(stream, 0, true);
}