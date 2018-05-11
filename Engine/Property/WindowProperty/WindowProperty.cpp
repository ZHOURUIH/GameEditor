#include "LayoutUtility.h"
#include "WindowProperty.h"

WindowPropertyPosition::WindowPropertyPosition()
{
	mPropertyName = "Position";
	mDescription = "窗口的位置";
}
void WindowPropertyPosition::setRealValue(const txPoint& value, Window* window)
{
	window->setPosition(value);
}
txPoint WindowPropertyPosition::getRealValue(Window * window)
{
	return window->getPosition();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertySize::WindowPropertySize()
{
	mPropertyName = "Size";
	mDescription = "窗口的大小";
}
void WindowPropertySize::setRealValue(const txPoint& value, Window* window)
{
	window->setSize(value);
}
txPoint WindowPropertySize::getRealValue(Window * window)
{
	return window->getSize();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyTransform::WindowPropertyTransform()
{
	mPropertyName = "Transform";
	mDescription = "窗口的平移,不影响窗口的位置属性,会影响子窗口的平移";
}
void WindowPropertyTransform::setRealValue(const VECTOR3& value, Window* window)
{
	window->setMatrixTranslation(value);
}
VECTOR3 WindowPropertyTransform::getRealValue(Window * window)
{
	return window->getMatrixTranslation();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyScale::WindowPropertyScale()
{
	mPropertyName = "Scale";
	mDescription = "窗口的缩放,不会影响窗口的尺寸属性,会影响子窗口的缩放";
}
void WindowPropertyScale::setRealValue(const VECTOR2& value, Window* window)
{
	window->setMatrixScale(value);
}
VECTOR2 WindowPropertyScale::getRealValue(Window * window)
{
	return window->getMatrixScale();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyRotation::WindowPropertyRotation()
{
	mPropertyName = "Rotation";
	mDescription = "窗口的旋转,yaw,pitch,roll旋转,不会影响窗口的位置属性,会影响子窗口的旋转";
	mChildName[0] = "yaw";
	mChildName[1] = "pitch";
	mChildName[2] = "roll";
	mChildDescribe[0] = "绕Y轴旋转,弧度制";
	mChildDescribe[1] = "绕X轴旋转,弧度制";
	mChildDescribe[2] = "绕Z轴旋转,弧度制";
}
void WindowPropertyRotation::setRealValue(const VECTOR3& value, Window* window)
{
	const MATRIX3& rot = txMath::eulerAngleToMatrix3(value);
	window->setMatrixRotation(rot);
}
VECTOR3 WindowPropertyRotation::getRealValue(Window* window)
{
	const MATRIX3& rotMat = window->getMatrixRotation();
	return txMath::matrix3ToEulerAngle(rotMat);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyTransformCenter::WindowPropertyTransformCenter()
{
	mPropertyName = "TransformCenter";
	mDescription = "窗口内的变换中心,x,y是一个相对值,实际值会乘以窗口的最终宽高,z是一个绝对值";
}
void WindowPropertyTransformCenter::setRealValue(const VECTOR3& value, Window* window)
{
	window->setMatrixTransformCenter(value);
}
VECTOR3 WindowPropertyTransformCenter::getRealValue(Window* window)
{
	return window->getMatrixTransformCenter();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyPrefab::WindowPropertyPrefab()
{
	mPropertyName = "Prefab";
	mDescription = "关联的预设";
	mReadOnly = true;
}
void WindowPropertyPrefab::setRealValue(const std::string& value, Window* window)
{
	window->setPrefabName(value);
}

std::string WindowPropertyPrefab::getRealValue(Window* window)
{
	return window->getPrefabName();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyVisible::WindowPropertyVisible()
{
	mPropertyName = "Visible";
	mDescription = "设置窗口是否可见";
}
void WindowPropertyVisible::setRealValue(const bool& value, Window* window)
{
	window->setVisible(value);
}
bool WindowPropertyVisible::getRealValue(Window* window)
{
	return window->getWindowVisible();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyHandleInput::WindowPropertyHandleInput()
{
	mPropertyName = "HandleInput";
	mDescription = "设置窗口是否可以接受用户输入";
}
void WindowPropertyHandleInput::setRealValue(const bool& value, Window* window)
{
	window->setEnableHandleInput(value);
}
bool WindowPropertyHandleInput::getRealValue(Window* window)
{
	return window->getEnableHandleInput();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyAlpha::WindowPropertyAlpha()
{
	mPropertyName = "Alpha";
	mDescription = "窗口透明度";
}
void WindowPropertyAlpha::setRealValue(const float& value, Window* window)
{
	window->setAlpha(value);
}
float WindowPropertyAlpha::getRealValue(Window* window)
{
	return window->getAlpha();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyColour::WindowPropertyColour()
{
	mPropertyName = "Colour";
	mDescription = "窗口附加颜色";
	mChildName[0] = "red";
	mChildName[1] = "green";
	mChildName[2] = "blue";
}
void WindowPropertyColour::setRealValue(const VECTOR3& value, Window* window)
{
	window->setColour(value);
}
VECTOR3 WindowPropertyColour::getRealValue(Window* window)
{
	return window->getColour();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyColourBlendMode::WindowPropertyColourBlendMode()
{
	mPropertyName = "ColourBlendMode";
	mDescription = "窗口附加颜色的叠加方式";
	ADD_ENUM(BM_ADD);
	ADD_ENUM(BM_MULTI);
	ADD_ENUM(BM_REPLACE);
}
void WindowPropertyColourBlendMode::setRealValue(const BLEND_MODE& value, Window* window)
{
	window->setColourBlendMode(value);
}
BLEND_MODE WindowPropertyColourBlendMode::getRealValue(Window* window)
{
	return window->getColourBlendMode();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyHSL::WindowPropertyHSL()
{
	mPropertyName = "HSLOffset";
	mDescription = "窗口色相,饱和度,亮度偏移";
	mChildName[0] = "hue";
	mChildName[1] = "saturation";
	mChildName[2] = "lum";
	mChildDescribe[0] = "色相";
	mChildDescribe[1] = "饱和度";
	mChildDescribe[2] = "亮度";
}
void WindowPropertyHSL::setRealValue(const VECTOR3& value, Window* window)
{
	window->setHSLOffset(value);
}
VECTOR3 WindowPropertyHSL::getRealValue(Window* window)
{
	return window->getHSLOffset();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertySymmetryMode::WindowPropertySymmetryMode()
{
	mPropertyName = "SymmetryMode";
	mDescription = "窗口渲染内容对称方式";
	ADD_ENUM(SM_NONE);
	ADD_ENUM(SM_LEFT_COPY_TO_RIGHT);
	ADD_ENUM(SM_LEFT_MIRROR_TO_RIGHT);
	ADD_ENUM(SM_HORI_INVERSE);
	ADD_ENUM(SM_VERT_INVERSE);
	ADD_ENUM(SM_HORI_VERT_MIRROR);
	ADD_ENUM(SM_BOTTOM_COPY_TO_TOP);
	ADD_ENUM(SM_BOTTOM_MIRROR_TO_TOP);
}
void WindowPropertySymmetryMode::setRealValue(const SYMMETRY_MODE& value, Window* window)
{
	window->setSymmetryMode(value);
}
SYMMETRY_MODE WindowPropertySymmetryMode::getRealValue(Window* window)
{
	return window->getSymmetryMode();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyWaterEffect::WindowPropertyWaterEffect()
{
	mPropertyName = "WaterEffect";
	mDescription = "水波纹效果";
}
void WindowPropertyWaterEffect::setRealValue(const bool& value, Window* window)
{
	window->setWaterEffect(value);
}
bool WindowPropertyWaterEffect::getRealValue(Window* window)
{
	return window->getWaterEffect();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyRotateAround::WindowPropertyRotateAround()
{
	mPropertyName = "RotateAround";
	mDescription = "父窗口旋转时,是否会改变自己在父窗口中的位置,true则窗口会围绕父窗口中心旋转,false则会绕自己中心旋转";
}
void WindowPropertyRotateAround::setRealValue(const bool& value, Window* window)
{
	window->setRotateAround(value);
}
bool WindowPropertyRotateAround::getRealValue(Window* window)
{
	return window->getRotateAround();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyDiriveParentRotate::WindowPropertyDiriveParentRotate()
{
	mPropertyName = "DiriveParentRotate";
	mDescription = "窗口的旋转是否会受到父窗口旋转的影响";
}
void WindowPropertyDiriveParentRotate::setRealValue(const bool& value, Window* window)
{
	window->setDiriveParentRotation(value);
}
bool WindowPropertyDiriveParentRotate::getRealValue(Window* window)
{
	return window->getDiriveParentRotation();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyInnerChild::WindowPropertyInnerChild()
{
	mPropertyName = "InnerChild";
	mDescription = "是否是内部子窗口";
	mReadOnly = true;
}
void WindowPropertyInnerChild::setRealValue(const bool& value, Window* window)
{
	// 只读属性不作处理
}
bool WindowPropertyInnerChild::getRealValue(Window* window)
{
	return window->isInnerChild();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyAutoSizeType::WindowPropertyAutoSizeType()
{
	mPropertyName = "AutoSizeType";
	mDescription = "自动计算尺寸方式";
	ADD_ENUM(AST_NONE);
	ADD_ENUM(AST_AUTO_HEIGHT);
	ADD_ENUM(AST_AUTO_WIDTH);
}
void WindowPropertyAutoSizeType::setRealValue(const AUTO_SIZE_TYPE& value, Window* window)
{
	window->setAutoSizeType(value);
}
AUTO_SIZE_TYPE WindowPropertyAutoSizeType::getRealValue(Window* window)
{
	return window->getAutoSizeType();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyHeightDim::WindowPropertyHeightDim()
{
	mPropertyName = "HeightDim";
	mDescription = "高度计算方式";
	mChildDescribe[0] = "当自动计算大小值为AUTO_HEIGHT时,相对于宽度的高度值";
	mChildDescribe[1] = "当自动计算大小值为AUTO_HEIGHT时,额外的高度值";
}
void WindowPropertyHeightDim::setRealValue(const txDim& value, Window* window)
{
	window->setAutoHeight(value);
}
txDim WindowPropertyHeightDim::getRealValue(Window* window)
{
	return window->getAutoHeight();
}

//---------------------------------------------------------------------------------------------------------------------------------------------
WindowPropertyWidthDim::WindowPropertyWidthDim()
{
	mPropertyName = "WidthDim";
	mDescription = "宽度计算方式";
	mChildDescribe[0] = "当自动计算大小值为AUTO_WIDTH时,相对于高度的宽度值";
	mChildDescribe[1] = "当自动计算大小值为AUTO_WIDTH时,额外的宽度值";
}
void WindowPropertyWidthDim::setRealValue(const txDim& value, Window* window)
{
	window->setAutoWidth(value);
}
txDim WindowPropertyWidthDim::getRealValue(Window* window)
{
	return window->getAutoWidth();
}