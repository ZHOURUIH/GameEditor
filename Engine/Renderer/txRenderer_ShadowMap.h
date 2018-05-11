#ifndef _TX_RENDERER_SHADOWMAP_H_
#define _TX_RENDERER_SHADOWMAP_H_

#include "txRendererInstance.h"

class txMaterial;
class txRendererShadowMap : public txRendererInstance
{
public:
	txRendererShadowMap(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	virtual void init();
	int render(txMaterial* material, const MATRIX4& MVPMatrix, const GLfloat* Vertices, const GLfloat *TexCoord, const GLushort *Indics, const GLsizei& count);
protected:
	GLuint shadowPositionHandle;
	GLuint shadowTexCoordHandle;
	GLuint shadowMVPMatrixHandle;
	GLuint shadowSamplerHandle;
};

#endif