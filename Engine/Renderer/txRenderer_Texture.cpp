#include "txRenderer_Texture.h"
#include "txTexture.h"
#include "Utility.h"

void txRendererTexture::init()
{
	getAttributeHandle(texvPositionHandle, "vPosition");
	getAttributeHandle(texvTexCoordHandle, "vTexCoord");
	getUniformHandle(texMVPMatrixHandle, "uMVPMatrix");
	getUniformHandle(texSampler, "uSampler");
	getUniformHandle(texSamplerMask, "uSamplerMask");
	getUniformHandle(texAlpha, "fAlpha");
	getUniformHandle(texColour, "vColour");
	getUniformHandle(texBlendMode, "blendMode");
	getUniformHandle(texHSLOffset, "vHSLOffset");
	getUniformHandle(texHasMask, "hasMask");
	getUniformHandle(texArc, "fArc");
	getUniformHandle(texLinearDodge, "fLinearDodge");
}

int txRendererTexture::render(txTexture *tex, txTexture* maskTex, const MATRIX4& MVPMatrix, const GLfloat* Vertices, const GLfloat* TexCoord,
	const GLushort* Indics, const GLsizei& count, const float& alpha, const VECTOR3& colour, const BLEND_MODE& mode, const VECTOR3& hslOffset, const float& fArc, const bool& linearDodge,
	const int& uWrap, const int& vWrap, const bool& useMipmap)
{
	if (tex == NULL)
	{
		return 0;
	}
	GLuint texID = tex->getTextureID();
	GLuint maskID = maskTex != NULL ? maskTex->getTextureID() : 0;
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	if (linearDodge)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	else
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glUseProgram(mProgram);
	checkGlError("glUseProgram");

	//set uniform
	glUniformMatrix4fv(texMVPMatrixHandle, 1, false, txMath::toPtr(MVPMatrix));
	glUniform1f(texAlpha, alpha);
	glUniform1f(texArc, fArc);
	glUniform3fv(texColour, 1, txMath::toPtr(colour));
	glUniform1i(texBlendMode, (int)mode);
	glUniform3fv(texHSLOffset, 1, txMath::toPtr(hslOffset));
	glUniform1f(texLinearDodge, linearDodge ? 1.0f : 0.0f);

	//set attribute
	glVertexAttribPointer(texvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(texvPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(texvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, TexCoord);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(texvTexCoordHandle);
	checkGlError("glEnableVertexAttribArray");

	//set texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vWrap);
	glUniform1i(texSampler, 0);

	if (maskID > 0)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, maskID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uWrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vWrap);
		glUniform1i(texSamplerMask, 1);
	}

	// 通知shader是否需要采样第1层纹理
	glUniform1i(texHasMask, maskID > 0 ? 1 : 0);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(texvPositionHandle);
	glDisableVertexAttribArray(texvTexCoordHandle);
	glDisable(GL_BLEND);
	return 0;
}