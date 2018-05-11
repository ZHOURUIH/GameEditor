#ifndef _NUMBER_WINDOW_PROPERTY_H_
#define _NUMBER_WINDOW_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(NumberWindow)

//---------------------------------------------------------------------------------------------------------------------------------------------
// 数字显示
class NumberWindowPropertyNumber : public NumberWindowProperty<txStringProperty, std::string>
{
public:
	NumberWindowPropertyNumber();
	virtual void setRealValue(const std::string& value, NumberWindow* window);
	virtual std::string getRealValue(NumberWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 数字风格
class NumberWindowPropertyNumberStyle : public NumberWindowProperty<txStringProperty, std::string>
{
public:
	NumberWindowPropertyNumberStyle();
	virtual void setRealValue(const std::string& value, NumberWindow* window);
	virtual std::string getRealValue(NumberWindow* window);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(const txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 数字间距
class NumberWindowPropertyNumberInterval : public NumberWindowProperty<txDimProperty, txDim>
{
public:
	NumberWindowPropertyNumberInterval();
	virtual void setRealValue(const txDim& value, NumberWindow* window);
	virtual txDim getRealValue(NumberWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 数字最大个数
class NumberWindowPropertyMaxNumberCount : public NumberWindowProperty<txIntProperty, int>
{
public:
	NumberWindowPropertyMaxNumberCount();
	virtual void setRealValue(const int& value, NumberWindow* window);
	virtual int getRealValue(NumberWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 数字停靠位置
class NumberWindowPropertyNumberDockingPosition : public NumberWindowProperty<txEnumProperty<WINDOW_DOCKING_POSITION>, WINDOW_DOCKING_POSITION>
{
public:
	NumberWindowPropertyNumberDockingPosition();
	virtual void setRealValue(const WINDOW_DOCKING_POSITION& value, NumberWindow* window);
	virtual WINDOW_DOCKING_POSITION getRealValue(NumberWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 数字是否随窗口一起旋转
class NumberWindowPropertyRotateWithWindow : public NumberWindowProperty<txBoolProperty, bool>
{
public:
	NumberWindowPropertyRotateWithWindow();
	virtual void setRealValue(const bool& value, NumberWindow* window);
	virtual bool getRealValue(NumberWindow* window);
};

#endif