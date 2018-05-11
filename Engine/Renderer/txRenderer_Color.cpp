#include "txRenderer_Color.h"
#include "Utility.h"

void txRendererColor::init()
{
	getAttributeHandle(CvPositionHandle, "vPosition");
	getUniformHandle(CColor, "uColor");
	getUniformHandle(CMVPMatrixHandle, "uMVPMatrix");
	getUniformHandle(CAlpha, "fAlpha");
}

int txRendererColor::render(const VECTOR4& color, const MATRIX4& MVPMatrix, const GLfloat *Vertices, const GLushort *Indics, GLsizei count, const float& alpha, const POLYGON_MODE& polygon)
{
	glEnable(GL_CULL_FACE);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_DITHER);
	glCullFace(GL_BACK);

	glUseProgram(mProgram);
	checkGlError("glUseProgram");

	//set uniform
	glUniform4fv(CColor, 1, txMath::toPtr(color));
	glUniformMatrix4fv(CMVPMatrixHandle, 1, false, txMath::toPtr(MVPMatrix));
	glUniform1f(CAlpha, alpha);

	//set attribute
	glVertexAttribPointer(CvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, Vertices);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(CvPositionHandle);
	checkGlError("glEnableVertexAttribArray");

	glDrawElements(polygon, count, GL_UNSIGNED_SHORT, Indics);
	glDisableVertexAttribArray(CvPositionHandle);
	return 0;
}
