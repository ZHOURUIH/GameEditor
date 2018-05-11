#include "TextWindowProperty.h"

TextWindowPropertyText::TextWindowPropertyText()
{
	mPropertyName = "Text";
	mDescription = "设置文字窗口显示的文字";
}
void TextWindowPropertyText::setRealValue(const std::string& value, TextWindow* window)
{
	window->setText(value, mAsync);
}
std::string TextWindowPropertyText::getRealValue(TextWindow* window)
{
	return window->getText();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextWindowPropertyTextDockingPosition::TextWindowPropertyTextDockingPosition()
{
	mPropertyName = "TextDockingPosition";
	mDescription = "设置文字窗口的文字位置";
	ADD_ENUM(WDP_LEFT);
	ADD_ENUM(WDP_RIGHT);
	ADD_ENUM(WDP_CENTER);
}
void TextWindowPropertyTextDockingPosition::setRealValue(const WINDOW_DOCKING_POSITION& value, TextWindow* window)
{
	window->setDockingPosition(value);
}
WINDOW_DOCKING_POSITION TextWindowPropertyTextDockingPosition::getRealValue(TextWindow* window)
{
	return window->getDockingPosition();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
txVector<std::string> TextWindowPropertyFont::mChoices;
TextWindowPropertyFont::TextWindowPropertyFont()
{
	mPropertyName = "Font";
	mDescription = "字体名";
}
void TextWindowPropertyFont::setRealValue(const std::string& value, TextWindow* window)
{
	window->setFontName(value, mAsync);
}
std::string TextWindowPropertyFont::getRealValue(TextWindow* window)
{
	return window->getFontName();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextWindowPropertyTextRotateWithWindow::TextWindowPropertyTextRotateWithWindow()
{
	mPropertyName = "TextRotateWithWindow";
	mDescription = "文字是否随窗口变换还是单独变换(旋转和缩放)";
}
void TextWindowPropertyTextRotateWithWindow::setRealValue(const bool& value, TextWindow* window)
{
	window->setRotateWithWindow(value);
}
bool TextWindowPropertyTextRotateWithWindow::getRealValue(TextWindow* window)
{
	return window->getRotateWithWindow();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextWindowPropertyTextSpace::TextWindowPropertyTextSpace()
{
	mPropertyName = "TextSpace";
	mDescription = "文字渲染间隙,其中的相对值是相对于窗口最终高度";
}
void TextWindowPropertyTextSpace::setRealValue(const txDim& value, TextWindow* window)
{
	window->setSpace(value);
}
txDim TextWindowPropertyTextSpace::getRealValue(TextWindow* window)
{
	return window->getSpace();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
TextWindowPropertyMaxTextCount::TextWindowPropertyMaxTextCount()
{
	mPropertyName = "MaxTextCount";
	mDescription = "最大文字个数";
}
void TextWindowPropertyMaxTextCount::setRealValue(const int& value, TextWindow* window)
{
	window->setMaxLength(value);
}
int TextWindowPropertyMaxTextCount::getRealValue(TextWindow* window)
{
	return window->getMaxLength();
}