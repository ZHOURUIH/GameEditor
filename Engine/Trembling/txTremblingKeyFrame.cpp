#include "txTremblingKeyFrame.h"
#include "txTremblingNode.h"

void txTremblingKeyFrame::setTime(const float& time)
{
	if (mParent == NULL || mParent->allowedKeyFrameTime(mIndexInParent, time))
	{
		mTime = time;
	}
}