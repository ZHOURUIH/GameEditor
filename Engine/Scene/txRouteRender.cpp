#include "txRendererManager.h"
#include "txCamera.h"
#include "txScene.h"
#include "txRouteRender.h"

txRouteRender::txRouteRender(txScene* scene)
:
mVisible(true),
mParentScene(scene)
{}

txRouteRender::~txRouteRender()
{
	destroy();
}

void txRouteRender::init(txVector<VECTOR3>& pointList)
{
	mRoutePointList.clear();
	int pointCount = pointList.size();
	FOR_STL(pointList, int i = 0; i < pointCount; ++i)
	{
		ROUTE_POINT point;
		point.mStart = pointList[i];
		mParentScene->getHeight(point.mStart.x, point.mStart.z, point.mStart.y);
		point.mStart.y += 0.1f;

		point.mEnd = pointList[(i + 1) % pointCount];
		mParentScene->getHeight(point.mEnd.x, point.mEnd.z, point.mEnd.y);
		point.mEnd.y += 0.1f;

		point.mVertices[0] = point.mStart.x;
		point.mVertices[1] = point.mStart.y;
		point.mVertices[2] = point.mStart.z;
		point.mVertices[3] = point.mEnd.x;
		point.mVertices[4] = point.mEnd.y;
		point.mVertices[5] = point.mEnd.z;
		point.mIndices[0] = 0;
		point.mIndices[1] = 1;
		mRoutePointList.push_back(point);
	}
	END_FOR_STL(pointList);
}

void txRouteRender::render(txCamera* camera)
{
	if (mVisible)
	{
		int pointCount = mRoutePointList.size();
		FOR_STL(mRoutePointList, int i = 0; i < pointCount; ++i)
		{
			txRendererInstance::mColourRender->render(VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), camera->getVPMatrix(),
				mRoutePointList[i].mVertices, mRoutePointList[i].mIndices, 2, 1.0f);
		}
		END_FOR_STL(mRoutePointList);
	}
}