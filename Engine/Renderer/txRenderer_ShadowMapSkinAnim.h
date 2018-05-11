#ifndef _TX_RENDERER_SHADOWMAP_SKINANIM_H_
#define _TX_RENDERER_SHADOWMAP_SKINANIM_H_

#include "txRendererInstance.h"

class txMaterial;
class txRendererShadowMapSkinAnim : public txRendererInstance
{
public:
	txRendererShadowMapSkinAnim(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	virtual void init();
	int render(txMaterial* material, const MATRIX4& MVMatrix, const MATRIX4& MVPMatrix, const MATRIX4* transMatrixs, const int& nMatrixs, const MATRIX4& ShadowMapMVPMatrix,
		const GLuint& shadowMapID, const GLfloat* Vertices, const GLfloat* TexCoords, const GLfloat* SkinIndics,
		const GLfloat* SkinWeights, const GLushort* Indics, const GLsizei& count);
protected:
	GLuint skinMeshShadowvPosition;		//vertex attributes
	GLuint skinMeshShadowvTexCoord;
	GLuint skinMeshShadowvSkinIndex;
	GLuint skinMeshShadowvSkinWeight;
	GLuint skinMeshShadowMVPMatrix;	 	//Uniform value
	GLuint skinMeshShadowDepthMVPMatrix;
	GLuint skinMeshShadowMVMatrix;
	GLuint skinMeshShadowNearD;
	GLuint skinMeshShadowFarD;
	GLuint skinMeshShadowTransMatrix;
	GLuint skinMeshShadowFogColor;
	GLuint skinMeshShadowSampler;
	GLuint skinMeshShadowDepthSampler;
	VECTOR3 skinMeshShadowfogColor;	//fog parameters
};

#endif