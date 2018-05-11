#ifndef _TX_RENDERER_LIGHT_H_
#define _TX_RENDERER_LIGHT_H_

#include "txRendererInstance.h"

class txMaterial;
class txDirLight;
class txRendererLight : public txRendererInstance
{
public:
	txRendererLight(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	virtual ~txRendererLight(){}
	virtual void init();
	int render(txMaterial* material, txDirLight* light, const VECTOR3& viewPos, const MATRIX4& MVMatrix, const MATRIX4& MVPMatrix, const GLfloat* Vertices, const GLfloat* Normals,
		const GLfloat* TexCoords, const GLushort* Indics, const GLsizei& count, const float& alpha);
protected:
	GLuint lightvPositionHandle;
	GLuint lightvVertNormalHandle;
	GLuint lightvTexCoordHandle;
	GLuint lightMVMatrixHandle;
	GLuint lightMVPMatrixHandle;
	GLuint lightNearDHandle;
	GLuint lightFarDHandle;
	GLuint lightFogColorHandle;
	GLuint lightDirHandle;
	GLuint lightViewPosHandle;
	GLuint lightuNsHandle;
	GLuint lightuKsHandle;
	GLuint lightuKdHandle;
	GLuint lightuKaHandle;
	GLuint lightUIncH;
	GLuint lightVIncH;
	GLuint lightuLightCorHandle;
	GLuint lightSamplerH;
	GLuint lightAlphaSampler;
	GLuint lightAlpha;
	GLuint lightHasAlphaTexture;
};

#endif