#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "txEngineDefine.h"

const std::string ROLE_PATH = "Role/";
const std::string CAMERA_EFFECT_PATH = "CameraEffect/";

enum CORE_EVENT
{
	CE_EDITOR_ERROR,				// 编辑核心错误信息,参数1是错误信息
	CE_EDITOR_DELETED,				// 删除了编辑体,参数1是编辑体名字
	CE_HEIGHT_MAP_DONE,				// 场景高度图已经计算完成,参数1是场景名,参数2是计算所用的秒数
	CE_EDITOR_ADDED,				// 添加了编辑体,参数1是编辑体的名字
	CE_EDITOR_SELECTION_CHANGED,	// 编辑体选中项有改变,无参数
	CE_EDITOR_COMPONENT_DELETED,	// 编辑体删除组件,参数1是编辑体名字,参数2是组件的名字
	CE_COMPONENT_PROPERTY_CHANGED,	// 组件属性改变,参数1是属性名,参数2是属性所属组件的名字,参数3是属性原来的值,参数4是新的属性值,参数5是编辑体名,参数6是改变属性的源
	CE_COMPONENT_PROPERTY_REFRESH,	// 组件属性改变,只是通知属性有改变,但是不指定到底是什么属性改变了
	CE_COMPONENT_SELECTION_CHANGED,	// 组件选中项有改变,无参数
	CE_COMPONENT_ADDED,				// 编辑体添加了一个组件,参数1是组件拥有者的名字,参数3添加的组件名,参数4添加组件在父组件中的位置
	CE_SCENE_OPEN,					// 打开场景,参数1为场景名
	CE_SCENE_NEW,					// 新建场景,参数1为场景名
	CE_SCENE_DESTROIED,				// 关闭场景,参数1为场景名
	CE_SCENE_MODIFIED,				// 场景是否修改过,参数1为场景名,参数2为是否修改过
	CE_SCENE_RENAMED,				// 场景重命名,参数1为修改前的名字,参数2为修改后的名字
	CE_UNDO_ENABLE_CHANGE,			// 是否可撤销状态改变,参数1为是否可撤销
	CE_REDO_ENABLE_CHANGE,			// 是否可重做状态改变,参数1为是否可重做
	CE_MAX,
};

enum MODAL_DIALOG_STYLE
{
	MDS_OK,
	MDS_YES,
	MDS_NO,
	MDS_CANCEL,
	MDS_MAX,
};

enum MODAL_DIALOG_RESULT
{
	MDR_OK,
	MDR_YES,
	MDR_NO,
	MDR_CANCEL,
	MDR_MAX,
};

struct RayCallBackUserData
{
	void* mRay;
	void* mScene;
};

typedef int(*ModalDialog)(const std::string& caption, const std::string& info, int style);

const int MAX_UNDO_COUNT = 128;

#endif