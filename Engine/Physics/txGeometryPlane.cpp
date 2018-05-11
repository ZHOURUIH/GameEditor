#include "txGeometryPlane.h"
#include "txCamera.h"
#include "txPhysicsUtility.h"
#include "txRigidBody.h"
#include "txRenderUtility.h"

txGeometryPlane::txGeometryPlane(txRigidBody* body, dSpaceID space, const GEOM_TYPE& type)
:
txGeometry(body, space, type),
mX(0.0f),
mZ(0.0f)
{
	;
}

void txGeometryPlane::create(const float& x, const float& z, const float& mass, const VECTOR3& pos, const MATRIX3& rot)
{
	mX = x;
	mZ = z;

	// 开始创建可叠加的形状
	startCreateGeometry(pos, rot);

	// 创建被变换的球体形状
	dGeomID tempGeom = createPlane();

	// 结束创建可叠加的形状
	endCreateGeometry(tempGeom, mass);
}

dGeomID txGeometryPlane::createPlane()
{
	// 两个三角形共4个顶点
	mVertexCount = 4;
	mVertices = TRACE_NEW_ARRAY(float, mVertexCount * 3, mVertices);
	fillPlaneVertices(mX, mZ);

	// 两个三角形共6个索引
	mIndexCount = 2 * 3;
	mIndices = TRACE_NEW_ARRAY(dTriIndex, mIndexCount, mIndices);
	int triangleIndex = 0;
	mIndices[triangleIndex * 3 + 0] = 0;
	mIndices[triangleIndex * 3 + 1] = 1;
	mIndices[triangleIndex * 3 + 2] = 2;
	++triangleIndex;

	mIndices[triangleIndex * 3 + 0] = 0;
	mIndices[triangleIndex * 3 + 1] = 2;
	mIndices[triangleIndex * 3 + 2] = 3;
	++triangleIndex;

	// 根据顶点和索引数据创建平面模型
	dTriMeshDataID triMeshData = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSingle(triMeshData, mVertices, 3 * sizeof(float), mVertexCount, mIndices, mIndexCount, 3 * sizeof(dTriIndex));
	dGeomID geom = dCreateTriMesh(NULL, triMeshData, NULL, NULL, NULL);
	return geom;
}

void txGeometryPlane::fillPlaneVertices(const float& x, const float& z)
{
	int vertexIndex = 0;
	mVertices[vertexIndex * 3 + 0] = x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = -z / 2.0f;
	++vertexIndex;

	mVertices[vertexIndex * 3 + 0] = -x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = -z / 2.0f;
	++vertexIndex;

	mVertices[vertexIndex * 3 + 0] = -x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = z / 2.0f;
	++vertexIndex;

	mVertices[vertexIndex * 3 + 0] = x / 2.0f;
	mVertices[vertexIndex * 3 + 1] = 0.0f;
	mVertices[vertexIndex * 3 + 2] = z / 2.0f;
	++vertexIndex;
}

void txGeometryPlane::render(txCamera* camera)
{
	txRenderUtility::renderPlane(camera, mX, mZ, mRigidBody->getWorldTransform() * mRelativeTransform, mColour, mWireframe);
}

void txGeometryPlane::setMass(const float& mass)
{
	// 使用盒子的质量来代替
	dMass planeMass;
	dMassSetBoxTotal(&planeMass, mass, mX, 0.001f, mZ);
	// 变换质量
	transformMass(planeMass);
	// 刷新刚体质量
	mRigidBody->refreshMass();
}

void txGeometryPlane::setWidthX(const float& x)
{
	setSide(x, mZ);
}

void txGeometryPlane::setLengthZ(const float& z)
{
	setSide(mX, z);
}

void txGeometryPlane::setSide(const float& x, const float& z)
{
	mX = x;
	mZ = z;
	dGeomID geom = mGeom;
	if (dGeomGetClass(geom) == dGeomTransformClass)
	{
		geom = dGeomTransformGetGeom(geom);
	}
	fillPlaneVertices(mX, mZ);
	// 修改参数后,需要刷新质量
	setMass(mMass.mass);
}