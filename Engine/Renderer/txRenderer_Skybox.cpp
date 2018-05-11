#include "txRenderer_Skybox.h"
#include "txMaterial.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "txTexture.h"

void txRendererSkybox::init()
{
	getAttributeHandle(skyBoxvPositionHandle, "vPosition");
	getAttributeHandle(skyBoxvTexCoordHandle, "vTexCoord");
	getUniformHandle(skyBoxVPMatrixHandle, "uVPMatrix");
	getUniformHandle(skyBoxFogColorHandle, "uFogColor");
	getUniformHandle(skyBoxFogCoefHandle, "fFogCoef");
	getUniformHandle(skyBoxSamplerH, "uSampler");
}

int txRendererSkybox::render(txMaterial* material, const MATRIX4& VPMatrix, const GLfloat* Vertices, const GLfloat* TexCoords, const GLushort* Indics, const GLsizei& count)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glUseProgram(mProgram);
	checkGlError("glUseProgram");

	//set uniform
	glUniformMatrix4fv(skyBoxVPMatrixHandle, 1, false, txMath::toPtr(VPMatrix));
	checkGlError("glUniformMatrix4fv");

	glUniform3fv(skyBoxFogColorHandle, 1, txMath::toPtr(mSceneManager->getSceneFogColor()));
	checkGlError("glUniform3fv");

	glUniform1f(skyBoxFogCoefHandle, mSceneManager->getSkyFogCoef());
	checkGlError("glUniform1f");

	//set attribute
	glVertexAttribPointer(skyBoxvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer 1");

	glEnableVertexAttribArray(skyBoxvPositionHandle);
	checkGlError("glEnableVertexAttribArray 1");

	glVertexAttribPointer(skyBoxvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, TexCoords);
	checkGlError("glVertexAttribPointer 2");

	glEnableVertexAttribArray(skyBoxvTexCoordHandle);
	checkGlError("glEnableVertexAttribArray 2");

	//set texture
	glActiveTexture(GL_TEXTURE0 + 0);
	txTexture* tex = mTextureManager->getTexture(material->getDiffuseTexture());
	int texID = 0;
	if(tex != NULL)
	{
		texID = tex->getTextureID();
	}
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glUniform1i(skyBoxSamplerH, 0);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(skyBoxvPositionHandle);
	glDisableVertexAttribArray(skyBoxvTexCoordHandle);
	return 0;
}