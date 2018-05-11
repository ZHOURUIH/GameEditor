#ifndef _ACTION_WINDOW_H_
#define _ACTION_WINDOW_H_

#include "txAction.h"
#include "txCommandReceiver.h"

#include "WindowHeader.h"

class ActionWindow : public txAction
{
public:
	ActionWindow(const std::string& typeName, const std::string& name)
		:
		txAction(typeName, name),
		mWindow(NULL)
	{}
	virtual ~ActionWindow(){ destroy(); }
	virtual void initProperty();
	virtual void update(float elapsedTime){}
	void setProperty(const std::string& propertyName, const std::string& propertyValue);
	void destroy(){}
	void setWindow(Window* window){ mWindow = window; }
	Window* getWindow() { return mWindow; }
protected:
	Window* mWindow;
};

#endif