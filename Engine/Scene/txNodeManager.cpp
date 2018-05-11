#include "txNodeManager.h"
#include "txNode.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"

int txNodeManager::mNodeIndexSeed = 0;

txNodeManager::txNodeManager()
{
	mAllNodeList.clear();
	mNeedDeleteNodeList.clear();
}

txNodeManager::~txNodeManager()
{
	DestroyAllNode();
	ClearUselessNode();
}

void txNodeManager::update(float elapsedTime)
{
	ClearUselessNode();
}

// 创建节点
txNode* txNodeManager::CreateNode(const std::string& name, const bool& useUniqueName)
{
	std::string nodeName = name;
	if(useUniqueName)
	{
		nodeName = CreateUniqueNodeName();
	}
	else
	{
		if (nodeName == EMPTY_STRING)
		{
			ENGINE_ERROR("error : node name can not be empty srting!");
			return NULL;
		}
	}
	if (GetNode(nodeName) != NULL)
	{
		ENGINE_ERROR("error : node named : %s have already exists!", nodeName.c_str());
		return NULL;
	}
	txNode* pNode = TRACE_NEW(txNode, pNode, nodeName);
	mAllNodeList.insert(nodeName, pNode);
	return pNode;
}

// 得到一个唯一的节点名称,如果外部在创建节点时使用了相同的命名格式,则生成的节点名称可能不是唯一的
std::string txNodeManager::CreateUniqueNodeName()
{
	return "node_" + txStringUtility::intToString(mNodeIndexSeed++);
}

// 根据名字得到节点
txNode* txNodeManager::GetNode(const std::string& name)
{
	auto iter = mAllNodeList.find(name);
	if(iter == mAllNodeList.end())
	{
		return NULL;
	}
	return iter->second;
}

// 销毁节点,实际只是标记一下该节点需要被销毁
bool txNodeManager::DestroyNode(const std::string& name)
{
	auto iter = mAllNodeList.find(name);
	if(iter == mAllNodeList.end())
	{
		ENGINE_ERROR("error : can not destroy node named : %s, can not find it!", name.c_str());
		return false;
	}
	txNode* pNode = iter->second;
	// 需要清空节点上挂接的物体
	pNode->DetachObject();
	// 将该节点放入删除列表中
	mNeedDeleteNodeList.push_back(pNode);
	// 从节点列表中删除
	mAllNodeList.erase(iter);
	return true;
}

// 销毁所有节点,但是不包括被标记需要销毁的节点
void txNodeManager::DestroyAllNode()
{
	auto iter = mAllNodeList.begin();
	auto iterEnd = mAllNodeList.end();
	FOR_STL(mAllNodeList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mAllNodeList);
	mAllNodeList.clear();
}

// 删除需要被删除的节点
void txNodeManager::ClearUselessNode()
{
	int listSize = mNeedDeleteNodeList.size();
	FOR_STL(mNeedDeleteNodeList, int i = 0; i < listSize; ++i)
	{
		TRACE_DELETE(mNeedDeleteNodeList[i]);
	}
	END_FOR_STL(mNeedDeleteNodeList);
	mNeedDeleteNodeList.clear();
}