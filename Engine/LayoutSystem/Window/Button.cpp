#include "txTexture.h"

#include "WindowHeader.h"
#include "ButtonProperty.h"

Button::Button(const std::string& name, const WINDOW_TYPE& type)
	:
	TextureWindow(name, type),
	mState(BS_NORMAL)
{
	for (int i = 0; i < BS_MAX_STATE; ++i)
	{
		mButtonTextureList[i] = NULL;
	}
}

void Button::initProperty()
{
	TextureWindow::initProperty();
	addProperty<ButtonPropertyNormalTexture>();
	addProperty<ButtonPropertyHoverTexture>();
	addProperty<ButtonPropertyPushedTexture>();
	addProperty<ButtonPropertyDisabledTexture>();
}

void Button::finishAsycLoad()
{
	for (int i = 0; i < BS_MAX_STATE; ++i)
	{
		if (mButtonTextureList[i] != NULL)
		{
			mButtonTextureList[i]->loadGLTexture();
		}
	}
}

void Button::getUsedResourceList(txVector<std::string>& resourceList)
{
	for (int i = 0; i < BS_MAX_STATE; ++i)
	{
		if (mButtonTextureList[i] != NULL)
		{
			resourceList.push_back(mButtonTextureList[i]->getName());
		}
	}
}