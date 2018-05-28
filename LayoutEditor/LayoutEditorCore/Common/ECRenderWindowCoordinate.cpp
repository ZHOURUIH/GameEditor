#include "txRendererManager.h"
#include "txEngineRoot.h"

#include "LayoutUtility.h"

#include "ECRenderWindowCoordinate.h"

ECRenderWindowCoordinate::ECRenderWindowCoordinate()
{
	mColour = VECTOR4(1.0f, 0.5f, 0.2f, 1.0f);
	mVisible = true;
}

void ECRenderWindowCoordinate::init()
{
	mIndexCount = 4;
	mVertexCount = 4;
	mVertices = TRACE_NEW_ARRAY(GLfloat, mIndexCount * 3, mVertices);
	mIndices = TRACE_NEW_ARRAY(GLushort, mIndexCount, mIndices);
	initCoordinate();

	GLushort tempIndices[] = { 0, 1, 2, 3 };
	for (int i = 0; i < mIndexCount; ++i)
	{
		mIndices[i] = tempIndices[i];
	}
}

void ECRenderWindowCoordinate::destroy()
{
	TRACE_DELETE(mVertices);
	TRACE_DELETE(mIndices);
}

void  ECRenderWindowCoordinate::render()
{
	if (mVisible)
	{
		txRendererInstance::mColourRender->render(mColour, txMath::MAT4_IDENTITY, mVertices, mIndices, mIndexCount, 1.0f);
	}
}

void  ECRenderWindowCoordinate::initCoordinate()
{
	int vertexIndex = 0;
	mVertices[vertexIndex * 3 + 0] = -1.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = 0.0f;

	++vertexIndex;
	mVertices[vertexIndex * 3 + 0] = 1.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = 0.0f;

	++vertexIndex;
	mVertices[vertexIndex * 3 + 0] = 0.0f;
	mVertices[vertexIndex * 3 + 1] = 1.0f;
	mVertices[vertexIndex * 3 + 2] = 0.0f;

	++vertexIndex;
	mVertices[vertexIndex * 3 + 0] = 0.0f;
	mVertices[vertexIndex * 3 + 1] = -1.0f;
	mVertices[vertexIndex * 3 + 2] = 0.0f;
}