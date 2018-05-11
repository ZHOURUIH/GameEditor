#ifndef _TX_RENDERER_COLOR_H_
#define _TX_RENDERER_COLOR_H_

#include "txRendererInstance.h"

class txRendererColor : public txRendererInstance
{
public:
	txRendererColor(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	virtual void init();
	int render(const VECTOR4& color, const MATRIX4& MVPMatrix, const GLfloat *Vertices, const GLushort *Indics, GLsizei count, const float& alpha = 1.0f, const POLYGON_MODE& polygon = PM_LINES);
protected:
	GLuint CvPositionHandle;
	GLuint CMVPMatrixHandle;
	GLuint CColor;
	GLuint CAlpha;
};

#endif