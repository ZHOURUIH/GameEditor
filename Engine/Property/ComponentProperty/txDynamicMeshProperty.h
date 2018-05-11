#ifndef _TX_DYNAMIC_MESH_PROPERTY_H_
#define _TX_DYNAMIC_MESH_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentDynamicMesh)

// 动态模型
class txDynamicMeshPropertyDynamicMesh : public txComponentDynamicMeshProperty<txStringProperty, std::string>
{
public:
	txDynamicMeshPropertyDynamicMesh();
	virtual void setRealValue(const std::string& value, txComponentDynamicMesh* component);
	virtual std::string getRealValue(txComponentDynamicMesh* component);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------
// 动作
class txDynamicMeshPropertyAnimation : public txComponentDynamicMeshProperty<txStringProperty, std::string>
{
public:
	txDynamicMeshPropertyAnimation();
	virtual void setRealValue(const std::string& value, txComponentDynamicMesh* component);
	virtual std::string getRealValue(txComponentDynamicMesh* component);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------
// 动作播放状态
class txDynamicMeshPropertyAnimationState : public txComponentDynamicMeshProperty<txEnumProperty<PLAY_STATE>, PLAY_STATE>
{
public:
	txDynamicMeshPropertyAnimationState();
	virtual void setRealValue(const PLAY_STATE& value, txComponentDynamicMesh* component);
	virtual PLAY_STATE getRealValue(txComponentDynamicMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// 动作长度,first是秒数,second是帧数
class txDynamicMeshPropertyAnimationLength : public txComponentDynamicMeshProperty<txVector2Property, VECTOR2>
{
public:
	txDynamicMeshPropertyAnimationLength();
	virtual void setRealValue(const VECTOR2& value, txComponentDynamicMesh* component);
	virtual VECTOR2 getRealValue(txComponentDynamicMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// 动作播放速度
class txDynamicMeshPropertyAnimationSpeed : public txComponentDynamicMeshProperty<txFloatProperty, float>
{
public:
	txDynamicMeshPropertyAnimationSpeed();
	virtual void setRealValue(const float& value, txComponentDynamicMesh* component);
	virtual float getRealValue(txComponentDynamicMesh* component);
};

//---------------------------------------------------------------------------------------------------------
// 动作播放的起始帧和终止帧
class txDynamicMeshPropertyAnimationClip : public txComponentDynamicMeshProperty<txVector2Property, VECTOR2>
{
public:
	txDynamicMeshPropertyAnimationClip();
	virtual void setRealValue(const VECTOR2& value, txComponentDynamicMesh* component);
	virtual VECTOR2 getRealValue(txComponentDynamicMesh* component);
};

#endif