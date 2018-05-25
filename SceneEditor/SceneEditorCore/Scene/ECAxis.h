#ifndef _EC_AXIS_H_
#define _EC_AXIS_H_

#include "txMovableObject.h"

class ECEditor;
class ECAxis : public txMovableObject
{
public:
	ECAxis(const std::string& name)
		:
		txMovableObject(name),
		mAttachTarget(NULL),
		mAutoShow(true)
	{}
	virtual ~ECAxis(){ destroy(); }
	virtual void initComponents();
	virtual void update(float elapsedTime);
	void destroy(){}
	void attachTarget(ECEditor* editor);
	void setVisible(bool visible);
	bool isVisible();
	void setAutoShow(bool autoShow);
	bool getAutoShow() { return mAutoShow; }
	ECEditor* getAttachTarget() { return mAttachTarget; }
protected:
	ECEditor* mAttachTarget;
	bool mAutoShow;				// 为true时,在挂接到物体上时会自动显示,从物体上取下时会自动隐藏,false则不会自动显示或隐藏
};

#endif