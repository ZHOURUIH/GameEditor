#ifndef _CAMERA_TREMBLING_H_
#define _CAMERA_TREMBLING_H_

#include "txComponentKeyFrame.h"

// 相机震动组件
class CameraTrembling : public txComponentKeyFrame
{
public:
	CameraTrembling(const std::string& type, const std::string& name)
		:
		txComponentKeyFrame(type, name)
	{}
	virtual ~CameraTrembling(){ destroy(); }
	virtual void lateUpdate(float elapsedTime);
	virtual bool isType(const std::string& type)
	{
		return txComponentKeyFrame::isType(type) || type == TOSTRING(CameraTrembling);
	}
	void destroy(){}
	virtual void applyKeyFrame(const txVector<float>& value);
	virtual void stop()
	{
		txComponentKeyFrame::stop();
		mCurOffset = txMath::VEC3_ZERO;
	}
protected:
	VECTOR3 mCurOffset;
};

#endif