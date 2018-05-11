#ifndef _TX_ROUTE_RENDER_H_
#define _TX_ROUTE_RENDER_H_

#include "txEngineDefine.h"

struct ROUTE_POINT
{
	VECTOR3 mStart;
	VECTOR3 mEnd;
	GLfloat mVertices[6];
	GLushort mIndices[2];
};

class txScene;
class txRouteRender
{
public:
	txRouteRender(txScene* scene);
	virtual ~txRouteRender();
	void init(txVector<VECTOR3>& pointList);
	void render(txCamera* camera);
	void destroy(){}
	const bool& isVisible() { return mVisible; }
	void setVisible(const bool& visible) { mVisible = visible; }
protected:
	bool mVisible;
	txVector<ROUTE_POINT> mRoutePointList;
	txScene* mParentScene;
};

#endif