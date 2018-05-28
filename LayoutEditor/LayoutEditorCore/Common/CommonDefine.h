#ifndef _COMMON_DEFINE_H_
#define _COMMON_DEFINE_H_

#include "txMathUtility.h"

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

enum CORE_EVENT
{
	CE_LAYOUT_DESTROIED,					// 布局被销毁,参数1为布局名
	CE_LAYOUT_CLOSED,						// 关闭布局,参数1为布局名字
	CE_SELECTION_CHANGED,					// 切换了选择项
	CE_WINDOW_RENAMED,						// 参数 1原来的名字 2新名字
	CE_LAYOUT_RENAMED,						// 布局重命名,参数1是布局原来的名字,参数2时布局新的名字
	CE_WINDOW_DELETED,						// 参数 1 父窗口名字,2窗口名字
	CE_WINDOW_ADDED,						// 参数 1 父窗口名字,2窗口名字,参数3是添加的窗口在父窗口中的顺序位置
	CE_LAYOUT_ADDED,						// 参数 1 Layout名字
	CE_WINDOW_PROPERTY_CHANGED,				// 窗口属性改变,参数1是属性名,参数2是修改前的属性值,参数3是修改后的属性值,参数4是窗口名,参数5是谁去修改的窗口的属性(地址)
	CE_LAYOUT_MODIFIED,						// 布局修改状态改变,参数1是布局名,参数2为是否修改
	CE_LAYOUT_VISIBLE_CHANGED,				// 布局的显示状态改变,参数1为布局名字,参数2为修改以后的显示状态
	CE_WINDOW_POS_CHANGED,					// 窗口在布局中的位置改变,参数1是窗口名,参数2是父窗口名,参数3是改变之前的窗口位置下标,参数4是改变之后的窗口位置下标
	CE_WINDOW_PREFAB_CHANGED,				// 窗口改变了关联的预设,参数1为窗口名,参数2为改变后的预设名
	CE_UNDO_ENABLE_CHANGE,					// 是否可撤销状态改变,参数1为是否可撤销
	CE_REDO_ENABLE_CHANGE,					// 是否可重做状态改变,参数1为是否可重做
	CE_PREFAB_RESOURCE_CHANGED,				// 窗口预设资源有改变,无参数
	CE_TEMPLATE_RESOURCE_CHANGED,			// 窗口模板资源有改变,无参数
	CE_WINDOW_CUTED,						// 窗口剪切并已粘贴,参数1是剪切的窗口名
	CE_TREMBLE_MODIFIED,					// 震动关键帧修改状态改变,参数1是震动名,参数2是是否修改
	CE_LOAD_TREMBLING,						// 加载关键帧震动,参数1是震动名
	CE_NEW_TREMBLING,						// 新建关键帧震动,参数1是震动名
	CE_ADD_NODE,							// 添加一个关键帧节点,参数1是节点名
	CE_DELETE_NODE,							// 删除一个关键帧节点,参数1是节点名
	CE_SELECT_TREMBLING_NODE,				// 选择一个关键帧节点,参数1是节点名
	CE_SELECT_KEY_FRAME,					// 选择一个关键帧,参数1是关键帧对象地址
	CE_ADD_KEY_FRAME,						// 添加一个关键帧,参数1是关键帧的下标
	CE_DELETE_KEY_FRAME,					// 删除一个关键帧,参数1是关键帧的下标
	CE_WINDOW_COMPONENT_PROPERTY_CHANGED,	// 窗口组件的属性改变,参数1是组件名,参数2是属性名,参数3是修改前的属性值,参数4是修改后的属性值,参数5是窗口名,参数6是谁去修改的窗口的属性(地址)
	CE_COMPONENT_SELECTION_CHANGED,			// 组件选择项改变
	CE_LOAD_ACTION_TREE,					// 加载一个行为树,参数1是行为树的名字
	CE_NEW_ACTION_TREE,						// 新建一个行为树,参数1是行为树的名字
	CE_DELETE_ACTION_TREE,					// 删除一个行为树,参数1是行为树的名字
	CE_SELECT_ACTION_TREE,					// 选择一个行为树,参数1是行为的名字
	CE_ACTION_SELECTION_CHANGED,			// 行为选中项改变
	CE_ADD_ACTION,							// 添加一个行为,参数1是行为的名字,参数2是父节点的名字
	CE_DELETE_ACTION,						// 删除一个行为,参数1是行为的名字
	CE_ACTION_TREE_MODIFIED,				// 行为树是否被修改,参数1是行为树名字,参数2是是否修改
	CE_ERROR_LOG,							// 编辑核心向界面层发出的错误信息,参数1是错误信息内容
	CE_INFO_LOG,							// 编辑核心向界面层发出的提示信息,参数1是提示信息内容
	CE_MAX,
};

typedef int(*ModalDialog)(const std::string& caption, const std::string& info, const int& style);

const int MAX_UNDO_COUNT = 100;

#endif