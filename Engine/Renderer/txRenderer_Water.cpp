#include "txRenderer_Water.h"
#include "txTexture.h"
#include "Utility.h"

void txRendererWater::init()
{
	getAttributeHandle(mPositionHandle, "vPosition");
	getUniformHandle(mMVPMatrixHandle, "uMVPMatrix");
	getUniformHandle(mTimeHandle, "time");
	getUniformHandle(mResolutionHandle, "resolution");
	getUniformHandle(mSamplerH, "tex0");
}

void txRendererWater::render(txTexture* texture, const MATRIX4& MVPMatrix, const GLfloat* Vertices,
	const GLushort* Indics, const GLsizei& count, const float& time, const VECTOR2& resolution)
{
	if (NULL == texture)
	{
		return;
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(mProgram);
	checkGlError("glUseProgram");

	//set uniform
	glUniformMatrix4fv(mMVPMatrixHandle, 1, false, txMath::toPtr(MVPMatrix));
	glUniform1f(mTimeHandle, time);
	glUniform2fv(mResolutionHandle, 1, txMath::toPtr(resolution));

	//set attribute
	glVertexAttribPointer(mPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(mPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	//set texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glUniform1i(mSamplerH, 0);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(mPositionHandle);
	glDisable(GL_BLEND);
}