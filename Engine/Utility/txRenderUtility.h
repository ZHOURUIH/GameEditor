#ifndef _TX_RENDER_UTILITY_H_
#define _TX_RENDER_UTILITY_H_

#include "txEngineDefine.h"

class txCamera;
class txRenderUtility
{
public:
	txRenderUtility();
	virtual ~txRenderUtility();
	static void renderBox(txCamera* camera, const float& x, const float& y, const float& z, const MATRIX4& transMat, const VECTOR4& colour = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), const bool& wireframe = true);
	static void renderPlane(txCamera* camera, const float& x, const float& z, const MATRIX4& transMat, const VECTOR4& colour = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), const bool& wireframe = true);
	static void renderSphere(txCamera* camera, const float& radius, const MATRIX4& transMat, const VECTOR4& colour = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), const bool& wireframe = true);
	static void renderCapsule(txCamera* camera, const float& capsuleLength, const float& radius, const MATRIX4& transMat, const VECTOR4& colour = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), const bool& wireframe = true);
	static void renderCylinder(txCamera* camera, const float& height, const float& radius, const MATRIX4& transMat, const VECTOR4& colour = VECTOR4(1.0f, 0.0f, 0.0f, 1.0f), const bool& wireframe = true);
protected:
	void initBoxVertex();
	void initPlaneVertex();
	void initSphereVertex();
	void initCylinderVertex();
	static void generateLineIndices(const short& triangleCount, GLushort* triangleIndices, GLushort*& lineIndices, short& lineIndexCount);
protected:
	static GLfloat* mBoxVertices;
	static short mBoxVertexCount;
	static GLushort* mBoxIndices;
	static short mBoxIndexCount;
	static GLushort* mBoxLineIndices;
	static short mBoxLineIndexCount;

	static GLfloat* mPlaneVertices;
	static short mPlaneVertexCount;
	static GLushort* mPlaneIndices;
	static short mPlaneIndexCount;
	static GLushort* mPlaneLineIndices;
	static short mPlaneLineIndexCount;

	static GLfloat* mSphereVertices;
	static short mSphereVertexCount;
	static GLushort* mSphereIndices;
	static short mSphereIndexCount;
	static GLushort* mSphereLineIndices;
	static short mSphereLineIndexCount;

	static GLfloat* mCylinderVertices;
	static short mCylinderVertexCount;
	static GLushort* mCylinderIndices;
	static short mCylinderIndexCount;
	static GLushort* mCylinderLineIndices;
	static short mCylinderLineIndexCount;
};

#endif