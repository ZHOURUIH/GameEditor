#include "txEngineRoot.h"
#include "txSceneManager.h"
#include "txCamera.h"
#include "txNodeManager.h"
#include "txSkyBox.h"
#include "txTextureManager.h"
#include "txTexture.h"
#include "txEngineEventSystem.h"
#include "txScene.h"
#include "txEntityManager.h"
#include "txEntity.h"
#include "txNode.h"
#include "txMagicParticleSystem.h"
#include "txSerializer.h"
#include "txRenderWindow.h"
#include "txComponentHeader.h"

#include "GameCamera.h"
#include "EditorCoreRoot.h"
#include "CoreEventSystem.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "ECScene.h"
#include "ECGrid.h"
#include "ECAxis.h"
#include "ECUtility.h"
#include "ECEditorManager.h"
#include "ECUndoManager.h"
#include "SceneEditorCore.h"

#include "EditorFrame.h"
#include "PropertyWindow.h"
#include "RenderCanvas.h"
#include "EditorApp.h"
#include "EditorBusyInfo.h"
#include "SceneHierarchy.h"
#include "ComponentResource.h"
#include "ComponentHierarchy.h"
#include "EditorCommandHeader.h"

enum
{
	ID_TIMER,
	ID_MENU_NEW_ADVANCE_SCENE,
	ID_MENU_OPEN_FILE,
	ID_MENU_OPEN_ADVANCE_SCENE,
	ID_MENU_SAVE_ADVANCE_SCENE,
	ID_MENU_TRANS_SCENE_FILE,
	ID_MENU_TRANS_SKINMESH_FILE,
	ID_MENU_CLOSE_FILE,
	ID_MENU_EXIT,
	ID_MENU_UNDO,
	ID_MENU_REDO,
	ID_MENU_COPY,
	ID_MENU_CUT,
	ID_MENU_PASTE,
	ID_MENU_SHOW_EDITOR_GRID,
	ID_MENU_SHOW_EDITOR_AXIS,
	ID_MENU_SHOW_SKY,
	ID_MENU_SHOW_SCENE_GRID,
	ID_MENU_SHOW_SCENE,
	ID_MENU_SHOW_HEIGHTMAP,
	ID_MENU_SCENE_HIERARCHY_PANEL,
	ID_MENU_PROPERTY_PANEL,
	ID_MENU_RENDER_PANEL,
	ID_MENU_COMPONENT_RESOURCE_PANEL,
	ID_MENU_COMPONENT_HIERARCHY_PANEL,
	ID_MENU_LOAD_LAYOUT,
	ID_MENU_SAVE_LAYOUT,
	ID_MENU_CALCULATE_HEIGHTMAP,
	ID_MENU_SAVE_HEIGHT_MAP,
	ID_MENU_LOAD_HEIGHT_MAP,
	ID_MENU_SAVE_ROAD_POINT,
	ID_TOOL_UNDO,
	ID_TOOL_REDO,
	ID_TOOL_COPY,
	ID_TOOL_CUT,
	ID_TOOL_PASTE,
	ID_TOOL_NEW_EDITOR,
	ID_TOOL_SHOW_EDITOR_GRID,
	ID_TOOL_SHOW_EDITOR_AXIS,
	ID_TOOL_SHOW_SKY,
	ID_TOOL_SHOW_SCENE_GRID,
	ID_TOOL_SHOW_SCENE,
	ID_TOOL_SHOW_HEIGHTMAP,
	ID_TOOL_SCENE_HIERARCHY_PANEL,
	ID_TOOL_PROPERTY_PANEL,
	ID_TOOL_RENDER_CANVAS,
	ID_TOOL_COMPONENT_RESOURCE_PANEL,
	ID_TOOL_COMPONENT_HIERARCHY_PANEL,
	ID_TOOL_RESET_CAMERA,
	ID_TOOL_VERTICAL_CAMERA,
	ID_TOOL_CAMERA_MOVE_SPEED,
	ID_TOOL_CAMERA_ROTATE_SPEED,
	ID_TOOL_SCENE_POS_X,
	ID_TOOL_SCENE_POS_Z,
	ID_TOOL_SCENE_MOVE_SPEED,
	ID_TOOL_BACKGROUND_COLOUR,
};

BEGIN_EVENT_TABLE(EditorFrame, wxFrame)

EVT_TIMER(ID_TIMER, OnTimer)
EVT_CLOSE(OnCloseWindow)
EVT_MENU(ID_MENU_NEW_ADVANCE_SCENE, OnNewAdvanceScene)
EVT_MENU(ID_MENU_OPEN_FILE, OnOpenScene)
EVT_MENU(ID_MENU_OPEN_ADVANCE_SCENE, OnOpenAdvanceScene)
EVT_MENU(ID_MENU_SAVE_ADVANCE_SCENE, OnSaveAdvanceScene)
EVT_MENU(ID_MENU_TRANS_SCENE_FILE, OnTransSceneFile)
EVT_MENU(ID_MENU_TRANS_SKINMESH_FILE, OnTransSkinMeshFile)
EVT_MENU(ID_MENU_CLOSE_FILE, OnCloseFile)
EVT_MENU(ID_MENU_EXIT, OnExit)
EVT_MENU(ID_MENU_UNDO, OnUndo)
EVT_MENU(ID_MENU_REDO, OnRedo)
EVT_MENU(ID_MENU_COPY, OnCopy)
EVT_MENU(ID_MENU_CUT, OnCut)
EVT_MENU(ID_MENU_PASTE, OnPaste)
EVT_MENU(ID_MENU_SHOW_EDITOR_GRID, OnShowEditorGrid)
EVT_MENU(ID_MENU_SHOW_EDITOR_AXIS, OnShowEditorAxis)
EVT_MENU(ID_MENU_SHOW_SKY, OnShowSky)
EVT_MENU(ID_MENU_SHOW_SCENE_GRID, OnShowSceneGrid)
EVT_MENU(ID_MENU_SHOW_SCENE, OnShowScene)
EVT_MENU(ID_MENU_SHOW_HEIGHTMAP, OnShowHeightMap)
EVT_MENU(ID_MENU_SCENE_HIERARCHY_PANEL, OnSceneHierarchyPanel)
EVT_MENU(ID_MENU_PROPERTY_PANEL, OnPropertyPanel)
EVT_MENU(ID_MENU_RENDER_PANEL, OnRenderCanvas)
EVT_MENU(ID_MENU_COMPONENT_RESOURCE_PANEL, OnComponentResourcePanel)
EVT_MENU(ID_MENU_COMPONENT_HIERARCHY_PANEL, OnComponentHierarchyPanel)
EVT_MENU(ID_MENU_LOAD_LAYOUT, OnLoadLayout)
EVT_MENU(ID_MENU_SAVE_LAYOUT, OnSaveLayout)
EVT_MENU(ID_MENU_CALCULATE_HEIGHTMAP, OnCalculateHeightMap)
EVT_MENU(ID_MENU_SAVE_HEIGHT_MAP, OnSaveHeightMap)
EVT_MENU(ID_MENU_SAVE_ROAD_POINT, OnSaveRoadPoint)
EVT_MENU(ID_MENU_LOAD_HEIGHT_MAP, OnLoadHeightMap)
EVT_TOOL(ID_TOOL_UNDO, OnUndo)
EVT_TOOL(ID_TOOL_REDO, OnRedo)
EVT_TOOL(ID_TOOL_COPY, OnCopy)
EVT_TOOL(ID_TOOL_CUT, OnCut)
EVT_TOOL(ID_TOOL_PASTE, OnPaste)
EVT_TOOL(ID_TOOL_NEW_EDITOR, OnNewEditor)
EVT_TOOL(ID_TOOL_SHOW_EDITOR_GRID, OnShowEditorGrid)
EVT_TOOL(ID_TOOL_SHOW_EDITOR_AXIS, OnShowEditorAxis)
EVT_TOOL(ID_TOOL_SHOW_SKY, OnShowSky)
EVT_TOOL(ID_TOOL_SHOW_SCENE_GRID, OnShowSceneGrid)
EVT_TOOL(ID_TOOL_SHOW_SCENE, OnShowScene)
EVT_TOOL(ID_TOOL_SHOW_HEIGHTMAP, OnShowHeightMap)
EVT_TOOL(ID_TOOL_SCENE_HIERARCHY_PANEL, OnSceneHierarchyPanel)
EVT_TOOL(ID_TOOL_PROPERTY_PANEL, OnPropertyPanel)
EVT_TOOL(ID_TOOL_RENDER_CANVAS, OnRenderCanvas)
EVT_TOOL(ID_TOOL_COMPONENT_RESOURCE_PANEL, OnComponentResourcePanel)
EVT_TOOL(ID_TOOL_COMPONENT_HIERARCHY_PANEL, OnComponentHierarchyPanel)
EVT_TOOL(ID_TOOL_RESET_CAMERA, OnResetCamera)
EVT_TOOL(ID_TOOL_VERTICAL_CAMERA, OnCameraVerticalDown)
EVT_TOOL(ID_TOOL_SCENE_POS_X, OnSceneX)
EVT_TOOL(ID_TOOL_SCENE_POS_Z, OnSceneZ)
EVT_TEXT_ENTER(ID_TOOL_SCENE_MOVE_SPEED, OnSceneMoveSpeed)
EVT_SLIDER(ID_TOOL_CAMERA_MOVE_SPEED, OnCameraMoveSpeed)
EVT_SLIDER(ID_TOOL_CAMERA_ROTATE_SPEED, OnCameraRotateSpeed)
EVT_COLOURPICKER_CHANGED(ID_TOOL_BACKGROUND_COLOUR, OnBackgroundColour)

END_EVENT_TABLE()

EditorFrame::EditorFrame(const wxString& title, const wxSize& size)
:
wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size),
txCommandReceiver("EditorFrame"),
mEditorCoreRoot(NULL)
{
	;
}

EditorFrame::~EditorFrame()
{
	wxGetApp().destroyBusyInfo();
}

void EditorFrame::init()
{
	setup();
	mTimer = TRACE_NEW(wxTimer, mTimer);
	mTimer->Start(20);
	mTimer->SetOwner(this, ID_TIMER);
}

void EditorFrame::destroy()
{
	TRACE_DELETE(mTimer);
	unregisterEditorCoreEvent();
	unregisterEngineEventHandler();
	TRACE_DELETE(mEditorCoreRoot);
	mAuiManager.UnInit();
}

void EditorFrame::registerEngineEventHandler()
{
	mEngineEventSystem->registeAllEvent(this);
}

void EditorFrame::unregisterEngineEventHandler()
{
	mEngineEventSystem->unregisteAllEvent(this);
}

void EditorFrame::registerEditorCoreEvent()
{
	mCoreEventSystem->registeAllEvent(this);
}

void EditorFrame::unregisterEditorCoreEvent()
{
	mCoreEventSystem->unregisteAllEvent(this);
}

void EditorFrame::setup()
{
	mAuiManager.SetManagedWindow(this);
	// 创建菜单栏
	createMenu();
	// 创建工具栏
	createToolBar();
	// 创建渲染窗口,因为在创建编辑核心时需要一个渲染窗口的句柄
	createRenderWindow();
	// 创建编辑核心
	createEditorCore();
	// 创建各个子窗口
	createWindows();
	// 创建底部的状态显示栏
	createStatuBar();
	// 创建完后刷新一遍全部控件的选中状态
	refreshAllMenuToolCheckState();
	// 刷新全部资源列表
	refreshAllResource();
}

void EditorFrame::createMenu()
{
	wxMenuBar* menuBar = new wxMenuBar();

	// 文件菜单
	mFileMenu = new wxMenu();
	mFileMenu->Append(ID_MENU_NEW_ADVANCE_SCENE, wxT("新建高级场景\tCtrl-N"));
	mFileMenu->Append(ID_MENU_OPEN_ADVANCE_SCENE, wxT("打开高级场景\tCtrl-O"));
	mFileMenu->Append(ID_MENU_SAVE_ADVANCE_SCENE, wxT("保存高级场景\tCtrl-S"));
	mFileMenu->Append(ID_MENU_OPEN_FILE, wxT("打开文件\tCtrl-Shift-O"));
	mFileMenu->Append(ID_MENU_TRANS_SCENE_FILE, wxT("转换场景文件"));
	mFileMenu->Append(ID_MENU_TRANS_SKINMESH_FILE, wxT("转换动态模型文件"));
	mFileMenu->Append(ID_MENU_CLOSE_FILE, wxT("关闭文件"));
	mFileMenu->Append(ID_MENU_EXIT, wxT("退出"));
	menuBar->Append(mFileMenu, wxT("文件"));

	// 编辑菜单
	mEditMenu = new wxMenu();
	mEditMenu->Append(ID_MENU_UNDO, wxT("撤销\tCtrl-Z"));
	mEditMenu->Append(ID_MENU_REDO, wxT("重做\tCtrl-Y"));
	mEditMenu->Append(ID_MENU_COPY, wxT("复制\tCtrl-C"));
	mEditMenu->Append(ID_MENU_CUT, wxT("剪切\tCtrl-X"));
	mEditMenu->Append(ID_MENU_PASTE, wxT("粘贴\tCtrl-V"));
	menuBar->Append(mEditMenu, wxT("编辑"));

	// 工具菜单
	mToolMenu = new wxMenu();
	mToolMenu->Append(ID_MENU_SHOW_EDITOR_GRID, wxT("显示/隐藏编辑体网格"), wxT("显示/隐藏编辑体网格"), wxITEM_CHECK);
	mToolMenu->Append(ID_MENU_SHOW_EDITOR_AXIS, wxT("显示/隐藏编辑体坐标轴"), wxT("显示/隐藏编辑体坐标轴"), wxITEM_CHECK);
	mToolMenu->Append(ID_MENU_SHOW_SKY, wxT("显示/隐藏天空"), wxT("显示/隐藏天空"), wxITEM_CHECK);
	mToolMenu->Append(ID_MENU_SHOW_SCENE_GRID, wxT("显示/隐藏场景网格"), wxT("显示/隐藏场景网格"), wxITEM_CHECK);
	mToolMenu->Append(ID_MENU_SHOW_SCENE, wxT("显示/隐藏场景"), wxT("显示/隐藏场景"), wxITEM_CHECK);
	mToolMenu->Append(ID_MENU_SHOW_HEIGHTMAP, wxT("显示/隐藏高度图"), wxT("显示/隐藏高度图"), wxITEM_CHECK);
	mToolMenu->Append(ID_MENU_CALCULATE_HEIGHTMAP, wxT("计算场景高度图"), wxT("计算场景高度图"), wxITEM_NORMAL);
	mToolMenu->Append(ID_MENU_SAVE_HEIGHT_MAP, wxT("保存场景高度图"), wxT("保存场景高度图"), wxITEM_NORMAL);
	mToolMenu->Append(ID_MENU_LOAD_HEIGHT_MAP, wxT("加载场景高度图"), wxT("加载场景高度图"), wxITEM_NORMAL);
	mToolMenu->Append(ID_MENU_SAVE_ROAD_POINT, wxT("保存场景路点"), wxT("保存场景路点"), wxITEM_NORMAL);
	menuBar->Append(mToolMenu, wxT("工具"));

	// 窗口菜单
	mWindowMenu = new wxMenu();
	mWindowMenu->Append(ID_MENU_SCENE_HIERARCHY_PANEL, wxT("场景视图"), wxT("场景视图"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_PROPERTY_PANEL, wxT("属性窗口"), wxT("属性窗口"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_RENDER_PANEL, wxT("渲染窗口"), wxT("渲染窗口"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_COMPONENT_RESOURCE_PANEL, wxT("组件资源"), wxT("组件资源"), wxITEM_CHECK);
	mWindowMenu->Append(ID_MENU_COMPONENT_HIERARCHY_PANEL, wxT("组件视图"), wxT("组件视图"), wxITEM_CHECK);
	menuBar->Append(mWindowMenu, wxT("窗口"));

	// 布局菜单
	mLayoutMenu = new wxMenu(); 
	mLayoutMenu->Append(ID_MENU_LOAD_LAYOUT, wxT("加载布局"));
	mLayoutMenu->Append(ID_MENU_SAVE_LAYOUT, wxT("保存布局"));
	menuBar->Append(mLayoutMenu, wxT("布局"));

	SetMenuBar(menuBar);
}

void EditorFrame::createToolBar()
{
	// 编辑工具栏
	mEditToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mEditToolBar->SetToolBitmapSize(wxSize(16, 16));
	mEditToolBar->AddTool(ID_TOOL_UNDO, wxT("撤销"), wxBITMAP(IDB_UNDO));
	mEditToolBar->AddTool(ID_TOOL_REDO, wxT("重做"), wxBITMAP(IDB_REDO));
	mEditToolBar->AddTool(ID_TOOL_COPY, wxT("复制"), wxBITMAP(IDB_COPY));
	mEditToolBar->AddTool(ID_TOOL_CUT, wxT("剪切"), wxBITMAP(IDB_CUT));
	mEditToolBar->AddTool(ID_TOOL_PASTE, wxT("粘贴"), wxBITMAP(IDB_PASTE));
	mEditToolBar->AddTool(ID_TOOL_NEW_EDITOR, wxT("新建物体"), wxBITMAP(IDB_NEW_EDITOR));
	mEditToolBar->Realize();
	mAuiManager.AddPane(mEditToolBar, wxAuiPaneInfo().Name(wxT("EditToolBar")).Caption(wxT("编辑工具栏")).ToolbarPane().Top());

	// 场景显示隐藏工具栏
	mShowHideToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mShowHideToolBar->SetToolBitmapSize(wxSize(16, 16));
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_EDITOR_GRID, wxT("显示/隐藏编辑体网格"), wxBITMAP(IDB_SHOW_EDITOR_GRID), wxT("显示/隐藏编辑体网格"), wxITEM_CHECK);
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_EDITOR_AXIS, wxT("显示/隐藏编辑体坐标轴"), wxBITMAP(IDB_SHOW_EDITOR_AXIS), wxT("显示/隐藏编辑体坐标轴"), wxITEM_CHECK);
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_SKY, wxT("显示/隐藏天空"), wxBITMAP(IDB_SHOW_SKY), wxT("显示/隐藏天空"), wxITEM_CHECK);
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_SCENE_GRID, wxT("显示/隐藏场景网格"), wxBITMAP(IDB_SHOW_SCENE_GRID), wxT("显示/隐藏场景网格"), wxITEM_CHECK);
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_SCENE, wxT("显示/隐藏场景"), wxBITMAP(IDB_SHOW_SCENE), wxT("显示/隐藏场景"), wxITEM_CHECK);
	mShowHideToolBar->AddTool(ID_TOOL_SHOW_HEIGHTMAP, wxT("显示/隐藏场景高度图"), wxBITMAP(IDB_SHOW_HEIGHT_MAP), wxT("显示/隐藏场景高度图"), wxITEM_CHECK);
	mShowHideToolBar->Realize();
	mAuiManager.AddPane(mShowHideToolBar, wxAuiPaneInfo().Name(wxT("ShowToolBar")).Caption(wxT("显示工具栏")).ToolbarPane().Top());

	// 面板窗口工具栏
	mWindowToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mWindowToolBar->SetToolBitmapSize(wxSize(16, 16));
	mWindowToolBar->AddTool(ID_TOOL_SCENE_HIERARCHY_PANEL, wxT("场景视图"), wxBITMAP(IDB_SCENE_HIERARCHY_PANEL), wxT("场景视图"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_PROPERTY_PANEL, wxT("属性窗口"), wxBITMAP(IDB_PROPERTY_PANEL), wxT("属性窗口"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_RENDER_CANVAS, wxT("渲染窗口"), wxBITMAP(IDB_RENDER_PANEL), wxT("渲染窗口"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_COMPONENT_RESOURCE_PANEL, wxT("组件资源"), wxBITMAP(IDB_COMPONENT_RESOURCE_PANEL), wxT("组件资源"), wxITEM_CHECK);
	mWindowToolBar->AddTool(ID_TOOL_COMPONENT_HIERARCHY_PANEL, wxT("组件视图"), wxBITMAP(IDB_COMPONENT_HIERARCHY_PANEL), wxT("组件视图"), wxITEM_CHECK);
	mWindowToolBar->Realize();
	mAuiManager.AddPane(mWindowToolBar, wxAuiPaneInfo().Name(wxT("WindowToolBar")).Caption(wxT("窗口工具栏")).ToolbarPane().Top());

	// 摄像机工具栏
	mCameraToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mCameraToolBar->SetToolBitmapSize(wxSize(16, 16));

	wxSlider* cameraMoveSlider = new wxSlider(mCameraToolBar, ID_TOOL_CAMERA_MOVE_SPEED, 20, 0, 100);
	mCameraToolBar->AddLabel(wxID_ANY, wxT("摄像机移动速度"));
	mCameraToolBar->AddControl(cameraMoveSlider, wxT("摄像机移动速度"));

	wxSlider* cameraRotateSlider = new wxSlider(mCameraToolBar, ID_TOOL_CAMERA_ROTATE_SPEED, 10, 0, 100);
	mCameraToolBar->AddLabel(wxID_ANY, wxT("摄像机旋转速度"));
	mCameraToolBar->AddControl(cameraRotateSlider, wxT("摄像机旋转速度"));

	mCameraToolBar->AddTool(ID_TOOL_RESET_CAMERA, wxT("重置摄像机"), wxBITMAP(IDB_RESET_CAMERA), wxT("重置摄像机"), wxITEM_NORMAL);
	mCameraToolBar->AddTool(ID_TOOL_VERTICAL_CAMERA, wxT("垂直"), wxBITMAP(IDB_CUT));
	mCameraToolBar->Realize();
	mAuiManager.AddPane(mCameraToolBar, wxAuiPaneInfo().Name(wxT("CameraToolBar")).Caption(wxT("摄像机工具栏")).ToolbarPane().Top());

	// 场景工具栏
	mSceneToolBar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_HORIZONTAL);
	mSceneToolBar->SetToolBitmapSize(wxSize(16, 16));
	mSceneToolBar->AddTool(ID_TOOL_SCENE_POS_X, wxT("场景X"), wxBITMAP(IDB_UNDO));
	mSceneToolBar->AddTool(ID_TOOL_SCENE_POS_Z, wxT("场景Z"), wxBITMAP(IDB_REDO));
	mSceneToolBar->AddLabel(wxID_ANY, wxT("场景移动速度"));
	mSceneMoveText = new wxTextCtrl(mSceneToolBar, ID_TOOL_SCENE_MOVE_SPEED);
	mSceneMoveText->SetValue(wxT("1"));
	mSceneToolBar->AddControl(mSceneMoveText, wxT("场景移动速度"));
	wxColourPickerCtrl* backgroundColour = new wxColourPickerCtrl(mSceneToolBar, ID_TOOL_BACKGROUND_COLOUR, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	mSceneToolBar->AddControl(backgroundColour);
	mSceneToolBar->Realize();
	mAuiManager.AddPane(mSceneToolBar, wxAuiPaneInfo().Name(wxT("SceneToolBar")).Caption(wxT("场景工具栏")).ToolbarPane().Top());

	mAuiManager.Update();
}

void EditorFrame::createRenderWindow()
{
	mRenderCanvas = new RenderCanvas(this, wxPoint(-1, -1), wxSize(-1, -1), wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE | wxMAXIMIZE, wxT(TOSTRING(RenderCanvas)));
	mAuiManager.AddPane(mRenderCanvas, wxAuiPaneInfo().Name(TOSTRING(RenderCanvas)).Center());
	mRenderCanvas->Show();

	mAuiManager.Update();

	mWindowIDList.insert(mRenderCanvas, std::make_pair(ID_MENU_RENDER_PANEL, ID_TOOL_RENDER_CANVAS));
	mWindowList.insert(mRenderCanvas->GetName().ToStdString(), mRenderCanvas);
}

#define NEW_PANEL(type, panel, title, menuID, toolID, size, floatingSize, dock)\
	panel = new type(this, wxDefaultPosition, size, wxMINIMIZE | wxMAXIMIZE, wxT(TOSTRING(type)));\
	mAuiManager.AddPane(panel, wxAuiPaneInfo().Name(panel->GetName()).BestSize(wxSize(-1, -1)).FloatingSize(floatingSize).Caption(wxT(title)).##dock##().Dockable(true));\
	mWindowIDList.insert(panel, std::make_pair(menuID, toolID));\
	mWindowList.insert(panel->GetName().ToStdString(), panel);

#define NEW_PANEL_LEFT(type, title, menuID, toolID) NEW_PANEL(type, m##type, title, menuID, toolID, wxSize(400, -1), wxSize(300, 800), Left)
#define NEW_PANEL_RIGHT(type, title, menuID, toolID) NEW_PANEL(type, m##type, title, menuID, toolID, wxSize(400, -1), wxSize(300, 800), Right)
#define NEW_PANEL_BOTTOM(type, title, menuID, toolID) NEW_PANEL(type, m##type, title, menuID, toolID, wxSize(-1, 400), wxSize(1200, 400), Bottom)

void EditorFrame::createWindows()
{
	NEW_PANEL_LEFT(SceneHierarchy, "场景视图", ID_MENU_SCENE_HIERARCHY_PANEL, ID_TOOL_SCENE_HIERARCHY_PANEL);
	NEW_PANEL_LEFT(ComponentResource, "组件资源", ID_MENU_COMPONENT_RESOURCE_PANEL, ID_TOOL_COMPONENT_RESOURCE_PANEL);
	NEW_PANEL_RIGHT(ComponentHierarchy, "组件视图", ID_MENU_COMPONENT_HIERARCHY_PANEL, ID_TOOL_COMPONENT_HIERARCHY_PANEL);
	NEW_PANEL_RIGHT(PropertyWindow, "属性视图", ID_MENU_PROPERTY_PANEL, ID_TOOL_PROPERTY_PANEL);

	EditorBase::notifyConstructDone();

	mAuiManager.Update();
}

void EditorFrame::createStatuBar()
{
	CreateStatusBar(2);
	SetStatusText("camera pos : 0.0, 0.0, 0.0", 0);
	SetStatusText("camera dir : 0.0, 0.0, 0.0", 1);
}

void EditorFrame::updateStatus()
{
	GameCamera* camera = mCameraManager->getMainCamera();
	if (camera != NULL)
	{
		const VECTOR3& pos = camera->getPosition();
		char buffer[256];
		SPRINTF(buffer, 256, "camera pos : %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
		SetStatusText(buffer, 0);

		VECTOR3 dir = camera->getDirection();
		SPRINTF(buffer, 256, "camera dir : %.3f, %.3f, %.3f", dir.x, dir.y, dir.z);
		SetStatusText(buffer, 1);
	}
}

void EditorFrame::createEditorCore()
{
	mEditorCoreRoot = TRACE_NEW(EditorCoreRoot, mEditorCoreRoot);
	mEditorCoreRoot->init(mRenderCanvas->GetHWND(), 32, mRenderCanvas->GetScreenRect().GetWidth(), mRenderCanvas->GetScreenRect().GetHeight());

	// 注册引擎事件接收者
	registerEngineEventHandler();
	mEngineRoot->notifyInitDone();

	// 注册编辑核心事件接收者
	registerEditorCoreEvent();
	mEditorCoreRoot->notifyInitDone();
}

void EditorFrame::refreshAllResource()
{
	mComponentResource->refreshList();
}

void EditorFrame::saveRoadPoint(const std::string& fullPathh, txMap<int, txVector<VECTOR3> >& roadPoint)
{
	auto itr = roadPoint.begin();
	auto itrEnd = roadPoint.end();
	txSerializer serializer;
	serializer.write((int)roadPoint.size());
	FOR_STL(roadPoint, ; itr != itrEnd; ++itr)
	{
		int pointSize = itr->second.size();
		serializer.write(pointSize);
		FOR_STL(itr->second, int i = 0; i < pointSize; ++i)
		{
			serializer.writeVector3(itr->second[i]);
		}
		END_FOR_STL(itr->second);
	}
	END_FOR_STL(roadPoint);

	serializer.writeToFile(fullPathh);
}

void EditorFrame::refreshAllMenuToolCheckState()
{
	// 可选中菜单和可选中工具按钮的选中状态
	ECScene* curScene = mSceneEditorCore->getCurScene();
	if (curScene != NULL)
	{
		bool showEditorAxis = curScene->getEditorAxis()->getAutoShow();
		bool showEditorGrid = curScene->getEditorGrid()->getAutoShow();
		bool showSky = true;
		bool showSceneGrid = curScene->getSceneGrid()->isVisible();
		bool showScene = curScene->getSceneVisible();
		
		mToolMenu->Check(ID_MENU_SHOW_EDITOR_AXIS, showEditorAxis);
		mToolMenu->Check(ID_MENU_SHOW_EDITOR_GRID, showEditorGrid);
		mToolMenu->Check(ID_MENU_SHOW_SKY, showSky);
		mToolMenu->Check(ID_MENU_SHOW_SCENE_GRID, showSceneGrid);
		mToolMenu->Check(ID_MENU_SHOW_SCENE, showScene);

		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_EDITOR_AXIS, showEditorAxis);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_EDITOR_GRID, showEditorGrid);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_SKY, showSky);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_SCENE_GRID, showSceneGrid);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_SCENE, showScene);
	}
	else
	{
		mToolMenu->Check(ID_MENU_SHOW_EDITOR_AXIS, false);
		mToolMenu->Check(ID_MENU_SHOW_EDITOR_GRID, false);
		mToolMenu->Check(ID_MENU_SHOW_SKY, false);
		mToolMenu->Check(ID_MENU_SHOW_SCENE_GRID, false);
		mToolMenu->Check(ID_MENU_SHOW_SCENE, false);

		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_EDITOR_AXIS, false);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_EDITOR_GRID, false);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_SKY, false);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_SCENE_GRID, false);
		mShowHideToolBar->ToggleTool(ID_TOOL_SHOW_SCENE, false);
	}
	
	// 各个窗口的显示状态
	auto iterWindow = mWindowIDList.begin();
	auto iterWindowEnd = mWindowIDList.end();
	FOR_STL(mWindowIDList, ; iterWindow != iterWindowEnd; ++iterWindow)
	{
		bool check = iterWindow->first->IsShown();
		mWindowMenu->Check(iterWindow->second.first, check);
		mWindowToolBar->ToggleTool(iterWindow->second.second, check);
	}
	END_FOR_STL(mWindowIDList);

	// 摄像机移动速度和旋转速度
	wxSlider* cameraMoveSpeedSlider = static_cast<wxSlider*>(mCameraToolBar->FindControl(ID_TOOL_CAMERA_MOVE_SPEED));
	cameraMoveSpeedSlider->SetValue(mCameraManager->getMainCamera()->getMoveSpeed());
	wxSlider* cameraRotateSpeedSlider = static_cast<wxSlider*>(mCameraToolBar->FindControl(ID_TOOL_CAMERA_ROTATE_SPEED));
	cameraRotateSpeedSlider->SetValue(mCameraManager->getMainCamera()->getRotateSpeed() * 10.0f);

	// 场景背景颜色
	wxColourPickerCtrl* colourPicker = static_cast<wxColourPickerCtrl*>(mSceneToolBar->FindControl(ID_TOOL_BACKGROUND_COLOUR));
	const VECTOR4& backgroundColour = mEditorCoreRoot->getEngineRoot()->getRenderWindow()->getBackgroundColour();
	wxColour colour(backgroundColour.r * 255, backgroundColour.g * 255, backgroundColour.b * 255, backgroundColour.a * 255);
	colourPicker->SetColour(colour);

	mAuiManager.Update();
}

void EditorFrame::OnTimer(wxTimerEvent& event)
{
	static unsigned long lastTime = timeGetTime();
	unsigned long curTime = timeGetTime();
	float elapsedTime = (curTime - lastTime) / 1000.0f;
	Update(elapsedTime);
	lastTime = curTime;
	Render();
}

void EditorFrame::Update(float elapsedTime)
{
	if (mEditorCoreRoot != NULL)
	{
		KeyProcess();

		// 更新编辑核心
		mEditorCoreRoot->preUpdate(elapsedTime);
		mEditorCoreRoot->update(elapsedTime);
		mEditorCoreRoot->lateUpdate(elapsedTime);

		updateStatus();
	}
}

void EditorFrame::Render()
{
	if (mEditorCoreRoot != NULL)
	{
		mEditorCoreRoot->render();
	}
}

void EditorFrame::KeyProcess()
{
	GameCamera* camera = mCameraManager->getMainCamera();
	if (camera != NULL)
	{
		if (mInputSystem->isKeyDown(VK_OEM_PLUS))
		{
			wxSlider* cameraSpeedSlider = static_cast<wxSlider*>(mCameraToolBar->FindControl(ID_TOOL_CAMERA_MOVE_SPEED));
			int curValue = cameraSpeedSlider->GetValue() + 1;
			if (curValue > cameraSpeedSlider->GetMax())
			{
				curValue = cameraSpeedSlider->GetMax();
			}
			cameraSpeedSlider->SetValue(curValue);
			camera->setMoveSpeed(curValue);
		}

		if (mInputSystem->isKeyDown(VK_OEM_MINUS))
		{
			wxSlider* cameraSpeedSlider = static_cast<wxSlider*>(mCameraToolBar->FindControl(ID_TOOL_CAMERA_MOVE_SPEED));
			int curValue = cameraSpeedSlider->GetValue() - 1;
			if (curValue < cameraSpeedSlider->GetMin())
			{
				curValue = cameraSpeedSlider->GetMin();
			}
			cameraSpeedSlider->SetValue(curValue);
			camera->setMoveSpeed(curValue);
		}
		if (mInputSystem->isKeyDown(VK_OEM_6))
		{
			wxSlider* cameraRotateSpeedSlider = static_cast<wxSlider*>(mCameraToolBar->FindControl(ID_TOOL_CAMERA_ROTATE_SPEED));
			int curValue = cameraRotateSpeedSlider->GetValue() + 1;
			if (curValue > cameraRotateSpeedSlider->GetMax())
			{
				curValue = cameraRotateSpeedSlider->GetMax();
			}
			cameraRotateSpeedSlider->SetValue(curValue);
			camera->setRotateSpeed(curValue / 10.0f);
		}

		if (mInputSystem->isKeyDown(VK_OEM_4))
		{
			wxSlider* cameraRotateSpeedSlider = static_cast<wxSlider*>(mCameraToolBar->FindControl(ID_TOOL_CAMERA_ROTATE_SPEED));
			int curValue = cameraRotateSpeedSlider->GetValue() - 1;
			if (curValue < cameraRotateSpeedSlider->GetMin())
			{
				curValue = cameraRotateSpeedSlider->GetMin();
			}
			cameraRotateSpeedSlider->SetValue(curValue);
			camera->setRotateSpeed(curValue / 10.0f);
		}
	}
}

void EditorFrame::enableUndo(const bool& enable)
{
	mEditToolBar->EnableTool(ID_TOOL_UNDO, enable);
	mEditMenu->Enable(ID_MENU_UNDO, enable);
	mAuiManager.Update();
}

void EditorFrame::enableRedo(const bool& enable)
{
	mEditToolBar->EnableTool(ID_TOOL_REDO, enable);
	mEditMenu->Enable(ID_MENU_REDO, enable);
	mAuiManager.Update();
}

void EditorFrame::OnNewAdvanceScene(wxCommandEvent& event)
{
	mSceneEditorCore->newScene("new scene");
	// 新建场景后显示场景视图
	mSceneHierarchy->showScene(mSceneEditorCore->getCurScene());
	refreshAllMenuToolCheckState();
}

void EditorFrame::OnOpenScene(wxCommandEvent& event)
{
	wxFileDialog dialog(this, wxT("打开文件"), wxT("../media"), "", "场景(*.scene)|*.scene|场景(*.json)|*.json");
	if (dialog.ShowModal() != wxID_OK)
	{
		return;
	}
	std::string fileName = dialog.GetFilename().ToStdString();
	std::string suffix = txStringUtility::getFileSuffix(fileName);
	fileName = txStringUtility::getFileNameNoSuffix(fileName);

	mSceneEditorCore->openScene(fileName, suffix == "scene");
	// 打开场景后显示场景视图
	mSceneHierarchy->showScene(mSceneEditorCore->getCurScene());
	refreshAllMenuToolCheckState();
}

void EditorFrame::OnOpenAdvanceScene(wxCommandEvent& event)
{
	wxFileDialog dialog(this, wxT("打开文件"), wxT("../media"), "", "场景(*.Ascene)|*.Ascene");
	if (dialog.ShowModal() != wxID_OK)
	{
		return;
	}
	std::string fileName = dialog.GetFilename().ToStdString();
	std::string suffix = txStringUtility::getFileSuffix(fileName);
	fileName = txStringUtility::getFileNameNoSuffix(fileName);
	mSceneEditorCore->openAdvanceScene(fileName);
	// 打开场景后显示场景视图
	mSceneHierarchy->showScene(mSceneEditorCore->getCurScene());
	refreshAllMenuToolCheckState();
}

void EditorFrame::OnSaveAdvanceScene(wxCommandEvent& event)
{
	mSceneEditorCore->saveAdvanceScene();
}

void EditorFrame::OnTransSceneFile(wxCommandEvent& event)
{
	wxFileDialog dialog(this, wxT("打开文件"), wxT("../media"), "", "场景(*.json)|*.json");
	if (dialog.ShowModal() != wxID_OK)
	{
		return;
	}
	wxString fileName = dialog.GetFilename();
	wxString path = dialog.GetPath();
	wxGetApp().getBusyInfo()->show();
	wxGetApp().getBusyInfo()->setBusyInfo("正在转换场景文件...");
	mSceneEditorCore->transSceneFile(path.ToStdString(), true);
	wxGetApp().getBusyInfo()->hide();
}

void EditorFrame::OnTransSkinMeshFile(wxCommandEvent& event)
{
	wxFileDialog dialog(this, wxT("打开文件"), wxT("../media"), "", "动态模型(*.json)|*.json");
	if (dialog.ShowModal() != wxID_OK)
	{
		return;
	}
	wxString fileName = dialog.GetFilename();
	wxString path = dialog.GetPath();
	wxGetApp().getBusyInfo()->show();
	wxGetApp().getBusyInfo()->setBusyInfo("正在转换动态模型文件...");
	mSceneEditorCore->transSkinMeshFile(path.ToStdString(), true);
	wxGetApp().getBusyInfo()->hide();
}

void EditorFrame::OnCloseFile(wxCommandEvent& event)
{
	mSceneEditorCore->destroyScene();
	refreshAllMenuToolCheckState();
}

void EditorFrame::OnExit(wxCommandEvent& event)
{
	// 发出关闭窗口的事件
	Close(true);
}

void EditorFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
	// 销毁自己的数据
	destroy();
	// 销毁窗口
	Destroy();
}

void EditorFrame::OnUndo(wxCommandEvent& event)
{
	mEditorCoreRoot->getUndoManager()->undo();
}

void EditorFrame::OnRedo(wxCommandEvent& event)
{
	mEditorCoreRoot->getUndoManager()->redo();
}

void EditorFrame::OnCopy(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnCut(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnPaste(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnNewEditor(wxCommandEvent& event)
{
	if (mSceneEditorCore->getCurScene() != NULL)
	{
		CommandSceneNewEditor* cmdNewEditor = NEW_CMD(cmdNewEditor);
		cmdNewEditor->mEditorName = mEditorManager->createUniqueName(EMPTY_STRING);
		mCommandSystem->pushCommand(cmdNewEditor, mSceneEditorCore->getCurScene());
	}
}

void EditorFrame::OnShowEditorGrid(wxCommandEvent& event)
{
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene != NULL)
	{
		// 关闭自动显示,并且隐藏网格
		scene->getEditorGrid()->setAutoShow(event.IsChecked());
		// 如果未选中,则需要隐藏网格,如果是选中状态,则在设置自动显示时会自动判断是否应该显示
		if (!event.IsChecked())
		{
			scene->getEditorGrid()->setVisible(false);
		}
	}
}

void EditorFrame::OnShowEditorAxis(wxCommandEvent& event)
{
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene != NULL)
	{
		// 设置自动显示
		scene->getEditorAxis()->setAutoShow(event.IsChecked());
		// 如果未选中,则需要隐藏坐标轴,如果是选中状态,则在设置自动显示时会自动判断是否应该显示
		if (!event.IsChecked())
		{
			scene->getEditorAxis()->setVisible(false);
		}
	}
}

void EditorFrame::OnShowSky(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnShowSceneGrid(wxCommandEvent& event)
{
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene != NULL)
	{
		// 场景网格没有自动显示和隐藏,所以直接设置显示或者隐藏
		scene->getSceneGrid()->setVisible(event.IsChecked());
	}
}

void EditorFrame::OnShowScene(wxCommandEvent& event)
{
	CommandSceneShow* cmd = NEW_CMD(cmd);
	cmd->mShow = event.IsChecked();
	mCommandSystem->pushCommand(cmd, mSceneEditorCore->getCurScene());
}

void EditorFrame::OnResetCamera(wxCommandEvent& event)
{
	mCameraManager->getMainCamera()->resetCamera();
}

void EditorFrame::OnSceneHierarchyPanel(wxCommandEvent& event)
{
	showPanel(mSceneHierarchy, event.IsChecked());
}

void EditorFrame::OnPropertyPanel(wxCommandEvent& event)
{
	showPanel(mPropertyWindow, event.IsChecked());
}

void EditorFrame::OnRenderCanvas(wxCommandEvent& event)
{
	showPanel(mRenderCanvas, event.IsChecked());
}

void EditorFrame::OnComponentResourcePanel(wxCommandEvent& event)
{
	showPanel(mComponentResource, event.IsChecked());
}

void EditorFrame::OnComponentHierarchyPanel(wxCommandEvent& event)
{
	showPanel(mComponentHierarchy, event.IsChecked());
}

void EditorFrame::showPanel(wxWindow* panel, const bool& show)
{
	if (mAuiManager.GetPane(panel).IsShown() != show)
	{
		mAuiManager.GetPane(panel).Show(show);
		mAuiManager.Update();
	}
}

wxWindow* EditorFrame::getWindow(const std::string& name)
{
	auto iter = mWindowList.find(name);
	if (iter != mWindowList.end())
	{
		return iter->second;
	}
	return NULL;
}

void EditorFrame::OnLoadLayout(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnSaveLayout(wxCommandEvent& event)
{
	;
}

void EditorFrame::OnCalculateHeightMap(wxCommandEvent& event)
{
	CommandSceneCalculateHeightMap* cmd = NEW_CMD(cmd);
	mCommandSystem->pushCommand(cmd, mSceneEditorCore->getCurScene());
}

void EditorFrame::OnShowHeightMap(wxCommandEvent& event)
{
	CommandSceneShowHeightMap* cmd = NEW_CMD(cmd);
	cmd->mShow = event.IsChecked();
	mCommandSystem->pushCommand(cmd, mSceneEditorCore->getCurScene());
}

void EditorFrame::OnSaveHeightMap(wxCommandEvent& event)
{
	CommandSceneSaveHeightMap* cmd = NEW_CMD(cmd);
	mCommandSystem->pushCommand(cmd, mSceneEditorCore->getCurScene());
}

void EditorFrame::OnSaveRoadPoint(wxCommandEvent& event)
{
	ECScene* curScene = mSceneEditorCore->getCurScene();
	std::string roadPointName = "ludian";
	txMap<int, txVector<VECTOR3> > allRoadPointList;
	for (int i = 0; i < 3; ++i)
	{
		txVector<VECTOR3> roadPointList;
		int j = 0;
		while (true)
		{
			txObject* ent = curScene->getScene()->getObject(roadPointName + txStringUtility::intToString(i + 1) + "_" + txStringUtility::intToString(j, 3));
			if (NULL == ent)
			{
				break;
			}
			roadPointList.push_back(ent->GetParent()->GetPosition());
			++j;
		}
		allRoadPointList.insert(i, roadPointList);
	}

	std::string fullPath = txUtility::getMediaPath() + "/" + curScene->getScene()->getName() + "/" + curScene->getScene()->getName() + ".roadpoint";
	saveRoadPoint(fullPath, allRoadPointList);
}

void EditorFrame::OnLoadHeightMap(wxCommandEvent& event)
{
	CommandSceneLoadHeightMap* cmd = NEW_CMD(cmd);
	mCommandSystem->pushCommand(cmd, mSceneEditorCore->getCurScene());
}

void EditorFrame::OnCameraMoveSpeed(wxCommandEvent& event)
{
	mCameraManager->getMainCamera()->setMoveSpeed(event.GetInt());
}

void EditorFrame::OnCameraRotateSpeed(wxCommandEvent& event)
{
	mCameraManager->getMainCamera()->setRotateSpeed(event.GetInt() / 10.0f);
}

void EditorFrame::OnSceneX(wxCommandEvent& event)
{
	std::string curSpeedValue2 = mSceneMoveText->GetValue().ToStdString();
	ECScene* curScene = mSceneEditorCore->getCurScene();
	if (curScene != NULL)
	{
		VECTOR3 moveDelta(txStringUtility::stringToFloat(curSpeedValue2), 0.0f, 0.0f);
		curScene->getScene()->moveScene(moveDelta);
	}
}

void EditorFrame::OnSceneZ(wxCommandEvent& event)
{
	std::string curSpeedValue2 = mSceneMoveText->GetValue().ToStdString();
	ECScene* curScene = mSceneEditorCore->getCurScene();
	if (curScene != NULL)
	{
		VECTOR3 moveDelta(0.0f, 0.0f, txStringUtility::stringToFloat(curSpeedValue2));
		curScene->getScene()->moveScene(moveDelta);
	}
}

void EditorFrame::OnSceneMoveSpeed(wxCommandEvent& event)
{
	
}

void EditorFrame::OnCameraVerticalDown(wxCommandEvent& event)
{
	GameCamera* camera = mCameraManager->getMainCamera();
	MATRIX3 rotation = txMath::getRotationMatrixFromDirection(-txMath::AXIS_Y);
	camera->setRotation(rotation, true);
	camera->setVerticalDown(true);
}

void EditorFrame::OnBackgroundColour(wxColourPickerEvent& event)
{
	wxColour colour = event.GetColour();
	mRenderWindow->setBackgroundColour(VECTOR4(colour.Red() / 255.0f, colour.Green() / 255.0f, colour.Blue() / 255.0f, colour.Alpha() / 255.0f));
}

void EditorFrame::onEditorEvent(const CORE_EVENT& type, const txVector<std::string>& params)
{
	if (type == CE_EDITOR_ERROR)
	{
		MessageBoxA(NULL, params[0].c_str(), "错误", MB_OK);
	}
	else if (type == CE_EDITOR_DELETED)
	{
		CommandEditorFrameNotifyEditorDeleted* cmd = NEW_CMD(cmd);
		cmd->mEditorName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_HEIGHT_MAP_DONE)
	{
		char strBuffer[256];
		SPRINTF(strBuffer, 256, "场景:%s的高度图计算完成,用时:%s秒", params[0].c_str(), params[1].c_str());
		MessageBoxA(NULL, strBuffer, "提示", MB_OK);
	}
	else if (type == CE_EDITOR_ADDED)
	{
		CommandEditorFrameNotifyEditorAdded* cmd = NEW_CMD(cmd);
		cmd->mEditorName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_EDITOR_SELECTION_CHANGED)
	{
		CommandEditorFrameNotifyEditorSelectionChanged* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_COMPONENT_PROPERTY_CHANGED)
	{
		CommandEditorFrameNotifyComponentPropertyChanged* cmd = NEW_CMD(cmd);
		cmd->mPropertyName = params[0];
		cmd->mComponentName = params[1];
		cmd->mOldPropertyValue = params[2];
		cmd->mPropertyValue = params[3];
		cmd->mEditorName = params[4];
		cmd->mChanger = (void*)(txStringUtility::stringToInt(params[5].c_str()));
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_COMPONENT_PROPERTY_REFRESH)
	{
		CommandEditorFrameNotifyComponentPropertyRefreshed* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_EDITOR_COMPONENT_DELETED)
	{
		CommandEditorFrameNotifyEditorComponentDeleted* cmd = NEW_CMD(cmd);
		cmd->mEditorName = params[0];
		cmd->mComponentName = params[1];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_COMPONENT_SELECTION_CHANGED)
	{
		CommandEditorFrameNotifyComponentSelectionChanged* cmd = NEW_CMD(cmd);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_COMPONENT_ADDED)
	{
		CommandEditorFrameNotifyEditorComponentAdded* cmd = NEW_CMD(cmd);
		cmd->mEditorName = params[0];
		cmd->mComponentName = params[1];
		cmd->mComponentPos = txStringUtility::stringToInt(params[2]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_SCENE_OPEN)
	{
		CommandEditorFrameNotifySceneOpen* cmd = NEW_CMD(cmd);
		cmd->mSceneName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_SCENE_NEW)
	{
		CommandEditorFrameNotifySceneNew* cmd = NEW_CMD(cmd);
		cmd->mSceneName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_SCENE_DESTROIED)
	{
		CommandEditorFrameNotifySceneClose* cmd = NEW_CMD(cmd);
		cmd->mSceneName = params[0];
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_SCENE_MODIFIED)
	{
		;
	}
	else if (type == CE_SCENE_RENAMED)
	{
		;
	}
	else if (type == CE_UNDO_ENABLE_CHANGE)
	{
		CommandEditorFrameNotifyUndoEnableChanged* cmd = NEW_CMD(cmd);
		cmd->mEnabled = txStringUtility::stringToBool(params[0]);
		mCommandSystem->pushCommand(cmd, this);
	}
	else if (type == CE_REDO_ENABLE_CHANGE)
	{
		CommandEditorFrameNotifyRedoEnableChanged* cmd = NEW_CMD(cmd);
		cmd->mEnabled = txStringUtility::stringToBool(params[0]);
		mCommandSystem->pushCommand(cmd, this);
	}
}

void EditorFrame::onEngineEvent(const ENGINE_EVENT& event, const txVector<std::string>& paramList)
{
	if (event == EE_LOAD_SCENE_OBJECT)
	{
		std::string info = paramList[0];
		info = std::string("正在加载 : ") + info;
		wxGetApp().getBusyInfo()->setBusyInfo(info.c_str());
	}
	else if (event == EE_END_LOAD_SCENE)
	{
		wxGetApp().getBusyInfo()->hide();
	}
	else if (event == EE_START_LOAD_SCENE)
	{
		wxGetApp().getBusyInfo()->show();
	}
	else if (event == EE_SCENE_LOAD_PROGRESS)
	{
		;
	}
	else if (event == EE_CALCULATE_HEIGHT_MAP_DONE)
	{
		char strBuffer[256];
		SPRINTF(strBuffer, 256, "场景:%s的高度图计算完成,用时:%s秒", paramList[0].c_str(), paramList[1].c_str());
		MessageBoxA(NULL, strBuffer, "提示", MB_OK);
	}
	else if (event == EE_ENGINE_ERROR_INFO)
	{
		MessageBoxA(NULL, paramList[0].c_str(), "错误", MB_OK);
	}
}