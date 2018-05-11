#ifndef _TX_SUBENTITY_H_
#define _TX_SUBENTITY_H_

#include "txEngineDefine.h"

class txMesh;
class txEntity;
class txMaterial;
class txSkin;
class txSubEntity
{
public:
	txSubEntity(txEntity* parent);
	virtual ~txSubEntity();

	void init(txMesh* mesh, const bool& bHasSkeleton = false);

	// 初始化子实体的纹理
	void GenerateSubEntityTextures();
	txVector<txMaterial*> getAllMaterials();
	txMesh* GetOriMesh(){ return mOrimesh; }
	txEntity* GetParent(){ return mParent; }
	void SetParent(txEntity* ent){ mParent = ent; }
	void SetMeshSkin(txSkin* pSkin);
	txSkin* GetMeshSkin() { return mSkin; }
	void BuildBoneMatrixArray();
	MATRIX4* GetBoneMatrixArray(){ return &boneMatrices[0]; }
	const int& GetBoneMatrixNum(){ return mBoneMatrixNum; }
	GLfloat* GetBoneIndicesArray(){ return boneIndices; }
	GLfloat* GetBoneWeightsArray(){ return boneWeights; }
	// 拷贝子实体的数据,parent(构造时就确定了),dataindex(实际上拷贝实体过程中每个类都没有考虑过这个dataindex)除外
	// boneMatrices不需要拷贝,是在播放动画时计算的
	void CopySubEntity(txSubEntity* pSubEntity, const ENTITY_COPY_TYPE& copyType);
	bool rayIntersect(const VECTOR3& ori, const VECTOR3& dir, VECTOR3& intersectPoint);
protected:
	void BuildSkinArray();
protected:
	// skin由外部创建,然后由subEntity销毁
	txSkin* mSkin;
	// mesh由外部创建,然后由subEntity销毁,与skin一样
	txMesh* mOrimesh;
	txEntity* mParent;
	MATRIX4 boneMatrices[RENDER_BONE_MAXNUM];
	txMap<std::string, int> mBoneMap;
	int mBoneMatrixNum;
	GLfloat* boneIndices;
	GLfloat* boneWeights;
	ENTITY_COPY_TYPE mCopyType;
};

#endif