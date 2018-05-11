#include "txPropertyManager.h"
#include "txProperty.h"
#include "txEngineDefine.h"
#include "txEngineLog.h"
#include "PropertyHeader.h"

txPropertyManager::~txPropertyManager()
{
	destroy();
}

void txPropertyManager::init()
{
	// 基础组件属性
	registeProperty<txComponentPropertyActive>();
	// 变换组件属性
	registeProperty<txTransformPropertyPosition>();
	registeProperty<txTransformPropertyRotation>();
	registeProperty<txTransformPropertyScale>();
	// 模型组件属性
	registeProperty<txMeshPropertyMeshPosition>();
	registeProperty<txMeshPropertyMeshRotation>();
	registeProperty<txMeshPropertyMeshScale>();
	// 静态模型组件属性
	registeProperty<txStaticMeshPropertyStaticMesh>();
	// 动态模型组件属性
	registeProperty<txDynamicMeshPropertyDynamicMesh>();
	registeProperty<txDynamicMeshPropertyAnimation>();
	registeProperty<txDynamicMeshPropertyAnimationState>();
	registeProperty<txDynamicMeshPropertyAnimationLength>();
	registeProperty<txDynamicMeshPropertyAnimationSpeed>();
	registeProperty<txDynamicMeshPropertyAnimationClip>();
	// 特效组件属性
	registeProperty<txEffectPropertyParticle>();
	// 物理组件属性
	registeProperty<txPhysicsPropertyShowAllGeometry>();
	registeProperty<txPhysicsPropertyKinematic>();
	registeProperty<txPhysicsPropertyLinearVelocity>();
	registeProperty<txPhysicsPropertyAngularVelocity>();
	registeProperty<txPhysicsPropertyConstantForce>();
	registeProperty<txPhysicsPropertyInstantForce>();
	registeProperty<txPhysicsPropertyConstantTorque>();
	registeProperty<txPhysicsPropertyInstantTorque>();
	// 碰撞体组件属性
	registeProperty<txCollisionPropertyGeometryMass>();
	registeProperty<txCollisionPropertyGeometryShowGeometry>();
	registeProperty<txCollisionPropertyGeometryRelativePosition>();
	registeProperty<txCollisionPropertyGeometryRelativeRotation>();
	// 盒子碰撞体组件属性
	registeProperty<txCollisionBoxPropertyBoxWidth>();
	registeProperty<txCollisionBoxPropertyBoxHeight>();
	registeProperty<txCollisionBoxPropertyBoxLength>();
	// 胶囊碰撞体组件属性
	registeProperty<txCollisionCapsulePropertyCenterCylinderHeight>();
	registeProperty<txCollisionCapsulePropertyHalfSphereRadius>();
	// 圆柱体碰撞体组件属性
	registeProperty<txCollisionCylinderPropertyCylinderHeight>();
	registeProperty<txCollisionCylinderPropertyBottomRadius>();
	// 球体碰撞体组件属性
	registeProperty<txCollisionSpherePropertySphereRadius>();
	// 平面碰撞体组件属性
	registeProperty<txCollisionPlanePropertyPlaneWidthX>();
	registeProperty<txCollisionPlanePropertyPlaneLengthZ>();
	// 透明组件属性
	registeProperty<txAlphaPropertyStartAlpha>();
	registeProperty<txAlphaPropertyTargetAlpha>();
	// 缩放组件属性
	registeProperty<txScalePropertyStartScale>();
	registeProperty<txScalePropertyTargetScale>();
	// 旋转组件属性
	registeProperty<txRotatePropertyTargetAngle>();
	registeProperty<txRotatePropertyStartAngle>();
	// 匀速或加速旋转的旋转组件
	registeProperty<txRotateSpeedPropertyRotateSpeed>();
	registeProperty<txRotateSpeedPropertyRotateAcceleration>();
	registeProperty<txRotateSpeedPropertyRotateSpeedPlayState>();
	// HSL组件
	registeProperty<txHSLPropertyStartHSL>();
	registeProperty<txHSLPropertyTargetHSL>();
	// 关键帧组件
	registeProperty<txKeyFramePropertyTrembling>();
	registeProperty<txKeyFramePropertyOnceLength>();
	registeProperty<txKeyFramePropertyAmplitude>();
	registeProperty<txKeyFramePropertyCurrentTime>();
	registeProperty<txKeyFramePropertyOffset>();
	registeProperty<txKeyFramePropertyCurState>();
	registeProperty<txKeyFramePropertyLoop>();
	registeProperty<txKeyFramePropertyFullOnce>();

	// 基本窗口属性
	registeProperty<WindowPropertyPosition>();
	registeProperty<WindowPropertySize>();
	registeProperty<WindowPropertyPrefab>();
	registeProperty<WindowPropertyVisible>();
	registeProperty<WindowPropertyHandleInput>();
	registeProperty<WindowPropertyAlpha>();
	registeProperty<WindowPropertyColour>();
	registeProperty<WindowPropertyColourBlendMode>();
	registeProperty<WindowPropertySymmetryMode>();
	registeProperty<WindowPropertyWaterEffect>();
	registeProperty<WindowPropertyRotation>();
	registeProperty<WindowPropertyScale>();
	registeProperty<WindowPropertyTransform>();
	registeProperty<WindowPropertyTransformCenter>();
	registeProperty<WindowPropertyHSL>();
	registeProperty<WindowPropertyRotateAround>();
	registeProperty<WindowPropertyInnerChild>();
	registeProperty<WindowPropertyAutoSizeType>();
	registeProperty<WindowPropertyHeightDim>();
	registeProperty<WindowPropertyWidthDim>();
	registeProperty<WindowPropertyDiriveParentRotate>();
	// 文本窗口属性
	registeProperty<TextWindowPropertyText>();
	registeProperty<TextWindowPropertyTextDockingPosition>();
	registeProperty<TextWindowPropertyFont>();
	registeProperty<TextWindowPropertyTextRotateWithWindow>();
	registeProperty<TextWindowPropertyTextSpace>();
	registeProperty<TextWindowPropertyMaxTextCount>();
	// 静态图片窗口属性
	registeProperty<TextureWindowPropertyTexture>();
	registeProperty<TextureWindowPropertyMaskTexture>();
	registeProperty<TextureWindowPropertyAutoSize>();
	registeProperty<TextureWindowPropertyTextureCoord>();
	registeProperty<TextureWindowPropertyTextureSize>();
	registeProperty<TextureWindowPropertyArcProgress>();
	registeProperty<TextureWindowPropertyLinearDodge>();
	// 序列帧窗口属性
	registeProperty<TextureAnimWindowPropertyTextureAnimSet>();
	registeProperty<TextureAnimWindowPropertyFrameCount>();
	registeProperty<TextureAnimWindowPropertyInterval>();
	registeProperty<TextureAnimWindowPropertyLoopMode>();
	registeProperty<TextureAnimWindowPropertyPlayState>();
	registeProperty<TextureAnimWindowPropertyStartIndex>();
	registeProperty<TextureAnimWindowPropertyEndIndex>();
	// 进度条窗口属性
	registeProperty<ProgressWindowPropertyPercent>();
	registeProperty<ProgressWindowPropertyLeftToRight>();
	// 数字窗口属性
	registeProperty<NumberWindowPropertyNumber>();
	registeProperty<NumberWindowPropertyNumberStyle>();
	registeProperty<NumberWindowPropertyNumberInterval>();
	registeProperty<NumberWindowPropertyMaxNumberCount>();
	registeProperty<NumberWindowPropertyNumberDockingPosition>();
	registeProperty<NumberWindowPropertyRotateWithWindow>();
	// 编辑框窗口属性
	registeProperty<EditBoxPropertyShowCaret>();
	registeProperty<EditBoxPropertyMaxTextLength>();
	registeProperty<EditBoxPropertyEditText>();
	// 勾选框窗口属性
	registeProperty<CheckBoxPropertyCheck>();
	// 普通按钮
	registeProperty<ButtonPropertyNormalTexture>();
	registeProperty<ButtonPropertyHoverTexture>();
	registeProperty<ButtonPropertyPushedTexture>();
	registeProperty<ButtonPropertyDisabledTexture>();
	// 视频窗口属性
	registeProperty<VideoWindowPropertyVideo>();
	registeProperty<VideoWindowPropertyPlayState>();
	registeProperty<VideoWindowPropertyVideoLoop>();
	registeProperty<VideoWindowPropertyRate>();
	registeProperty<VideoWindowPropertyVideoLength>();
	registeProperty<VideoWindowPropertyPlayPercent>();
	registeProperty<VideoWindowPropertyPlayTime>();
	registeProperty<VideoWindowPropertyVideoSize>();
	registeProperty<VideoWindowPropertyVideoFPS>();

	// 窗口基础行为
	registeProperty<ActionWindowPropertyActionWindow>();
	registeProperty<ActionWindowPropertyExecuteTime>();
	// 透明度变化
	registeProperty<ActionWindowAlphaPropertyStartAlpha>();
	registeProperty<ActionWindowAlphaPropertyTargetAlpha>();
	// hsl变化
	registeProperty<ActionWindowHSLPropertyStartHSL>();
	registeProperty<ActionWindowHSLPropertyTargetHSL>();
	// 移动
	registeProperty<ActionWindowMovePropertyStartPos>();
	registeProperty<ActionWindowMovePropertyTargetPos>();
	// 旋转
	registeProperty<ActionWindowRotatePropertyStartRotation>();
	registeProperty<ActionWindowRotatePropertyTargetRotation>();
	// 缩放
	registeProperty<ActionWindowScalePropertyStartScale>();
	registeProperty<ActionWindowScalePropertyTargetScale>();
}

void txPropertyManager::destroy()
{
	auto iter = mPropertyList.begin();
	auto iterEnd = mPropertyList.end();
	FOR_STL(mPropertyList, ; iter != iterEnd; ++iter)
	{
		auto iterProp = iter->second.begin();
		auto iterPropEnd = iter->second.end();
		FOR_STL(iter->second, ; iterProp != iterPropEnd; ++iterProp)
		{
			TRACE_DELETE(iterProp->second);
		}
		END_FOR_STL(iter->second);
	}
	END_FOR_STL(mPropertyList);
	mPropertyList.clear();
}

txProperty* txPropertyManager::getProperty(const std::string& name, const std::string& receiverType)
{
	auto iter = mPropertyList.find(receiverType);
	if (iter != mPropertyList.end())
	{
		auto iterProp = iter->second.find(name);
		if (iterProp != iter->second.end())
		{
			return iterProp->second;
		}
	}
	return NULL;
}

PROPERTY_TYPE txPropertyManager::getPropertyType(const std::string& name, const std::string& receiverType)
{
	auto iter = mPropertyList.find(receiverType);
	if (iter != mPropertyList.end())
	{
		auto iterProp = iter->second.find(name);
		if (iterProp != iter->second.end())
		{
			return iterProp->second->getType();
		}
	}
	return PT_STRING;
}

bool txPropertyManager::addPropertyToList(txProperty* pro, const std::string& receiverType)
{
	auto iter = mPropertyList.find(receiverType);
	if (iter == mPropertyList.end())
	{
		txMap<std::string, txProperty*> propList;
		propList.insert(pro->getName(), pro);
		mPropertyList.insert(receiverType, propList);
	}
	else
	{
		auto iterProp = iter->second.find(pro->getName());
		if (iterProp != iter->second.end())
		{
			ENGINE_ERROR("error : can not add property to manager! property name : %s", pro->getName().c_str());
			return false;
		}
		else
		{
			iter->second.insert(pro->getName(), pro);
		}
	}
	return true;
}