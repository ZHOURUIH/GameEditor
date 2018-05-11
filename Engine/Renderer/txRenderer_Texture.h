#ifndef _TX_RENDERER_TEXTURE_H_
#define _TX_RENDERER_TEXTURE_H_

#include "txRendererInstance.h"

class txTexture;
class txRendererTexture : public txRendererInstance
{
public:
	txRendererTexture(const std::string& vertName, const std::string& fragName)
		:
		txRendererInstance(vertName, fragName)
	{}
	void init();
	// count为索引的数量
	int render(txTexture* tex, txTexture* maskTex, const MATRIX4& MVPMatrix, const GLfloat* Vertices, const GLfloat* TexCoord, 
		const GLushort* Indics, const GLsizei& count, const float& alpha, const VECTOR3& colour, const BLEND_MODE& mode, const VECTOR3& hslOffset, const float& fArc, const bool& linearDodge,
		const int& uWrap = GL_CLAMP_TO_EDGE, const int& vWrap = GL_CLAMP_TO_EDGE, const bool& useMipmap = true);
protected:
	GLuint texvPositionHandle;
	GLuint texvTexCoordHandle;
	GLuint texMVPMatrixHandle;
	GLuint texSampler;
	GLuint texSamplerMask;
	GLuint texHasMask;
	GLuint texAlpha;
	GLuint texColour;
	GLuint texBlendMode;
	GLuint texHSLOffset;
	GLuint texArc;
	GLuint texLinearDodge;
};

#endif