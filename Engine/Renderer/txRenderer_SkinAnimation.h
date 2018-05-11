#ifndef _TX_RENDERER_SKIN_ANIMATION_H_
#define _TX_RENDERER_SKIN_ANIMATION_H_

#include "txRendererInstance.h"

class txMaterial;
class txRendererSkinAnimation : public txRendererInstance
{
public:
	txRendererSkinAnimation(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	virtual void init();
	int render(txMaterial* material, const MATRIX4& MVMatrix, const MATRIX4& MVPMatrix, const MATRIX4* transMatrixs, const int& nMatrixs, const GLfloat* Vertices,
		const GLfloat* TexCoords, const GLfloat* SkinIndics, const GLfloat* SkinWeights, const GLushort* Indics, const GLsizei& count, const float& alpha);
protected:
	GLuint skinvPositionHandle;		//vertex attributes
	GLuint skinvTexCoordHandle;
	GLuint skinvSkinIndexHandle;
	GLuint skinvSkinWeightHandle;
	GLuint skinMVPMatrixHandle;	 	//Uniform value
	GLuint skinMVMatrixHandle;
	GLuint skinNearDHandle;
	GLuint skinFarDHandle;
	GLuint skinTransMatrixHandle;
	GLuint skinFogColorHandle;
	GLuint skinSamplerH;
	GLuint skinAlphaSampler;
	GLuint skinHasAlphaTex;
	GLuint skinAlpha;
};

#endif