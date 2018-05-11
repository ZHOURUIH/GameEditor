#ifndef _TEXT_WINDOW_PROPERTY_H_
#define _TEXT_WINDOW_PROPERTY_H_

#include "WindowProperty.h"

WINDOW_PROPERTY(TextWindow)

//---------------------------------------------------------------------------------------------------------------------------------------------
// 文字
class TextWindowPropertyText : public TextWindowProperty<txStringProperty, std::string>
{
public:
	TextWindowPropertyText();
	virtual void setRealValue(const std::string& value, TextWindow* window);
	virtual std::string getRealValue(TextWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 文字停靠位置
class TextWindowPropertyTextDockingPosition : public TextWindowProperty<txEnumProperty<WINDOW_DOCKING_POSITION>, WINDOW_DOCKING_POSITION>
{
public:
	TextWindowPropertyTextDockingPosition();
	virtual void setRealValue(const WINDOW_DOCKING_POSITION& value, TextWindow* window);
	virtual WINDOW_DOCKING_POSITION getRealValue(TextWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 字体名
class TextWindowPropertyFont : public TextWindowProperty<txStringProperty, std::string>
{
public:
	TextWindowPropertyFont();
	virtual void setRealValue(const std::string& value, TextWindow* window);
	virtual std::string getRealValue(TextWindow* window);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(const txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 文字是否随窗口变换还是单独变换(旋转和缩放)
class TextWindowPropertyTextRotateWithWindow : public TextWindowProperty<txBoolProperty, bool>
{
public:
	TextWindowPropertyTextRotateWithWindow();
	virtual void setRealValue(const bool& value, TextWindow* window);
	virtual bool getRealValue(TextWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 文字渲染间隙
class TextWindowPropertyTextSpace : public TextWindowProperty<txDimProperty, txDim>
{
public:
	TextWindowPropertyTextSpace();
	virtual void setRealValue(const txDim& value, TextWindow* window);
	virtual txDim getRealValue(TextWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 最大文字个数
class TextWindowPropertyMaxTextCount : public TextWindowProperty<txIntProperty, int>
{
public:
	TextWindowPropertyMaxTextCount();
	virtual void setRealValue(const int& value, TextWindow* window);
	virtual int getRealValue(TextWindow* window);
};

#endif
