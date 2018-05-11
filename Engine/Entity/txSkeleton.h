#ifndef _TX_SKELETON_H_
#define _TX_SKELETON_H_

#include "txEngineDefine.h"

class txBone;
class txBoneHook;
class txEntity;
class txSkeleton
{
public:
	txSkeleton(txEntity* parentEntity);
	virtual ~txSkeleton();
	void Clear();
	bool IsSkeletonEmpty() {return mRoot == NULL;}
	txBone* GetRootBone(){ return mRoot; }
	txBone* CreateBone(const std::string& name);
	txBone* GetBone(const std::string& name);
	int getBoneCount() { return mBoneList.size(); }
	static void DestroyBone(txBone* pBone);
	// 根据骨头名和挂点名创建挂点,会先查找骨头然后创建挂点
	txBoneHook* CreateBoneHook(const std::string& boneName, const std::string& hookName);
	// 根据骨头名和挂点名得到挂点,会先查找骨头然后得到挂点
	txBoneHook* GetBoneHook(const std::string& boneName, const std::string& hookName);
	// 根据骨头名和挂点名销毁挂点,会先查找骨头然后销毁挂点
	bool DestroyBoneHook(const std::string& boneName, const std::string& hookName);
	// 在所有挂点列表中得到指定名字的挂点,会先得到骨头名,然后查找骨头,再得到挂点
	txBoneHook* GetHookInAllBones(const std::string& hookName);
	// 在所有挂点列表中销毁指定名字的挂点,会先得到骨头名,然后查找骨头,再销毁挂点
	bool DestroyHookInAllBones(const std::string& hookName);
	// 返回是否在指定的骨头中有指定名字的挂点
	bool HasHook(const std::string& boneName, const std::string& hookName);
	// 在所有挂点列表中查找是否有该名字的挂点
	bool HasHookInAllBones(const std::string& hookName);
	void SetBonesParent();
	// 将挂点名和挂点绑定的骨头名添加到列表中,只能由挂点的构造函数调用
	void AddHookNameToList(const std::string& hookName, const std::string& boneName);
	// 将指定的挂点名从列表中删除,只能由挂点的析构函数调用
	void RemoveHookNameFromList(const std::string& hookName);
	// 拷贝骨骼数据
	void CopySkeleton(txSkeleton* pSkeleton);
	void Refresh();
protected:
	txMap<std::string, txBone*> mBoneList;
	txBone* mRoot;
	txEntity* mParentEntity;
	// 所有骨头的挂点列表,first为挂点名称,second为挂点所绑定的骨头的名称
	// 所以不仅同一块骨头上的所有挂点不允许有重复的名字,一个骨骼中所有骨头的所有挂点的也不允许有相同的挂点名称
	// 在挂点的构造中将挂点名加入列表,在挂点的析构中将挂点名从列表中删除
	txMap<std::string, std::string> mBoneHookList;
};

#endif