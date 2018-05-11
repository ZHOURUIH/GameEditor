#include "WindowComponentMoveRelative.h"
#include "WindowHeader.h"
#include "txEngineLog.h"
#include "Utility.h"

void WindowComponentMoveRelative::applyKeyFrame(const txVector<float>& value)
{
	int valueCount = value.size();
	if (valueCount != 4)
	{
		ENGINE_ERROR("window move relative needs 4 dimensions KeyFrame! cur is : %d", valueCount);
		return;
	}
	txPoint posOffset(txDim(value[0], (int)value[1]), txDim(value[2], (int)value[3]));
	static_cast<Window*>(mComponentOwner)->setPosition(mStartPos + posOffset);
}