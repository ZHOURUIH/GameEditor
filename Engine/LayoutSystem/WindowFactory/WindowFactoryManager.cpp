#include "WindowFactoryManager.h"
#include "WindowFactory.h"
#include "Layout.h"
#include "WindowHeader.h"

void WindowFactoryManager::init()
{
	addFactory<WindowFactory<WindowPrefab> >(WT_PREFAB)->setPublicType(false);
	addFactory<WindowFactory<Window> >(WT_WINDOW);
	addFactory<WindowFactory<Button> >(WT_BUTTON);
	addFactory<WindowFactory<EditBox> >(WT_EDITBOX);
	addFactory<WindowFactory<CheckBox> >(WT_CHECKBOX);
	addFactory<WindowFactory<NumberWindow> >(WT_NUMBER_WINDOW);
	addFactory<WindowFactory<ProgressWindow> >(WT_PROGRESS_WINDOW);
	addFactory<WindowFactory<TextureWindow> >(WT_TEXTURE_WINDOW);
	addFactory<WindowFactory<TextureAnimWindow> >(WT_TEXTURE_ANIM_WINDOW);
	addFactory<WindowFactory<TextWindow> >(WT_TEXT_WINDOW);
	addFactory<WindowFactory<VideoWindow> >(WT_VIDEO_WINDOW);
	if (mFactoryList.size() < WT_MAX)
	{
		ENGINE_ERROR("error : not all window factory registered! registered count : %d, max count : %d", mFactoryList.size(), WT_MAX);
	}
}