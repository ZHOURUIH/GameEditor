#include "txRenderUtility.h"
#include "txRendererManager.h"
#include "txCamera.h"

GLfloat* txRenderUtility::mBoxVertices = NULL;
short txRenderUtility::mBoxVertexCount = 0;
GLushort* txRenderUtility::mBoxIndices = NULL;
short txRenderUtility::mBoxIndexCount = 0;
GLushort* txRenderUtility::mBoxLineIndices = NULL;
short txRenderUtility::mBoxLineIndexCount = 0;

GLfloat* txRenderUtility::mPlaneVertices = NULL;
short txRenderUtility::mPlaneVertexCount = 0;
GLushort* txRenderUtility::mPlaneIndices = NULL;
short txRenderUtility::mPlaneIndexCount = 0;
GLushort* txRenderUtility::mPlaneLineIndices = NULL;
short txRenderUtility::mPlaneLineIndexCount = 0;

GLfloat* txRenderUtility::mSphereVertices = NULL;
short txRenderUtility::mSphereVertexCount = 0;
GLushort* txRenderUtility::mSphereIndices = NULL;
short txRenderUtility::mSphereIndexCount = 0;
GLushort* txRenderUtility::mSphereLineIndices = NULL;
short txRenderUtility::mSphereLineIndexCount = 0;

GLfloat* txRenderUtility::mCylinderVertices = NULL;
short txRenderUtility::mCylinderVertexCount = 0;
GLushort* txRenderUtility::mCylinderIndices = NULL;
short txRenderUtility::mCylinderIndexCount = 0;
GLushort* txRenderUtility::mCylinderLineIndices = NULL;
short txRenderUtility::mCylinderLineIndexCount = 0;

txRenderUtility::txRenderUtility()
{
	initBoxVertex();
	initPlaneVertex();
	initSphereVertex();
	initCylinderVertex();
}

txRenderUtility::~txRenderUtility()
{
	TRACE_DELETE_ARRAY(mBoxVertices);
	TRACE_DELETE_ARRAY(mBoxIndices);
	TRACE_DELETE_ARRAY(mBoxLineIndices);
	TRACE_DELETE_ARRAY(mPlaneVertices);
	TRACE_DELETE_ARRAY(mPlaneIndices);
	TRACE_DELETE_ARRAY(mPlaneLineIndices);
	TRACE_DELETE_ARRAY(mSphereVertices);
	TRACE_DELETE_ARRAY(mSphereIndices);
	TRACE_DELETE_ARRAY(mSphereLineIndices);
	TRACE_DELETE_ARRAY(mCylinderVertices);
	TRACE_DELETE_ARRAY(mCylinderIndices);
	TRACE_DELETE_ARRAY(mCylinderLineIndices);
}

void txRenderUtility::renderBox(txCamera* camera, const float& x, const float& y, const float& z, const MATRIX4& transMat, const VECTOR4& colour, const bool& wireframe)
{
	MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, VECTOR3(x, y, z));
	MATRIX4 finalTransMat = transMat * scaleMat;
	POLYGON_MODE polygon;
	int indexCount = 0;
	GLushort* indices = NULL;
	if (wireframe)
	{
		indexCount = mBoxLineIndexCount;
		indices = mBoxLineIndices;
		polygon = PM_LINES;
	}
	else
	{
		indexCount = mBoxIndexCount;
		indices = mBoxIndices;
		polygon = PM_TRIANGLES;
	}
	txRendererInstance::mColourRender->render(colour, camera->getVPMatrix() * finalTransMat, mBoxVertices, indices, indexCount, 1.0f, polygon);
}

void txRenderUtility::renderPlane(txCamera* camera, const float& x, const float& z, const MATRIX4& transMat, const VECTOR4& colour, const bool& wireframe)
{
	MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, VECTOR3(x, 1.0f, z));
	MATRIX4 finalTransMat = transMat * scaleMat;
	POLYGON_MODE polygon;
	int indexCount = 0;
	GLushort* indices = NULL;
	if (wireframe)
	{
		indexCount = mPlaneLineIndexCount;
		indices = mPlaneLineIndices;
		polygon = PM_LINES;
	}
	else
	{
		indexCount = mPlaneIndexCount;
		indices = mPlaneIndices;
		polygon = PM_TRIANGLES;
	}
	txRendererInstance::mColourRender->render(colour, camera->getVPMatrix() * finalTransMat, mPlaneVertices, indices, indexCount, 1.0f, polygon);
}

void txRenderUtility::renderSphere(txCamera* camera, const float& radius, const MATRIX4& transMat, const VECTOR4& colour, const bool& wireframe)
{
	MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, VECTOR3(radius, radius, radius));
	MATRIX4 finalTransMat = transMat * scaleMat;
	POLYGON_MODE polygon;
	int indexCount = 0;
	GLushort* indices = NULL;
	if (wireframe)
	{
		indexCount = mSphereLineIndexCount;
		indices = mSphereLineIndices;
		polygon = PM_LINES;
	}
	else
	{
		indexCount = mSphereIndexCount;
		indices = mSphereIndices;
		polygon = PM_TRIANGLES;
	}
	txRendererInstance::mColourRender->render(colour, camera->getVPMatrix() * finalTransMat, mSphereVertices, indices, indexCount, 1.0f, polygon);
}

void txRenderUtility::renderCapsule(txCamera* camera, const float& capsuleLength, const float& radius, const MATRIX4& transMat, const VECTOR4& colour, const bool& wireframe)
{
	float cylinderLength = capsuleLength - 2.0f * radius;
	POLYGON_MODE polygon;
	int sphereIndexCount = 0;
	GLushort* sphereIndices = NULL;
	int cylinderIndexCount = 0;
	GLushort* cylinderIndices = NULL;
	if (wireframe)
	{
		sphereIndexCount = mSphereLineIndexCount / 2;
		sphereIndices = mSphereLineIndices;
		cylinderIndexCount = mCylinderLineIndexCount;
		cylinderIndices = mCylinderLineIndices;
		polygon = PM_LINES;
	}
	else
	{
		sphereIndexCount = mSphereIndexCount / 2;
		sphereIndices = mSphereIndices;
		cylinderIndexCount = mCylinderIndexCount;
		cylinderIndices = mCylinderIndices;
		polygon = PM_TRIANGLES;
	}
	// 前半球
	MATRIX4 scaleSphereMat = txMath::scale(txMath::MAT4_IDENTITY, VECTOR3(radius, radius, radius));
	MATRIX3 leftRot = txMath::getPitchMatrix3(txMath::MATH_PI / 2.0f);
	MATRIX4 leftSphereTranslateMat = txMath::translate(txMath::MAT4_IDENTITY, VECTOR3(0.0f, 0.0f, cylinderLength / 2.0f));
	MATRIX4 leftHalfSphereTransMat = transMat * leftSphereTranslateMat * scaleSphereMat * MATRIX4(leftRot);
	txRendererInstance::mColourRender->render(colour, camera->getVPMatrix() * leftHalfSphereTransMat, mSphereVertices, sphereIndices, sphereIndexCount, 1.0f, polygon);
	// 圆柱体
	MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, VECTOR3(radius, radius, cylinderLength));
	MATRIX3 cylinderRot = txMath::getPitchMatrix3(txMath::MATH_PI / 2.0f);
	MATRIX4 cylinderTransMat = transMat * scaleMat * MATRIX4(cylinderRot);
	txRendererInstance::mColourRender->render(colour, camera->getVPMatrix() * cylinderTransMat, mCylinderVertices, cylinderIndices, cylinderIndexCount, 1.0f, polygon);
	// 后半球
	MATRIX4 rightHalfSphereTranslateMat = txMath::translate(txMath::MAT4_IDENTITY, VECTOR3(0.0f, 0.0f, -cylinderLength / 2.0f));
	MATRIX3 rightRot = txMath::getPitchMatrix3(-txMath::MATH_PI / 2.0f);
	MATRIX4 rightHalfSphereTransMat = transMat * rightHalfSphereTranslateMat * scaleSphereMat * MATRIX4(rightRot);
	txRendererInstance::mColourRender->render(colour, camera->getVPMatrix() * rightHalfSphereTransMat, mSphereVertices, sphereIndices, sphereIndexCount, 1.0f, polygon);
}

void txRenderUtility::renderCylinder(txCamera* camera, const float& height, const float& radius, const MATRIX4& transMat, const VECTOR4& colour, const bool& wireframe)
{
	MATRIX3 cylinderRot = txMath::getPitchMatrix3(txMath::MATH_PI / 2.0f);
	MATRIX4 scaleMat = txMath::scale(txMath::MAT4_IDENTITY, VECTOR3(radius, radius, height));
	MATRIX4 finalTransMat = transMat * scaleMat * MATRIX4(cylinderRot);
	POLYGON_MODE polygon;
	int indexCount = 0;
	GLushort* indices = NULL;
	if (wireframe)
	{
		indexCount = mCylinderLineIndexCount;
		indices = mCylinderLineIndices;
		polygon = PM_LINES;
	}
	else
	{
		indexCount = mCylinderIndexCount;
		indices = mCylinderIndices;
		polygon = PM_TRIANGLES;
	}
	txRendererInstance::mColourRender->render(colour, camera->getVPMatrix() * finalTransMat, mCylinderVertices, indices, indexCount, 1.0f, polygon);
}

void txRenderUtility::initBoxVertex()
{
	mBoxVertexCount = 8;
	mBoxVertices = TRACE_NEW_ARRAY(GLfloat, mBoxVertexCount * 3, mBoxVertices);
	float defaultX = 1.0f, defaultY = 1.0f, defaultZ = 1.0f;
	// 上底面的4个顶点,俯视的顺时针的顺序
	short vertexIndex = 0;
	mBoxVertices[vertexIndex * 3 + 0] = -defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = -defaultZ / 2.0f;
	++vertexIndex;

	mBoxVertices[vertexIndex * 3 + 0] = -defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = defaultZ / 2.0f;
	++vertexIndex;

	mBoxVertices[vertexIndex * 3 + 0] = defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = defaultZ / 2.0f;
	++vertexIndex;

	mBoxVertices[vertexIndex * 3 + 0] = defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = -defaultZ / 2.0f;
	++vertexIndex;

	// 下底面的4个顶点
	mBoxVertices[vertexIndex * 3 + 0] = -defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = -defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = -defaultZ / 2.0f;
	++vertexIndex;

	mBoxVertices[vertexIndex * 3 + 0] = -defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = -defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = defaultZ / 2.0f;
	++vertexIndex;

	mBoxVertices[vertexIndex * 3 + 0] = defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = -defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = defaultZ / 2.0f;
	++vertexIndex;

	mBoxVertices[vertexIndex * 3 + 0] = defaultX / 2.0f;
	mBoxVertices[vertexIndex * 3 + 1] = -defaultY / 2.0f;
	mBoxVertices[vertexIndex * 3 + 2] = -defaultZ / 2.0f;
	++vertexIndex;

	if (vertexIndex != mBoxVertexCount)
	{
		ENGINE_ERROR("error : init box vertices error!");
	}

	const short triangleCount = 12;
	mBoxIndexCount = triangleCount * 3;
	mBoxIndices = TRACE_NEW_ARRAY(GLushort, mBoxIndexCount, mBoxIndices);

	short triangleIndex = 0;
	// Y轴正方向的两个三角形
	mBoxIndices[triangleIndex * 3 + 0] = 2;
	mBoxIndices[triangleIndex * 3 + 1] = 1;
	mBoxIndices[triangleIndex * 3 + 2] = 0;
	++triangleIndex;
	mBoxIndices[triangleIndex * 3 + 0] = 3;
	mBoxIndices[triangleIndex * 3 + 1] = 2;
	mBoxIndices[triangleIndex * 3 + 2] = 0;
	++triangleIndex;
	// Y轴负方向的两个三角形
	mBoxIndices[triangleIndex * 3 + 0] = 4;
	mBoxIndices[triangleIndex * 3 + 1] = 5;
	mBoxIndices[triangleIndex * 3 + 2] = 6;
	++triangleIndex;
	mBoxIndices[triangleIndex * 3 + 0] = 4;
	mBoxIndices[triangleIndex * 3 + 1] = 6;
	mBoxIndices[triangleIndex * 3 + 2] = 7;
	++triangleIndex;
	// Z轴正方向的两个三角形
	mBoxIndices[triangleIndex * 3 + 0] = 2;
	mBoxIndices[triangleIndex * 3 + 1] = 6;
	mBoxIndices[triangleIndex * 3 + 2] = 5;
	++triangleIndex;
	mBoxIndices[triangleIndex * 3 + 0] = 2;
	mBoxIndices[triangleIndex * 3 + 1] = 5;
	mBoxIndices[triangleIndex * 3 + 2] = 1;
	++triangleIndex;
	// Z轴负方向的两个三角形
	mBoxIndices[triangleIndex * 3 + 0] = 3;
	mBoxIndices[triangleIndex * 3 + 1] = 0;
	mBoxIndices[triangleIndex * 3 + 2] = 4;
	++triangleIndex;
	mBoxIndices[triangleIndex * 3 + 0] = 3;
	mBoxIndices[triangleIndex * 3 + 1] = 4;
	mBoxIndices[triangleIndex * 3 + 2] = 7;
	++triangleIndex;
	// X轴正方向的两个三角形
	mBoxIndices[triangleIndex * 3 + 0] = 2;
	mBoxIndices[triangleIndex * 3 + 1] = 3;
	mBoxIndices[triangleIndex * 3 + 2] = 7;
	++triangleIndex;
	mBoxIndices[triangleIndex * 3 + 0] = 2;
	mBoxIndices[triangleIndex * 3 + 1] = 7;
	mBoxIndices[triangleIndex * 3 + 2] = 6;
	++triangleIndex;
	// X轴负方向的两个三角形
	mBoxIndices[triangleIndex * 3 + 0] = 0;
	mBoxIndices[triangleIndex * 3 + 1] = 1;
	mBoxIndices[triangleIndex * 3 + 2] = 5;
	++triangleIndex;
	mBoxIndices[triangleIndex * 3 + 0] = 0;
	mBoxIndices[triangleIndex * 3 + 1] = 5;
	mBoxIndices[triangleIndex * 3 + 2] = 4;
	++triangleIndex;

	if (triangleIndex * 3 != mBoxIndexCount)
	{
		ENGINE_ERROR("error : init box indices error!");
	}

	// 根据三角形渲染的顶点索引生成用于线框渲染的顶点索引
	generateLineIndices(triangleCount, mBoxIndices, mBoxLineIndices, mBoxLineIndexCount);
}

void txRenderUtility::initPlaneVertex()
{
	mPlaneVertexCount = 4;
	mPlaneVertices = TRACE_NEW_ARRAY(GLfloat, mPlaneVertexCount * 3, mPlaneVertices);
	float defaultX = 1.0f, defaultZ = 1.0f;
	short vertexIndex = 0;
	mPlaneVertices[vertexIndex * 3 + 0] = -defaultX / 2.0f;
	mPlaneVertices[vertexIndex * 3 + 1] = 0.0f;
	mPlaneVertices[vertexIndex * 3 + 2] = -defaultZ / 2.0f;
	++vertexIndex;

	mPlaneVertices[vertexIndex * 3 + 0] = -defaultX / 2.0f;
	mPlaneVertices[vertexIndex * 3 + 1] = 0.0f;
	mPlaneVertices[vertexIndex * 3 + 2] = defaultZ / 2.0f;
	++vertexIndex;

	mPlaneVertices[vertexIndex * 3 + 0] = defaultX / 2.0f;
	mPlaneVertices[vertexIndex * 3 + 1] = 0.0f;
	mPlaneVertices[vertexIndex * 3 + 2] = defaultZ / 2.0f;
	++vertexIndex;

	mPlaneVertices[vertexIndex * 3 + 0] = defaultX / 2.0f;
	mPlaneVertices[vertexIndex * 3 + 1] = 0.0f;
	mPlaneVertices[vertexIndex * 3 + 2] = -defaultZ / 2.0f;
	++vertexIndex;

	if (vertexIndex != mPlaneVertexCount)
	{
		ENGINE_ERROR("error : init plane vertices error!");
	}

	const short triangleCount = 2;
	mPlaneIndexCount = triangleCount * 3;
	mPlaneIndices = TRACE_NEW_ARRAY(GLushort, mPlaneIndexCount, mPlaneIndices);
	short triangleIndex = 0;
	mPlaneIndices[triangleIndex * 3 + 0] = 2;
	mPlaneIndices[triangleIndex * 3 + 1] = 3;
	mPlaneIndices[triangleIndex * 3 + 2] = 0;
	++triangleIndex;

	mPlaneIndices[triangleIndex * 3 + 0] = 1;
	mPlaneIndices[triangleIndex * 3 + 1] = 2;
	mPlaneIndices[triangleIndex * 3 + 2] = 0;
	++triangleIndex;

	if (triangleIndex * 3 != mPlaneIndexCount)
	{
		ENGINE_ERROR("error : init plane indices error!");
	}

	// 根据三角形渲染的顶点索引生成用于线框渲染的顶点索引
	generateLineIndices(triangleCount, mPlaneIndices, mPlaneLineIndices, mPlaneLineIndexCount);
}

void txRenderUtility::initSphereVertex()
{
	const short slideCount = 20;
	const short rectsPerSlide = 20;
	mSphereVertexCount = (slideCount + 1) * rectsPerSlide;
	mSphereVertices = TRACE_NEW_ARRAY(GLfloat, mSphereVertexCount * 3, mSphereVertices);
	float defaultRadius = 1.0f;
	float angle = txMath::MATH_PI * 2.0f / rectsPerSlide;
	// 从Z轴负方向开始计算,顺时针
	for (short i = 0; i < slideCount + 1; ++i)
	{
		// 当前层到球心的距离,低于球心则小于0,高于球心则大于0
		float curHeight = (float)i / slideCount * defaultRadius * 2.0f - defaultRadius;
		float curRadius = std::sqrt(defaultRadius * defaultRadius - curHeight * curHeight);
		for (short j = 0; j < rectsPerSlide; ++j)
		{
			short vertexIndex = i * rectsPerSlide + j;
			mSphereVertices[vertexIndex * 3 + 0] = std::sin(angle * j) * curRadius;
			mSphereVertices[vertexIndex * 3 + 1] = curHeight;
			mSphereVertices[vertexIndex * 3 + 2] = std::cos(angle * j) * curRadius;
		}
	}

	// 创建用于三角形渲染的顶点索引
	const short triangleCount = rectsPerSlide * 2 * slideCount;
	mSphereIndexCount = triangleCount * 3;
	mSphereIndices = TRACE_NEW_ARRAY(GLushort, mSphereIndexCount, mSphereIndices);
	for (short i = 0; i < slideCount; ++i)
	{
		for (short j = 0; j < rectsPerSlide; ++j)
		{
			short triangleIndex = i * rectsPerSlide * 2 + j * 2 + 0;
			mSphereIndices[triangleIndex * 3 + 0] = i * rectsPerSlide + j;
			mSphereIndices[triangleIndex * 3 + 1] = (i + 1) * rectsPerSlide + j;
			mSphereIndices[triangleIndex * 3 + 2] = (i + 1) * rectsPerSlide + (j + 1) % rectsPerSlide;

			++triangleIndex;
			mSphereIndices[triangleIndex * 3 + 0] = i * rectsPerSlide + j;
			mSphereIndices[triangleIndex * 3 + 1] = (i + 1) * rectsPerSlide + (j + 1) % rectsPerSlide;
			mSphereIndices[triangleIndex * 3 + 2] = i * rectsPerSlide + (j + 1) % rectsPerSlide;
		}
	}

	// 根据三角形渲染的顶点索引生成用于线框渲染的顶点索引
	generateLineIndices(triangleCount, mSphereIndices, mSphereLineIndices, mSphereLineIndexCount);
}

void txRenderUtility::initCylinderVertex()
{
	const short slideCount = 20;
	const short rectsPerSlide = 20;
	// 两个底面的中心各有一个顶点
	mCylinderVertexCount = (slideCount + 1) * rectsPerSlide + 2;
	mCylinderVertices = TRACE_NEW_ARRAY(GLfloat, mCylinderVertexCount * 3, mCylinderVertices);
	float defaultHeight = 1.0f, defaultRadius = 1.0f;
	float angle = txMath::MATH_PI * 2.0f / rectsPerSlide;
	// 从Z轴负方向开始计算,顺时针
	for (short i = 0; i < slideCount + 1; ++i)
	{
		float cueHeight = (float)i / slideCount * defaultHeight - defaultHeight / 2.0f;
		for (short j = 0; j < rectsPerSlide; ++j)
		{
			short vertexIndex = i * rectsPerSlide + j;
			mCylinderVertices[vertexIndex * 3 + 0] = std::sin(angle * j) * defaultRadius;
			mCylinderVertices[vertexIndex * 3 + 1] = cueHeight;
			mCylinderVertices[vertexIndex * 3 + 2] = std::cos(angle * j) * defaultRadius;
		}
	}
	// 上下底面中心的顶点
	short topVertexIndex = (slideCount + 1) * rectsPerSlide;
	mCylinderVertices[topVertexIndex * 3 + 0] = 0.0f;
	mCylinderVertices[topVertexIndex * 3 + 1] = defaultHeight / 2.0f;
	mCylinderVertices[topVertexIndex * 3 + 2] = 0.0f;
	short bottomVertexIndex = topVertexIndex + 1;
	mCylinderVertices[bottomVertexIndex * 3 + 0] = 0.0f;
	mCylinderVertices[bottomVertexIndex * 3 + 1] = -defaultHeight / 2.0f;
	mCylinderVertices[bottomVertexIndex * 3 + 2] = 0.0f;

	// 还要加上上下底面的三角形
	const short triangleCount = rectsPerSlide * 2 * slideCount + rectsPerSlide * 2;
	mCylinderIndexCount = triangleCount * 3;
	mCylinderIndices = TRACE_NEW_ARRAY(GLushort, mCylinderIndexCount, mCylinderIndices);
	for (short i = 0; i < slideCount; ++i)
	{
		for (short j = 0; j < rectsPerSlide; ++j)
		{
			short triangleIndex = i * rectsPerSlide * 2 + j * 2 + 0;
			mCylinderIndices[triangleIndex * 3 + 0] = i * rectsPerSlide + j;
			mCylinderIndices[triangleIndex * 3 + 1] = (i + 1) * rectsPerSlide + j;
			mCylinderIndices[triangleIndex * 3 + 2] = (i + 1) * rectsPerSlide + (j + 1) % rectsPerSlide;

			++triangleIndex;
			mCylinderIndices[triangleIndex * 3 + 0] = i * rectsPerSlide + j;
			mCylinderIndices[triangleIndex * 3 + 1] = (i + 1) * rectsPerSlide + (j + 1) % rectsPerSlide;
			mCylinderIndices[triangleIndex * 3 + 2] = i * rectsPerSlide + j + 1;
		}
	}

	short startTriangleIndex = slideCount * rectsPerSlide * 2;
	// 上底面的三角形
	for (short i = 0; i < rectsPerSlide; ++i)
	{
		short curTriangleIndex = startTriangleIndex + i;
		mCylinderIndices[curTriangleIndex * 3 + 0] = slideCount * rectsPerSlide + i;
		mCylinderIndices[curTriangleIndex * 3 + 1] = slideCount * rectsPerSlide + (i + 1) % rectsPerSlide;
		mCylinderIndices[curTriangleIndex * 3 + 2] = topVertexIndex;
	}

	startTriangleIndex += rectsPerSlide;
	// 下底面的三角形
	for (short i = 0; i < rectsPerSlide; ++i)
	{
		short curTriangleIndex = startTriangleIndex + i;
		mCylinderIndices[curTriangleIndex * 3 + 0] = i;
		mCylinderIndices[curTriangleIndex * 3 + 1] = (i + 1) % rectsPerSlide;
		mCylinderIndices[curTriangleIndex * 3 + 2] = bottomVertexIndex;
	}

	// 根据三角形渲染的顶点索引生成用于线框渲染的顶点索引
	generateLineIndices(triangleCount, mCylinderIndices, mCylinderLineIndices, mCylinderLineIndexCount);
}

void txRenderUtility::generateLineIndices(const short& triangleCount, GLushort* triangleIndices, GLushort*& lineIndices, short& lineIndexCount)
{
	lineIndexCount = triangleCount * 3 * 2;
	lineIndices = TRACE_NEW_ARRAY(GLushort, lineIndexCount, lineIndices);
	for (short i = 0; i < triangleCount; ++i)
	{
		// 三角形的三个点的索引
		GLushort point0 = triangleIndices[i * 3 + 0];
		GLushort point1 = triangleIndices[i * 3 + 1];
		GLushort point2 = triangleIndices[i * 3 + 2];

		// 拆成3条线
		lineIndices[i * 3 * 2 + 0] = point0;
		lineIndices[i * 3 * 2 + 1] = point1;
		lineIndices[i * 3 * 2 + 2] = point1;
		lineIndices[i * 3 * 2 + 3] = point2;
		lineIndices[i * 3 * 2 + 4] = point2;
		lineIndices[i * 3 * 2 + 5] = point0;
	}
}