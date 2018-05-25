#include "txComponentFactory.h"
#include "txComponentFactoryManager.h"

#include "EditorCommandHeader.h"
#include "EditorCoreRoot.h"
#include "ECEditor.h"
#include "SceneEditorCore.h"
#include "ComponentEditorCore.h"

#include "EditorApp.h"
#include "EditorFrame.h"
#include "ComponentResource.h"

enum
{
	ID_REFRESH,
	ID_WINDOW_RESOURCE_LISTCTRL,
};

BEGIN_EVENT_TABLE(ComponentResource, EditorPanel)
EVT_TOOL(ID_REFRESH, ComponentResource::OnRefresh)
EVT_LIST_ITEM_ACTIVATED(ID_WINDOW_RESOURCE_LISTCTRL, ComponentResource::OnDoubleClick)
END_EVENT_TABLE()

txMap<std::string, std::string> ComponentResource::mComponentTypeLabelList;
txMap<std::string, std::string> ComponentResource::mComponentLabelTypeList;

ComponentResource::ComponentResource(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
EditorPanel(parent, position, size, style, name)
{
	setup();
}

ComponentResource::~ComponentResource()
{
	TRACE_DELETE(mImageList);
}

void ComponentResource::setup()
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);
	mResourceList = new wxListCtrl(this, ID_WINDOW_RESOURCE_LISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_NO_HEADER | wxLC_ICON | wxLC_SINGLE_SEL);
	bSizer1->Add(mResourceList, 1, wxALL | wxEXPAND, 5);
	this->SetSizer(bSizer1);
	this->Layout();

	mComponentTypeLabelList.insert(TOSTRING(txComponentAlpha), "透明");
	mComponentTypeLabelList.insert(TOSTRING(txComponentAudio), "音效");
	mComponentTypeLabelList.insert(TOSTRING(txComponentCollisionBox), "盒子碰撞体");
	mComponentTypeLabelList.insert(TOSTRING(txComponentCollisionCapsule), "胶囊碰撞体");
	mComponentTypeLabelList.insert(TOSTRING(txComponentCollisionCylinder), "圆柱体碰撞体");
	mComponentTypeLabelList.insert(TOSTRING(txComponentCollisionPlane), "平面碰撞体");
	mComponentTypeLabelList.insert(TOSTRING(txComponentCollisionSphere), "球体碰撞体");
	mComponentTypeLabelList.insert(TOSTRING(txComponentDynamicMesh), "动态模型");
	mComponentTypeLabelList.insert(TOSTRING(txComponentEffect), "粒子特效");
	mComponentTypeLabelList.insert(TOSTRING(txComponentHSL), "HSL");
	mComponentTypeLabelList.insert(TOSTRING(txComponentMove), "运动");
	mComponentTypeLabelList.insert(TOSTRING(txComponentPhysics), "物理");
	mComponentTypeLabelList.insert(TOSTRING(txComponentRotateAccelerateToTarget), "加速旋转到目标");
	mComponentTypeLabelList.insert(TOSTRING(txComponentRotateSpeed), "旋转");
	mComponentTypeLabelList.insert(TOSTRING(txComponentRotateToTarget), "匀速旋转到目标");
	mComponentTypeLabelList.insert(TOSTRING(txComponentScale), "缩放");
	mComponentTypeLabelList.insert(TOSTRING(txComponentStaticMesh), "静态模型");
	mComponentTypeLabelList.insert(TOSTRING(txComponentTransform), "变换");
	mComponentTypeLabelList.insert(TOSTRING(txComponentTrembling), "震动");
	mComponentTypeLabelList.insert(TOSTRING(txComponentTwinkle), "闪烁");
	auto iterNameLabel = mComponentTypeLabelList.begin();
	auto iterNameLabelEnd = mComponentTypeLabelList.end();
	FOR_STL(mComponentTypeLabelList, ; iterNameLabel != iterNameLabelEnd; ++iterNameLabel)
	{
		mComponentLabelTypeList.insert(iterNameLabel->second, iterNameLabel->first);
	}
	END_FOR_STL(mComponentTypeLabelList);

	txMap<std::string, wxBitmap> bitmapList;
	bitmapList.insert(TOSTRING(txComponentAlpha), wxBITMAP(IDB_COMPONENT_ALPHA));
	bitmapList.insert(TOSTRING(txComponentAudio), wxBITMAP(IDB_COMPONENT_AUDIO));
	bitmapList.insert(TOSTRING(txComponentCollisionBox), wxBITMAP(IDB_COMPONENT_COLLISION_BOX));
	bitmapList.insert(TOSTRING(txComponentCollisionCapsule), wxBITMAP(IDB_COMPONENT_COLLISION_CAPSULE));
	bitmapList.insert(TOSTRING(txComponentCollisionCylinder), wxBITMAP(IDB_COMPONENT_COLLISION_CYLINDER));
	bitmapList.insert(TOSTRING(txComponentCollisionPlane), wxBITMAP(IDB_COMPONENT_COLLISION_PLANE));
	bitmapList.insert(TOSTRING(txComponentCollisionSphere), wxBITMAP(IDB_COMPONENT_COLLISION_SPHERE));
	bitmapList.insert(TOSTRING(txComponentDynamicMesh), wxBITMAP(IDB_COMPONENT_DYNAMIC_MESH));
	bitmapList.insert(TOSTRING(txComponentEffect), wxBITMAP(IDB_COMPONENT_EFFECT));
	bitmapList.insert(TOSTRING(txComponentHSL), wxBITMAP(IDB_COMPONENT_HSL));
	bitmapList.insert(TOSTRING(txComponentMove), wxBITMAP(IDB_COMPONENT_MOVE));
	bitmapList.insert(TOSTRING(txComponentPhysics), wxBITMAP(IDB_COMPONENT_PHYSICS));
	bitmapList.insert(TOSTRING(txComponentRotateAccelerateToTarget), wxBITMAP(IDB_COMPONENT_ROTATE_ACCELERATE_TO_TARGET));
	bitmapList.insert(TOSTRING(txComponentRotateSpeed), wxBITMAP(IDB_COMPONENT_ROTATE_SPEED));
	bitmapList.insert(TOSTRING(txComponentRotateToTarget), wxBITMAP(IDB_COMPONENT_ROTATE_TO_TARGET));
	bitmapList.insert(TOSTRING(txComponentScale), wxBITMAP(IDB_COMPONENT_SCALE));
	bitmapList.insert(TOSTRING(txComponentStaticMesh), wxBITMAP(IDB_COMPONENT_STATIC_MESH));
	bitmapList.insert(TOSTRING(txComponentTransform), wxBITMAP(IDB_COMPONENT_TRANSFORM));
	bitmapList.insert(TOSTRING(txComponentTrembling), wxBITMAP(IDB_COMPONENT_TREMBLING));
	bitmapList.insert(TOSTRING(txComponentTwinkle), wxBITMAP(IDB_COMPONENT_TWINKLE));

	mImageList = TRACE_NEW(wxImageList, mImageList, 16, 16);
	auto iterBitmap = bitmapList.begin();
	auto iterBitmapEnd = bitmapList.end();
	FOR_STL(bitmapList, ; iterBitmap != iterBitmapEnd; ++iterBitmap)
	{
		mComponentIconIndexList.insert(iterBitmap->first, mImageList->Add(iterBitmap->second));
	}
	END_FOR_STL(bitmapList);
	mResourceList->SetImageList(mImageList, 0);
}

void ComponentResource::refreshList()
{
	mResourceList->DeleteAllItems();
	auto& factoryList = mComponentFactoryManager->getFactoryList();
	auto iter = factoryList.begin();
	auto iterEnd = factoryList.end();
	FOR_STL(factoryList, ; iter != iterEnd; ++iter)
	{
		if (txStringUtility::startWith(iter->first, "Camera"))
		{
			continue;
		}
		int imageIndex = -1;
		auto iterIndex = mComponentIconIndexList.find(iter->first);
		if (iterIndex != mComponentIconIndexList.end())
		{
			imageIndex = iterIndex->second;
		}
		const std::string& label = getComponentLabelFromType(iter->first);
		mResourceList->InsertItem(mResourceList->GetItemCount(), label, imageIndex);
	}
	END_FOR_STL(factoryList);
}

void ComponentResource::OnRefresh(wxCommandEvent& event)
{
	refreshList();
}

void ComponentResource::OnDoubleClick(wxListEvent& event)
{
	auto& editorSelections = mSceneEditorCore->getEditorSelections();
	if (editorSelections.size() == 0)
	{
		return;
	}
	auto& componentSelections = mComponentEditorCore->getComponentSelections();
	std::string itemName = mResourceList->GetItemText(event.GetItem());
	CommandEditorAddComponent* cmdAdd = NEW_CMD(cmdAdd);
	cmdAdd->mType = getComponentTypeFromLabel(itemName);
	if (componentSelections.size() > 0)
	{
		cmdAdd->mParentComponentName = componentSelections[0]->getName();
	}
	mCommandSystem->pushCommand(cmdAdd, editorSelections[0]);
}