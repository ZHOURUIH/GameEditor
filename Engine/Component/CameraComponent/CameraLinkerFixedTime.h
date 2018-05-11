#ifndef _CAMERA_LINKER_FIXED_TIME_H_
#define _CAMERA_LINKER_FIXED_TIME_H_

#include "CameraLinker.h"

// 摄像机跟随组件
class CameraLinkerFixedTime : public CameraLinker
{
public:
	CameraLinkerFixedTime(const std::string& type, const std::string& name)
		:
		CameraLinker(type, name),
		mTime(0.2f),
		mIgnoreY(true)
	{}
	virtual ~CameraLinkerFixedTime(){ destroy(); }
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type)
	{
		return CameraLinker::isType(type) || type == TOSTRING(CameraLinkerFixedTime);
	}
	void destroy(){}
	void setFixedTime(const float& time) { mTime = time; }
	void setIgnoreY(const bool& ignore) { mIgnoreY = ignore; }
	const bool& getIgnoreY() { return mIgnoreY; }
protected:
	float mTime;
	bool mIgnoreY;		// 是否忽略Y轴的变化,当Y轴变化时摄像机在Y轴上的位置不会根据时间改变
};

#endif