#ifndef _EC_RENDER_WINDOW_COORDINATE_H_
#define _EC_RENDER_WINDOW_COORDINATE_H_

#include "txEngineDefine.h"

class ECRenderWindowCoordinate
{
public:
	ECRenderWindowCoordinate();
	virtual ~ECRenderWindowCoordinate(){ destroy(); }
	virtual void init();
	virtual void render();
	void destroy();
	const bool& isVisible() { return mVisible; }
	void setVisible(const bool& visible) { mVisible = visible; }
	const VECTOR4& getColour() { return mColour; }
	void setColour(const VECTOR4& colour) { mColour = colour; }
protected:
	void initCoordinate();
protected:
	VECTOR4 mColour;
	bool mVisible;
	GLfloat* mVertices;
	GLushort* mIndices;
	int mIndexCount;
	int mVertexCount;
};

#endif 