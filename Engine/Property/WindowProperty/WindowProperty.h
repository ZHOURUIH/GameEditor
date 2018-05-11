#ifndef _WINDOW_PROPERTY_H_
#define _WINDOW_PROPERTY_H_

#include "txBaseProperty.h"

#include "WindowHeader.h"

WINDOW_PROPERTY(Window);
//位置
class WindowPropertyPosition : public WindowProperty<txPointProperty, txPoint>
{
public:
	WindowPropertyPosition();
	virtual void setRealValue(const txPoint& value, Window* window);
	virtual txPoint getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
//大小
class WindowPropertySize : public WindowProperty<txPointProperty, txPoint>
{
public:
	WindowPropertySize();
	virtual void setRealValue(const txPoint& value, Window* window);
	virtual txPoint getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 平移
class WindowPropertyTransform : public WindowProperty<txVector3Property, VECTOR3>
{
public:
	WindowPropertyTransform();
	virtual void setRealValue(const VECTOR3& value, Window* window);
	virtual VECTOR3 getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 缩放
class WindowPropertyScale : public WindowProperty<txVector2Property, VECTOR2>
{
public:
	WindowPropertyScale();
	virtual void setRealValue(const VECTOR2& value, Window* window);
	virtual VECTOR2 getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 旋转
class WindowPropertyRotation : public WindowProperty<txVector3Property, VECTOR3>
{
public:
	WindowPropertyRotation();
	virtual void setRealValue(const VECTOR3& value, Window* window);
	virtual VECTOR3 getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 变换中心
class WindowPropertyTransformCenter : public WindowProperty<txVector3Property, VECTOR3>
{
public:
	WindowPropertyTransformCenter();
	virtual void setRealValue(const VECTOR3& value, Window* window);
	virtual VECTOR3 getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 关联的预设
class WindowPropertyPrefab : public WindowProperty<txStringProperty, std::string>
{
public:
	WindowPropertyPrefab();
	virtual void setRealValue(const std::string& value, Window* window);
	virtual std::string getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
//可见性
class WindowPropertyVisible : public WindowProperty<txBoolProperty, bool>
{
public:
	WindowPropertyVisible();
	virtual void setRealValue(const bool& value, Window* window);
	virtual bool getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 是否接收输入事件
class WindowPropertyHandleInput : public WindowProperty<txBoolProperty, bool>
{
public:
	WindowPropertyHandleInput();
	virtual void setRealValue(const bool& value, Window* window);
	virtual bool getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 透明度
class WindowPropertyAlpha : public WindowProperty<txFloatProperty, float>
{
public:
	WindowPropertyAlpha();
	virtual void setRealValue(const float& value, Window* window);
	virtual float getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 窗口附加颜色
class WindowPropertyColour : public WindowProperty<txVector3Property, VECTOR3>
{
public:
	WindowPropertyColour();
	virtual void setRealValue(const VECTOR3& value, Window* window);
	virtual VECTOR3 getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 窗口附加颜色的叠加方式
class WindowPropertyColourBlendMode : public WindowProperty<txEnumProperty<BLEND_MODE>, BLEND_MODE>
{
public:
	WindowPropertyColourBlendMode();
	virtual void setRealValue(const BLEND_MODE& value, Window* window);
	virtual BLEND_MODE getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 窗口色相,饱和度,亮度
class WindowPropertyHSL : public WindowProperty<txVector3Property, VECTOR3>
{
public:
	WindowPropertyHSL();
	virtual void setRealValue(const VECTOR3& value, Window* window);
	virtual VECTOR3 getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 窗口渲染内容对称方式
class WindowPropertySymmetryMode : public WindowProperty<txEnumProperty<SYMMETRY_MODE>, SYMMETRY_MODE>
{
public:
	WindowPropertySymmetryMode();
	virtual void setRealValue(const SYMMETRY_MODE& value, Window* window);
	virtual SYMMETRY_MODE getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 水波纹效果
class WindowPropertyWaterEffect : public WindowProperty<txBoolProperty, bool>
{
public:
	WindowPropertyWaterEffect();
	virtual void setRealValue(const bool& value, Window* window);
	virtual bool getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 父窗口旋转时,是否会改变自己在父窗口中的位置,true则窗口会围绕父窗口中心旋转,false则会绕自己中心旋转
class WindowPropertyRotateAround : public WindowProperty<txBoolProperty, bool>
{
public:
	WindowPropertyRotateAround();
	virtual void setRealValue(const bool& value, Window* window);
	virtual bool getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 窗口的旋转是否会受到父窗口旋转的影响
class WindowPropertyDiriveParentRotate : public WindowProperty<txBoolProperty, bool>
{
public:
	WindowPropertyDiriveParentRotate();
	virtual void setRealValue(const bool& value, Window* window);
	virtual bool getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 是否是内部子窗口
class WindowPropertyInnerChild : public WindowProperty<txBoolProperty, bool>
{
public:
	WindowPropertyInnerChild();
	virtual void setRealValue(const bool& value, Window* window);
	virtual bool getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 自动计算尺寸方式
class WindowPropertyAutoSizeType : public WindowProperty<txEnumProperty<AUTO_SIZE_TYPE>, AUTO_SIZE_TYPE>
{
public:
	WindowPropertyAutoSizeType();
	virtual void setRealValue(const AUTO_SIZE_TYPE& value, Window* window);
	virtual AUTO_SIZE_TYPE getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 高度计算方式
class WindowPropertyHeightDim : public WindowProperty<txDimProperty, txDim>
{
public:
	WindowPropertyHeightDim();
	virtual void setRealValue(const txDim& value, Window* window);
	virtual txDim getRealValue(Window* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 宽度计算方式
class WindowPropertyWidthDim : public WindowProperty<txDimProperty, txDim>
{
public:
	WindowPropertyWidthDim();
	virtual void setRealValue(const txDim& value, Window* window);
	virtual txDim getRealValue(Window* window);
};

#endif