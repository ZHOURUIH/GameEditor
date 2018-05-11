#ifndef _NODE_MANAGER_H_
#define _NODE_MANAGER_H_

#include "txEngineDefine.h"

class txNode;

// 节点管理器,用于管理节点的创建和销毁,所有的节点都不能外部直接new出来,必须使用该管理器来创建
class txNodeManager
{
public:
	txNodeManager();
	virtual ~txNodeManager();
	void init(){}
	void update(float elapsedTime);
	// 创建节点
	txNode* CreateNode(const std::string& name = EMPTY_STRING, const bool& useUniqueName = true);
	// 根据名字得到节点
	txNode* GetNode(const std::string& name);
	// 得到一个唯一的节点名称,如果外部在创建节点时使用了相同的命名格式,则生成的节点名称可能不是唯一的
	std::string CreateUniqueNodeName();
	// 销毁节点,实际只是标记一下该节点需要被销毁
	bool DestroyNode(const std::string& name);
	// 销毁所有节点,但是不包括被标记需要销毁的节点
	void DestroyAllNode();
	// 销毁需要被销毁的节点,只在一帧更新的最后才会调用该函数
	void ClearUselessNode();
protected:
	txVector<txNode*> mNeedDeleteNodeList;
	txMap<std::string, txNode*> mAllNodeList;
	static int mNodeIndexSeed; // 用于生成唯一的节点名称
};

#endif