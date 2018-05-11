#ifndef _TX_RENDERER_UI_H_
#define _TX_RENDERER_UI_H_

#include "txRenderer_Texture.h"

class txRendererUI : public txRendererTexture
{
public:
	txRendererUI(const std::string& vertName, const std::string& fragName)
		:
		txRendererTexture(vertName, fragName)
	{}
	virtual ~txRendererUI(){}
	int render(txTexture* tex, txTexture* maskTex, const MATRIX4& MVPMat, const GLfloat* vertices, const GLushort* indices,
		const GLfloat* texCoords, const float& alpha, const VECTOR3& colour, const BLEND_MODE& mode, const VECTOR3& hslOffset, const SYMMETRY_MODE& symmetryMode, const bool& useWater, 
		const float& time, const VECTOR2& resolution, const float& fArc, const bool& linearDodge, const bool& useMipmap = true);
protected:
};

#endif