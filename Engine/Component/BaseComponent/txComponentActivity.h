#ifndef _TX_COMPONENT_ACTIVITY_H_
#define _TX_COMPONENT_ACTIVITY_H_

#include "txComponent.h"

// 运动组件
class txComponentActivity : public txComponent
{
public:
	txComponentActivity(const std::string& type, const std::string& name)
		:
		txComponent(type, name),
		mGravityAcceleration(9.8f)
	{}
	virtual ~txComponentActivity(){ destroy(); }
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentActivity); }
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentActivity); }
	void destroy(){}
	void checkSpeed(VECTOR3& speed)
	{
		txMath::checkInt(speed.x);
		txMath::checkInt(speed.y);
		txMath::checkInt(speed.z);
	}
	void checkAndSetSpeed(VECTOR3& speed)
	{
		checkSpeed(speed);
		changeSpeed(speed);
	}

	// 以下函数需要子类实现
	// 通知物体开始下降
	virtual void notifyStartFall(){}
	// 通知物体着陆了
	virtual void notifyLand(){}
	// 使物体改变速度
	virtual void changeSpeed(const VECTOR3& speed){}
	virtual void getHeight(const float& x, const float& z, float& y)
	{
		y = 0.0f;
	}
protected:
	float mGravityAcceleration;
};

#endif 