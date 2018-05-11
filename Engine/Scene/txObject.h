#ifndef	_TX_OBJECT_H_
#define _TX_OBJECT_H_

#include "txEngineDefine.h"
#include "txNode.h"
#include "txEngineBase.h"

class txObject : public txEngineBase
{
public:
	txObject(OBJECT_TYPE type)
		:
		mType(type)
	{
		memset(mName, 0, 200);
		mParent = NULL;
	}
	virtual ~txObject()
	{
		// 如果当前还挂接在节点上,则从节点上取下来
		if (mParent != NULL)
		{
			// 先清空指针,然后再调用节点的移除物体,避免无限循环调用
			txNode* parent = mParent;
			mParent = NULL;
			parent->DetachObject();
		}
		memset(mName, 0, 200);
	}
	void setName(const char *name)
	{
		memcpy(mName, name, strlen(name));
	}
	const char* getName(){ return mName; }
	const OBJECT_TYPE& getType() { return mType; }
	txNode* GetParent(){ return mParent; }
	void SetParent(txNode* node)
	{
		// 如果自己已经有父节点了,则先从父节点上取下来
		if (mParent != NULL)
		{
			// 先清空指针,然后再调用节点的移除物体,避免无限循环调用
			txNode* parent = mParent;
			mParent = NULL;
			parent->DetachObject();
		}
		// 然后挂接到新的父节点上
		mParent = node;
		if (mParent != NULL)
		{
			mParent->AttachObject(this);
		}
		// 设置了父节点后通知自己父节点有改变
		NotifyParentUpdated();
	}

	// 当父节点发生了改变或者被刷新时,通知物体
	virtual void NotifyParentUpdated() = 0;
	virtual void Update(float fElaspedTime) = 0;
protected:
	char mName[200];
	txNode* mParent;
	OBJECT_TYPE mType;
};

#endif
