#include "txEngineRoot.h"
#include "txTexture.h"
#include "txTextureManager.h"
#include "txBaseProperty.h"

txVector<std::string> txTextureProperty::mChoices;
txTextureProperty::txTextureProperty()
{
	mPropertyType = PT_TEXTURE;
	if (mChoices.size() == 0)
	{
		refreshTextureList();
	}
}
void txTextureProperty::set(const std::string& value, txPropertyReceiver* receiver)
{
	PTexture texture = NULL;
	if (value != EMPTY_STRING && value != "none")
	{
		texture = mTextureManager->createTexture(P_UI_TEXTURE_PATH + value, true, false);
		if (texture != NULL)
		{
			// 如果是在子线程中使用的该属性,则此处不能在显存中创建纹理
			texture->initTexture(!mAsync);
		}
	}
	setValue(texture, receiver);
}
std::string txTextureProperty::get(txPropertyReceiver* receiver)
{
	txTexture* texture = getValue(receiver);
	if (texture != NULL)
	{
		std::string texName = texture->getName();
		if (txStringUtility::startWith(texName, P_UI_TEXTURE_PATH))
		{
			texName = texName.substr(P_UI_TEXTURE_PATH.length(), texName.length() - P_UI_TEXTURE_PATH.length());
		}
		return texName;
	}
	return EMPTY_STRING;
}
void txTextureProperty::refreshTextureList()
{
	mChoices.clear();
	txVector<std::string> fileList;
	txFileUtility::findFiles(P_UI_TEXTURE_PATH, fileList, ".swi", false);
	int fileCount = fileList.size();
	FOR_STL(fileList, int i = 0; i < fileCount; ++i)
	{
		mChoices.push_back(txStringUtility::getFileName(fileList[i]));
	}
	END_FOR_STL(fileList);
	mChoices.push_back("none");
}