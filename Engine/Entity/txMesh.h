#ifndef _TX_MESH_H_
#define _TX_MESH_H_

#include "txDirLight.h"
#include "txAABox.h"

class txSkin;
class txMesh;
struct SkinMeshPair
{
	txMesh* mMesh;
	txSkin* mSkin;
};

class txSkeleton;
class txMaterial;
class txSubMesh;
class txMesh
{
public:
	txMesh();
	virtual ~txMesh();
	// 拷贝所有数据到pMesh中
	void CopyMesh(txMesh* pMesh);
	txVector<txMaterial*> getAllMaterials();
	// 初始化所有子模型的纹理
	void GenerateTextures();
	void SetNumTris(const int& num){ nNumTris = num; }
	void MakeVertexBuffer(const int& len);
	void MakeNormalBuffer(const int& len);
	void MakeTexCoordBuffer(const int& len);
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint, const MATRIX4& aabbTransform);
	void SetAABox(const txAABox& pAABox);
	txSubMesh* CreateSubMesh();
	bool isSkeletonMesh;
	void setAutoDestroyTexture(const bool& autoDestroy);
	void createAABBRenderBuffer(const txAABox& aabb);
	void create3DMaxAABBRenderBuffer(const txAABox& aabb);
	//get------------------------------------------------------------------------------------------------------------------------
	GLfloat* getAABBVertexBuffer() { return mAABBBufferV; }
	GLushort* getAABBIndexBuffer() { return mAABBBufferN; }
	const int& getAABBVertexBufferSize() { return mAABBVertexBufferSize; }
	const int& getAABBIndexBufferSize() { return mAABBIndexBufferSize; }
	GLfloat* get3DMaxAABBVertexBuffer() { return m3DMaxAABBBufferV; }
	GLushort* get3DMaxAABBIndexBuffer() { return m3DMaxAABBBufferN; }
	const int& get3DMaxAABBVertexBufferSize() { return m3DMaxAABBVertexBufferSize; }
	const int& get3DMaxAABBIndexBufferSize() { return m3DMaxAABBIndexBufferSize; }
	int getSubMeshNum() { return mSubMeshes.size(); }
	txSubMesh* getSubMesh(const int& nIndex);
	txSubMesh* getMaterialSubMesh(const int& nMaterialIndex);
	txAABox* GetAABox() { return m_pAABox; }
	txAABox* get3DMaxAABB() { return m3DMaxAABB; }
	const int& GetVertexBufferSize(){ return VSize; }
	const int& GetNormalBufferSize(){ return NSize; }
	const int& GetTexCoordBufferSize(){ return TSize; }
	GLfloat* GetVertexBuffer(){ return pVBuf; }
	GLfloat* GetNormalBuffer(){ return pNBuf; }
	GLfloat* GetTexCoordBuffer(){ return pTBuf; }
	const int& GetNumTris(){ return nNumTris; }
protected:
	GLfloat* pVBuf;	//vertex
	GLfloat* pNBuf;	//Normal
	GLfloat* pTBuf;	//TexCoord
	txAABox* m_pAABox;	//AABox
	txAABox* m3DMaxAABB; // 通过调整以后的原始包围盒,只有动态模型需要调整,因为坐标系与3dmax中的不同
	txVector<txSubMesh*> mSubMeshes;		//material sets
	int VSize, NSize, TSize, ISize;
	int nNumTris;
	// 渲染包围盒用的顶点数据
	int mAABBVertexBufferSize;
	int mAABBIndexBufferSize;
	GLfloat* mAABBBufferV;
	GLushort* mAABBBufferN;
	//3DMax渲染包围盒用的顶点数据
	int m3DMaxAABBVertexBufferSize;
	int m3DMaxAABBIndexBufferSize;
	GLfloat* m3DMaxAABBBufferV;
	GLushort* m3DMaxAABBBufferN;
};

#endif