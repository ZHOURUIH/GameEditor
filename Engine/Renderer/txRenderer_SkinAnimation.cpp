#include "txRenderer_SkinAnimation.h"
#include "txDirLight.h"
#include "txMaterial.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "txTexture.h"

void txRendererSkinAnimation::init()
{
	getAttributeHandle(skinvPositionHandle, "vPosition");
	getAttributeHandle(skinvTexCoordHandle, "vTexCoord");
	getAttributeHandle(skinvSkinIndexHandle, "Bone");
	getAttributeHandle(skinvSkinWeightHandle, "Weight");
	getUniformHandle(skinMVPMatrixHandle, "uMVPMatrix");
	getUniformHandle(skinMVMatrixHandle, "uMVMatrix");
	getUniformHandle(skinNearDHandle, "uNearD");
	getUniformHandle(skinFarDHandle, "uFarD");
	getUniformHandle(skinAlpha, "fAlpha");
	getUniformHandle(skinTransMatrixHandle, "boneMatrices");
	getUniformHandle(skinFogColorHandle, "uFogColor");
	getUniformHandle(skinSamplerH, "uSampler");
	getUniformHandle(skinAlphaSampler, "alphaSampler");
	getUniformHandle(skinHasAlphaTex, "hasAlphaTexture");
}

int txRendererSkinAnimation::render(txMaterial* material, const MATRIX4& MVMatrix, const MATRIX4& MVPMatrix, const MATRIX4* transMatrixs, const int& nMatrixs, const GLfloat* Vertices,
	const GLfloat* TexCoords, const GLfloat* SkinIndics, const GLfloat* SkinWeights, const GLushort* Indics, const GLsizei& count, const float& alpha)
{
	// 由于部分物体在背面裁剪以后会把本应该可见的三角面给裁剪掉,所以暂时不使用背面裁剪
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glUseProgram(mProgram);
	checkGlError("glUseProgram");

	//set uniform
	glUniformMatrix4fv(skinMVPMatrixHandle, 1, false, txMath::toPtr(MVPMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(skinMVMatrixHandle, 1, false, txMath::toPtr(MVMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(skinTransMatrixHandle, nMatrixs, false, txMath::toPtr(transMatrixs[0]));
	checkGlError("glUniformMatrix4fv");

	glUniform1f(skinNearDHandle, mSceneManager->getSceneFogNearDis());
	checkGlError("glUniform1f");

	glUniform1f(skinFarDHandle, mSceneManager->getSceneFogFarDis());
	checkGlError("glUniform1f");

	glUniform1f(skinAlpha, alpha);
	checkGlError("glUniform1f");

	glUniform3fv(skinFogColorHandle, 1, txMath::toPtr(mSceneManager->getSceneFogColor()));
	checkGlError("glUniform3fv");

	//set vertex attribute
	glVertexAttribPointer(skinvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinvPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	//texcoord
	glVertexAttribPointer(skinvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, TexCoords);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinvTexCoordHandle);
	checkGlError("glEnableVertexAttribArray");

	//skinIndics
	glVertexAttribPointer(skinvSkinIndexHandle, 4, GL_FLOAT, GL_FALSE, 0, SkinIndics);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinvSkinIndexHandle);
	checkGlError("glEnableVertexAttribArray");

	//skinWeights
	glVertexAttribPointer(skinvSkinWeightHandle, 4, GL_FLOAT, GL_FALSE, 0, SkinWeights);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinvSkinWeightHandle);
	checkGlError("glEnableVertexAttribArray");

	int texID = 0;
	int alphaTexID = 0;
	if (material != NULL)
	{
		txTexture* tex = mTextureManager->getTexture(material->getDiffuseTexture());
		if (tex != NULL)
		{
			texID = tex->getTextureID();
			alphaTexID = tex->getAlphaTextureID();
		}
	}

	//set texture
	glActiveTexture(GL_TEXTURE0 + 0);

	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glUniform1i(skinSamplerH, 0);

	// 如果存在透明纹理,则激活第1层纹理
	if (alphaTexID > 0)
	{
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, alphaTexID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 透明纹理只能用linear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glUniform1i(skinAlphaSampler, 1);
	}

	// 通知shader是否需要在透明纹理上采集透明度
	glUniform1i(skinHasAlphaTex, alphaTexID > 0 ? 1 : 0);
	checkGlError("glUniform1i");

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(skinvPositionHandle);
	glDisableVertexAttribArray(skinvTexCoordHandle);
	glDisableVertexAttribArray(skinvSkinIndexHandle);
	glDisableVertexAttribArray(skinvSkinWeightHandle);
	return 0;
}