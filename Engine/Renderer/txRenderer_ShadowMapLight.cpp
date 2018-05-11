#include "txRenderer_ShadowMapLight.h"
#include "txDirLight.h"
#include "txMaterial.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "txTexture.h"

void txRendererShadowMapLight::init()
{
	getAttributeHandle(staticMeshShadowvPosition, "vPosition");
	getAttributeHandle(staticMeshShadowvVertNormal, "vNormal");
	getAttributeHandle(staticMeshShadowvTexCoord, "vTexCoord");
	getUniformHandle(staticMeshShadowMVPMatrix, "uMVPMatrix");
	getUniformHandle(staticMeshShadowDepthMVPMatrix, "uShadowMVPMatrix");
	getUniformHandle(staticMeshShadowMVMatrix, "uMVMatrix");
	getUniformHandle(staticMeshShadowNearD, "uNearD");
	getUniformHandle(staticMeshShadowFarD, "uFarD");
	getUniformHandle(staticMeshShadowFogColor, "uFogColor");
	getUniformHandle(staticMeshShadowDir, "uLightDirection");
	getUniformHandle(staticMeshShadowViewPos, "uViewPosition");
	getUniformHandle(staticMeshShadowuNs, "uNs");
	getUniformHandle(staticMeshShadowuKs, "uKs");
	getUniformHandle(staticMeshShadowuKd, "uKd");
	getUniformHandle(staticMeshShadowuKa, "uKa");
	getUniformHandle(staticMeshShadowuLightCor, "uLightColor");
	getUniformHandle(staticMeshShadowSampler, "uSampler");
	getUniformHandle(staticMeshShadowDepthSampler, "uShadowSampler");
	getUniformHandle(staticMeshShadowUInc, "uInc");
	getUniformHandle(staticMeshShadowVInc, "vInc");
}

int txRendererShadowMapLight::render(txMaterial* material, txDirLight* light, const VECTOR3& viewPos, const MATRIX4& MVMatrix, 
	const MATRIX4& MVPMatrix, const MATRIX4& ShadowMapMVPMatrix, const GLuint& shadowMapID,const GLfloat *Vertices, const GLfloat *Normals, 
	const GLfloat *TexCoords, const GLushort *Indics, const GLsizei& count)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glUseProgram(mProgram);
	checkGlError("staticMeshShadowProgram glUseProgram");

	//set uniform
	glUniformMatrix4fv(staticMeshShadowMVPMatrix, 1, false, txMath::toPtr(MVPMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(staticMeshShadowDepthMVPMatrix, 1, false, txMath::toPtr(ShadowMapMVPMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniformMatrix4fv(staticMeshShadowMVMatrix, 1, false, txMath::toPtr(MVMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniform1f(staticMeshShadowNearD, mSceneManager->getSceneFogNearDis());
	checkGlError("glUniform1f");

	glUniform1f(staticMeshShadowFarD, mSceneManager->getSceneFogFarDis());
	checkGlError("glUniform1f");

	glUniform3fv(staticMeshShadowFogColor, 1, txMath::toPtr(mSceneManager->getSceneFogColor()));
	checkGlError("glUniform3fv");
	glVertexAttribPointer(staticMeshShadowvPosition, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(staticMeshShadowvPosition);
	checkGlError("glEnableVertexAttribArray");
	glVertexAttribPointer(staticMeshShadowvTexCoord, 2, GL_FLOAT, GL_FALSE, 0, TexCoords);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(staticMeshShadowvTexCoord);
	checkGlError("glEnableVertexAttribArray");

	glActiveTexture(GL_TEXTURE0);
	txTexture* tex = mTextureManager->getTexture(material->getDiffuseTexture());
	int texID = 0;
	if(tex != NULL)
	{
		texID = tex->getTextureID();
	}
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glUniform1i(staticMeshShadowSampler, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glUniform1i(staticMeshShadowDepthSampler, 1);

	glUniform1f(staticMeshShadowUInc, material->getTextureOffsetU());
	checkGlError("glUniform1f");

	glUniform1f(staticMeshShadowVInc, material->getTextureOffsetV());
	checkGlError("glUniform1f");

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(staticMeshShadowvPosition);
	glDisableVertexAttribArray(staticMeshShadowvTexCoord);
	return 0;
}
