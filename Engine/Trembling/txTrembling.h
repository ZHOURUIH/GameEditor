#ifndef _TX_TREMBLING_H_
#define _TX_TREMBLING_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txTremblingNode;

// 震动节点存储
class txTrembling : public txEngineBase
{
public:
	txTrembling(const std::string& name)
		:
		mLength(0.0f),
		mName(name)
	{}
	virtual ~txTrembling(){ destroy(); }
	bool init(const std::string& filePath);
	void destroy();
	void saveTrembling(std::string& stream);
	bool readFile(const std::string& filePath);
	txTremblingNode* addNode(const std::string& name, const std::string& info, txMap<float, float>& keyFrameList);
	void deleteNode(const std::string& name);
	txTremblingNode* getNode(const std::string& name);
	void refreshLength();
	void queryValue(const float& time, txVector<float>& valueList, const float& amplitude = 1.0f);
	txVector<float> queryValue(const float& time, const float& amplitude = 1.0f);
	txVector<txTremblingNode*>& getTremblingNodeList()	{ return mTremblingNodeList; }
	const float& getLength()							{ return mLength; }
	const std::string& getName()						{ return mName; }
	const std::string& getInfo()						{ return mInfo; }
	const std::string& getFormat()						{ return mFormat; }
	void setInfo(const std::string& info)				{ mInfo = info; }
	void setFormat(const std::string& format)			{ mFormat = format; }
protected:
	txVector<txTremblingNode*> mTremblingNodeList;
	txMap<std::string, int> mTremblingMap;			// first是节点的名字,second是节点在vector中的下标
	float mLength;			// 震动长度
	std::string mName;		// 震动名
	std::string mInfo;		// 震动的描述
	std::string mFormat;	// 描述关键帧格式
	static int mDimensionNameSeed;
};

#endif