#ifndef _TX_TREMBLING_NODE_H_
#define _TX_TREMBLING_NODE_H_

#include "txEngineDefine.h"

class txTremblingKeyFrame;
// 震动维度
class txTremblingNode
{
public:
	txTremblingNode(const std::string& name)
		:
		mLength(0.0f),
		mName(name)
	{}
	virtual ~txTremblingNode() { destroy(); }
	virtual void init(){}
	void destroy(){ clear(); }
	void clear();
	void saveTrembingNode(std::string& stream);
	float queryKeyFrame(const float& keyTime);
	bool deleteKeyFrame(const int& index);
	txTremblingKeyFrame* insertKeyFrame(const int& index, const float& time, const float& offset);
	bool allowedKeyFrameTime(const int& index, const float& time);
	void setKeyFrameList(txMap<float, float>& keyFrameList);
	int findIndex(const float& time);	// 查找该时间点的前一个关键帧的下标
	txTremblingKeyFrame* getKeyFrame(const int& index)
	{
		if (index >= 0 && index < (int)mKeyFrameList.size())
		{
			return mKeyFrameList[index];
		}
		return NULL;
	}
	txVector<txTremblingKeyFrame*>& getKeyFrameList()				{ return mKeyFrameList; }
	int getKeyFrameCount()											{ return mKeyFrameList.size(); }
	const float& getLength()										{ return mLength; }
	const std::string& getName()									{ return mName; }
	const std::string& getInfo()									{ return mInfo; }
	void setInfo(const std::string& info)							{ mInfo = info; }
protected:
	txVector<txTremblingKeyFrame*> mKeyFrameList;
	float mLength;
	std::string mName;
	std::string mInfo;
};

#endif