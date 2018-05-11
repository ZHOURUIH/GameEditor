#ifndef _CAMERA_LINKER_FIXED_H_
#define _CAMERA_LINKER_FIXED_H_

#include "CameraLinker.h"

//摄像机跟随组件
class CameraLinkerFixed : public CameraLinker
{
public:
	CameraLinkerFixed(const std::string& type, const std::string& name)
		:
		CameraLinker(type, name)
	{}
	virtual ~CameraLinkerFixed(){ destroy(); }
	virtual void update(float elapsedTime);
	virtual bool isType(const std::string& type)
	{
		return CameraLinker::isType(type) || type == TOSTRING(CameraLinkerFixed);
	}
	void destroy(){}
protected:
};

#endif