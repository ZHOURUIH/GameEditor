#include "txRenderer_ShadowMapSkinAnim.h"
#include "txDirLight.h"
#include "txMaterial.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "txTexture.h"

void txRendererShadowMapSkinAnim::init()
{
	getAttributeHandle(skinMeshShadowvPosition, "vPosition");
	getAttributeHandle(skinMeshShadowvTexCoord, "vTexCoord");
	getAttributeHandle(skinMeshShadowvSkinIndex, "Bone");
	getAttributeHandle(skinMeshShadowvSkinWeight, "Weight");
	getUniformHandle(skinMeshShadowMVPMatrix, "uMVPMatrix");
	getUniformHandle(skinMeshShadowDepthMVPMatrix, "uShadowMVPMatrix");
	getUniformHandle(skinMeshShadowMVMatrix, "uMVMatrix");
	getUniformHandle(skinMeshShadowNearD, "uNearD");
	getUniformHandle(skinMeshShadowFarD, "uFarD");
	getUniformHandle(skinMeshShadowTransMatrix, "boneMatrices");
	getUniformHandle(skinMeshShadowFogColor, "uFogColor");
	getUniformHandle(skinMeshShadowSampler, "uSampler");
	getUniformHandle(skinMeshShadowDepthSampler, "uShadowSampler");
}

int txRendererShadowMapSkinAnim::render(txMaterial* material, const MATRIX4& MVMatrix, const MATRIX4& MVPMatrix,
	const MATRIX4* transMatrixs, const int& nMatrixs, const MATRIX4& ShadowMapMVPMatrix,
	const GLuint& shadowMapID, const GLfloat* Vertices, const GLfloat* TexCoords, const GLfloat* SkinIndics,
	const GLfloat* SkinWeights, const GLushort* Indics, const GLsizei& count)
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
	glUniformMatrix4fv(skinMeshShadowMVPMatrix, 1, false, txMath::toPtr(MVPMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(skinMeshShadowDepthMVPMatrix, 1, false, txMath::toPtr(ShadowMapMVPMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(skinMeshShadowMVMatrix, 1, false, txMath::toPtr(MVMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(skinMeshShadowTransMatrix, nMatrixs, false, txMath::toPtr(transMatrixs[0]));
	checkGlError("glUniformMatrix4fv");

	glUniform1f(skinMeshShadowNearD, mSceneManager->getSceneFogNearDis());
	checkGlError("glUniform1f");

	glUniform1f(skinMeshShadowFarD, mSceneManager->getSceneFogFarDis());
	checkGlError("glUniform1f");

	glUniform3fv(skinMeshShadowFogColor, 1, txMath::toPtr(mSceneManager->getSceneFogColor()));
	checkGlError("glUniform3fv");

	glVertexAttribPointer(skinMeshShadowvPosition, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinMeshShadowvPosition);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(skinMeshShadowvTexCoord, 2, GL_FLOAT, GL_FALSE, 0, TexCoords);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinMeshShadowvTexCoord);
	checkGlError("glEnableVertexAttribArray");

	//skinIndics
	glVertexAttribPointer(skinMeshShadowvSkinIndex, 4, GL_FLOAT, GL_FALSE, 0, SkinIndics);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinMeshShadowvSkinIndex);
	checkGlError("glEnableVertexAttribArray");

	//skinWeights
	glVertexAttribPointer(skinMeshShadowvSkinWeight, 4, GL_FLOAT, GL_FALSE, 0, SkinWeights);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(skinMeshShadowvSkinWeight);
	checkGlError("glEnableVertexAttribArray");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);
	glUniform1i(skinMeshShadowDepthSampler, 0);

	glActiveTexture(GL_TEXTURE1);
	txTexture* tex = mTextureManager->getTexture(material->getDiffuseTexture());
	int texID = 0;
	if(tex != NULL)
	{
		texID = tex->getTextureID();
	}
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(skinMeshShadowSampler, 1);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(skinMeshShadowvPosition);
	glDisableVertexAttribArray(skinMeshShadowvTexCoord);
	glDisableVertexAttribArray(skinMeshShadowvSkinIndex);
	glDisableVertexAttribArray(skinMeshShadowvSkinWeight);
	return 0;
}
