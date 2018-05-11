#ifndef _TX_RENDERER_SKYBOX_H_
#define _TX_RENDERER_SKYBOX_H_

#include "txRendererInstance.h"

class txMaterial;
class txRendererSkybox : public txRendererInstance
{
public:
	txRendererSkybox(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	virtual ~txRendererSkybox(){}
	void init();
	int render(txMaterial* material, const MATRIX4& VPMatrix, const GLfloat* Vertices, const GLfloat* TexCoords, const GLushort* Indics, const GLsizei& count);
protected:
	GLuint skyBoxvPositionHandle;
	GLuint skyBoxvTexCoordHandle;
	GLuint skyBoxVPMatrixHandle;
	GLuint skyBoxFogColorHandle;
	GLuint skyBoxFogCoefHandle;
	GLuint skyBoxSamplerH;
};

#endif