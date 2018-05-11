#include "txRenderer_UI.h"
#include "txRenderer_Water.h"

int txRendererUI::render(txTexture* tex, txTexture* maskTex, const MATRIX4& MVPMat, const GLfloat* vertices, const GLushort* indices,
	const GLfloat* texCoords, const float& alpha, const VECTOR3& colour, const BLEND_MODE& mode, const VECTOR3& hslOffset, const SYMMETRY_MODE& symmetryMode, const bool& useWater, 
	const float& time, const VECTOR2& resolution, const float& fArc, const bool& linearDodge, const bool& useMipmap)
{
	int uWrap = GL_CLAMP_TO_EDGE;
	int vWrap = GL_CLAMP_TO_EDGE;

	// 纹理寻址方式
	// U方向上
	if (symmetryMode == SM_LEFT_COPY_TO_RIGHT)
	{
		uWrap = GL_REPEAT;
	}
	else if (symmetryMode == SM_LEFT_MIRROR_TO_RIGHT || symmetryMode == SM_HORI_VERT_MIRROR)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		uWrap = GL_MIRRORED_REPEAT_ARB;
#endif
	}
	// V方向上
	if (symmetryMode == SM_BOTTOM_COPY_TO_TOP)
	{
		vWrap = GL_REPEAT;
	}
	else if (symmetryMode == SM_HORI_VERT_MIRROR || symmetryMode == SM_BOTTOM_MIRROR_TO_TOP)
	{
#if RUN_PLATFORM == PLATFORM_WINDOWS
		vWrap = GL_MIRRORED_REPEAT_ARB;
#endif
	}
	
	if (!useWater)
	{
		txRendererTexture::render(tex, maskTex, MVPMat, vertices, texCoords, indices, UI_INDEX_COUNT, alpha, colour, mode, hslOffset, fArc, linearDodge, uWrap, vWrap, useMipmap);
	}
	else
	{
		mWaterRender->render(tex, MVPMat, vertices, indices, UI_INDEX_COUNT, time, resolution);
	}
	return 0;
}