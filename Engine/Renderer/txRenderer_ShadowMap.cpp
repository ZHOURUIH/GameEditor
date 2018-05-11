#include "txRenderer_ShadowMap.h"
#include "txMaterial.h"
#include "txTextureManager.h"
#include "txTexture.h"
#include "Utility.h"

void txRendererShadowMap::init()
{
	getAttributeHandle(shadowPositionHandle, "vPosition");
	getAttributeHandle(shadowTexCoordHandle, "vTexCoord");
	getUniformHandle(shadowMVPMatrixHandle, "uMVPMatrix");
	getUniformHandle(shadowSamplerHandle, "uSampler");
}

int txRendererShadowMap::render(txMaterial* material, const MATRIX4& MVPMatrix, const GLfloat* Vertices, const GLfloat* TexCoord, const GLushort* Indics, const GLsizei& count)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glUseProgram(mProgram);
	checkGlError("glUseProgram  shadowProgram");

	//set uniform
	glUniformMatrix4fv(shadowMVPMatrixHandle, 1, false, txMath::toPtr(MVPMatrix));

	//set attribute
	glVertexAttribPointer(shadowPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(shadowPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	glVertexAttribPointer(shadowTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, TexCoord);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(shadowTexCoordHandle);
	checkGlError("glEnableVertexAttribArray");

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
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glUniform1i(shadowSamplerHandle, 0);

	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, Indics);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(shadowPositionHandle);
	glDisableVertexAttribArray(shadowTexCoordHandle);
	return 0;
}
