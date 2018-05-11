#ifndef _TX_RENDERER_WATER_H_
#define _TX_RENDERER_WATER_H_

#include "txRendererInstance.h"

class txRendererWater : public txRendererInstance
{
public:
	txRendererWater(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	void init();
	// count为索引的数量
	void render(txTexture* texture, const MATRIX4& MVPMatrix, const GLfloat* Vertices,
		const GLushort* Indics, const GLsizei& count, const float& time, const VECTOR2& resolution);
protected:
	GLuint mPositionHandle;
	GLuint mMVPMatrixHandle;
	GLuint mSamplerH;
	GLuint mTimeHandle;
	GLuint mResolutionHandle;
};

#endif