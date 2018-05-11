#ifndef _TX_RENDERER_SHADOWMAP_LIGHT_H_
#define _TX_RENDERER_SHADOWMAP_LIGHT_H_

#include "txRendererInstance.h"

class txMaterial;
class txDirLight;
class txRendererShadowMapLight : public txRendererInstance
{
public:
	txRendererShadowMapLight(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	virtual void init();
	int render(txMaterial* material, txDirLight* light, const VECTOR3& viewPos, const MATRIX4& MVMatrix, const MATRIX4& MVPMatrix, const MATRIX4& ShadowMapMVPMatrix,
		const GLuint& shadowMapID, const GLfloat* Vertices, const GLfloat* Normals, const GLfloat* TexCoords, const GLushort* Indics, const GLsizei& count);
protected:
	GLuint staticMeshShadowvPosition;
	GLuint staticMeshShadowvVertNormal;
	GLuint staticMeshShadowvTexCoord;
	GLuint staticMeshShadowMVMatrix;
	GLuint staticMeshShadowMVPMatrix;
	GLuint staticMeshShadowDepthMVPMatrix;
	GLuint staticMeshShadowNearD;
	GLuint staticMeshShadowFarD;
	GLuint staticMeshShadowFogColor;
	GLuint staticMeshShadowDir;
	GLuint staticMeshShadowViewPos;
	GLuint staticMeshShadowuNs;
	GLuint staticMeshShadowuKs;
	GLuint staticMeshShadowuKd;
	GLuint staticMeshShadowuKa;
	GLuint staticMeshShadowUInc;
	GLuint staticMeshShadowVInc;
	GLuint staticMeshShadowuLightCor;
	GLuint staticMeshShadowSampler;
	GLuint staticMeshShadowDepthSampler;
};

#endif