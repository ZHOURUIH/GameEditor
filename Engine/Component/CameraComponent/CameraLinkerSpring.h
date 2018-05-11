#ifndef _CAMERA_COMPONENT_SPRING_H_
#define _CAMERA_COMPONENT_SPRING_H_

#include "CameraLinker.h"

// 摄像机弹簧组件
class CameraLinkerSpring: public CameraLinker
{
public:
	CameraLinkerSpring(const std::string& type, const std::string& name)
		:
		CameraLinker(type, name),
		mCurLength(0.0f),
		mNormalLength(0.0f),
		mSpringK(0.0f)
	{}
	virtual ~CameraLinkerSpring(){ destroy(); }
	virtual bool isType(const std::string& type)
	{
		return CameraLinker::isType(type) || type == TOSTRING(CameraLinkerSpring);
	}
	void destroy(){}
	void setNormalLength(const float& length){ mNormalLength = length; }    //摄像机正常的长度
	void setSpringK(const float& k){ mSpringK = k; }                 //摄像机弹力
	void setCurLength(const float& curLength);       //当前的长度
protected:
	// 根据弹簧当前的参数,计算弹簧的拉力或者推力
	float calculateCurForce()
	{
		float force = (mCurLength - mNormalLength) * mSpringK; //当前距离减去正常距离乘以长度就是所受的力
		return force;
	}
private:
	float mCurLength; //当前的距离
	float mNormalLength;//正常情况下的距离
	float mSpringK;//弹簧系数     单位N/m
};

#endif