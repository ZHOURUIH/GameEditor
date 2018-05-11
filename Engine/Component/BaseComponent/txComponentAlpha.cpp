#include "txComponentHeader.h"
#include "txAlphaProperty.h"
#include "txEntity.h"

void txComponentAlpha::initProperty()
{
	txComponentKeyFrame::initProperty();
	addProperty<txAlphaPropertyStartAlpha>();
	addProperty<txAlphaPropertyTargetAlpha>();
}

void txComponentAlpha::applyKeyFrame(const txVector<float>& value)
{
	if (value.size() == 0)
	{
		return;
	}
	if (mComponentOwner != NULL)
	{
		float alpha = mStartAlpha + (mTargetAlpha - mStartAlpha) * value[0];
		// 如果是第一级组件,则影响所有的模型组件
		if (mParent == NULL)
		{
			// 取得模型组件列表,设置透明度
			auto& componentList = mComponentOwner->getComponentsByBaseType(TOSTRING(txComponentMesh));
			auto iterDynamic = componentList.begin();
			auto iterDynamicEnd = componentList.end();
			FOR_STL(componentList, ; iterDynamic != iterDynamicEnd; ++iterDynamic)
			{
				txComponentMesh* componentMesh = static_cast<txComponentMesh*>(iterDynamic->second);
				if (componentMesh->isActive() && componentMesh->getEntity() != NULL)
				{
					componentMesh->getEntity()->setAlpha(alpha);
				}
			}
			END_FOR_STL(componentList);
		}
		// 如果拥有父组件,则只影响父组件
		else
		{
			if (mParent->getBaseType() == TOSTRING(txComponentMesh))
			{
				txComponentMesh* componentMesh = static_cast<txComponentMesh*>(mParent);
				if (componentMesh->isActive() && componentMesh->getEntity() != NULL)
				{
					componentMesh->getEntity()->setAlpha(alpha);
				}
			}
		}
	}
}