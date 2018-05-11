#include "txRenderer_Light.h"
#include "txDirLight.h"
#include "txMaterial.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "txEngineRoot.h"
#include "txTexture.h"

void txRendererLight::init()
{
	getAttributeHandle(lightvPositionHandle, "vPosition");
	getAttributeHandle(lightvVertNormalHandle, "vNormal");
	getAttributeHandle(lightvTexCoordHandle, "vTexCoord");
	getUniformHandle(lightMVPMatrixHandle, "uMVPMatrix");
	getUniformHandle(lightMVMatrixHandle, "uMVMatrix");
	getUniformHandle(lightNearDHandle, "uNearD");
	getUniformHandle(lightFarDHandle, "uFarD");
	getUniformHandle(lightFogColorHandle, "uFogColor");
	getUniformHandle(lightDirHandle, "uLightDirection");
	getUniformHandle(lightViewPosHandle, "uViewPosition");
	getUniformHandle(lightuNsHandle, "uNs");
	getUniformHandle(lightuKsHandle, "uKs");
	getUniformHandle(lightuKdHandle, "uKd");
	getUniformHandle(lightuKaHandle, "uKa");
	getUniformHandle(lightuLightCorHandle, "uLightColor");
	getUniformHandle(lightSamplerH, "uSampler");
	getUniformHandle(lightAlphaSampler, "alphaSampler");
	getUniformHandle(lightUIncH, "uInc");
	getUniformHandle(lightVIncH, "vInc");
	getUniformHandle(lightAlpha, "fAlpha");
	getUniformHandle(lightHasAlphaTexture, "hasAlphaTexture");
}

int txRendererLight::render(txMaterial* material, txDirLight* light, const VECTOR3& viewPos, const MATRIX4& MVMatrix, const MATRIX4& MVPMatrix, const GLfloat* Vertices,
										const GLfloat* Normals, const GLfloat* TexCoords, const GLushort* Indics, const GLsizei& count, const float& alpha)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDepthFunc(GL_LESS);

	glDepthMask(GL_TRUE);

	glUseProgram(mProgram);
	checkGlError("glUseProgram");

	//set uniform
	glUniformMatrix4fv(lightMVPMatrixHandle, 1, false, txMath::toPtr(MVPMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(lightMVMatrixHandle, 1, false, txMath::toPtr(MVMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniform1f(lightNearDHandle, mSceneManager->getSceneFogNearDis());
	checkGlError("glUniform1f");

	glUniform1f(lightFarDHandle, mSceneManager->getSceneFogFarDis());
	checkGlError("glUniform1f");

	glUniform1f(lightAlpha, alpha);
	checkGlError("glUniform1f");

	glUniform3fv(lightFogColorHandle, 1, txMath::toPtr(mSceneManager->getSceneFogColor()));
	checkGlError("glUniform3fv");

	//set attribute
	glVertexAttribPointer(lightvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(lightvPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(lightvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, TexCoords);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(lightvTexCoordHandle);
	checkGlError("glEnableVertexAttribArray");

	txTexture* tex = mTextureManager->getTexture(material->getDiffuseTexture());
	int texID = 0;
	int alphaTexID = 0;
	if (tex != NULL)
	{
		texID = tex->getTextureID();
		alphaTexID = tex->getAlphaTextureID();
	}

	//set texture
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glUniform1f(lightUIncH, material->getTextureOffsetU());
	checkGlError("glUniform1f");

	glUniform1f(lightVIncH, material->getTextureOffsetV());
	checkGlError("glUniform1f");

	glUniform1i(lightSamplerH, 0);

	// 如果存在透明纹理,则激活第1层纹理
	if (alphaTexID > 0)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, alphaTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 透明纹理只能用linear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glUniform1i(lightAlphaSampler, 1);
	}

	// 通知shader是否需要在透明纹理上采集透明度
	glUniform1i(lightHasAlphaTexture, alphaTexID > 0 ? 1 : 0);
	checkGlError("glUniform1i");

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(lightvPositionHandle);
	glDisableVertexAttribArray(lightvTexCoordHandle);
	return 0;
}
