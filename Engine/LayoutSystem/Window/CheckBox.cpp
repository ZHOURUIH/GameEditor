#include "txTexture.h"

#include "WindowHeader.h"
#include "Layout.h"
#include "CheckBoxProperty.h"

CheckBox::CheckBox(const std::string& name, const WINDOW_TYPE& type)
	:
	ComplexWindow(name, type),
	mBackground(NULL),
	mMask(NULL)
{}

void CheckBox::init(Layout* layout)
{
	ComplexWindow::init(layout);
	mBackground = (TextureWindow*)(createInnerChild("TextureWindow", mName + "_background"));
	mMask = (TextureWindow*)(createInnerChild("TextureWindow", mName + "_mask"));
	addInnerChild(mBackground);
	mBackground->addChild(mMask);
}

void CheckBox::initProperty()
{
	ComplexWindow::initProperty();
	addProperty<CheckBoxPropertyCheck>();
}

void CheckBox::setCkeck(const bool& check)
{
	mMask->setVisible(check);
	// 发送选中改变的事件
	pushWindowEvent(check ? WE_SELECT : WE_UNSELECT);
}

bool CheckBox::isChecked()
{
	return mMask->isVisible();
}

void CheckBox::onLeftDown(const VECTOR2& position)
{
	ComplexWindow::onLeftDown(position);
	setCkeck(!isChecked());
}
void CheckBox::renameInnerChildren(const std::string& namePrefix)
{
	mBackground->rename(namePrefix + "_background");
	mMask->rename(namePrefix + +"_mask");
}