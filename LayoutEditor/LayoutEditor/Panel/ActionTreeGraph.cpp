#include "txAction.h"
#include "txActionTree.h"
#include "Utility.h"
#include "txTremblingNode.h"
#include "txTremblingKeyFrame.h"

#include "EditorCoreRoot.h"
#include "ActionTreeEditorCore.h"

#include "ActionTreeGraph.h"

enum
{
	ID_MENU_DELETE_ACTION,
};

BEGIN_EVENT_TABLE(ActionTreeGraph, GraphPanelBase)
EVT_MENU(ID_MENU_DELETE_ACTION, ActionTreeGraph::OnDeleteAction)
END_EVENT_TABLE()

const int BLOCK_WIDTH = 10;
const int BLOCK_HEIGHT = 10;
const float MAX_UNIT_PIXELS = 1000.0f;
const float MIN_UNIT_PIXELS = 0.1f;
const int LEVEL_COUNT = 17;
const float LEVEL[LEVEL_COUNT] = { MIN_UNIT_PIXELS, 0.2f, 0.4f, 0.8f, 1.0f, 2.0f, 4.0f, 8.0f, 10.0f, 20.0f, 40.0f, 80.0f, 100.0f, 200.0f, 400.0f, 800.0f, MAX_UNIT_PIXELS };

ActionTreeGraph::ActionTreeGraph(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
	:
	GraphPanelBase(parent, position, size, style, name),
	mPixelPerUnitHori(100.0f),
	mHoriStart(30),
	mVertStart(230)
{
	mActionColourList.insert(TOSTRING(txAction), txMath::COLOUR_RED);
	mActionColourList.insert(TOSTRING(ActionWindow), txMath::COLOUR_GREEN);
	mActionColourList.insert(TOSTRING(ActionWindowAlpha), txMath::COLOUR_BLUE);
	mActionColourList.insert(TOSTRING(ActionWindowHSL), txMath::COLOUR_YELLOW);
	mActionColourList.insert(TOSTRING(ActionWindowMove), txMath::COLOUR_PURPLE);
	mActionColourList.insert(TOSTRING(ActionWindowRotate), txMath::COLOUR_ORANGE);
	mActionColourList.insert(TOSTRING(ActionWindowScale), txMath::COLOUR_PINK);
}

void ActionTreeGraph::notifyMouseWheel(wxMouseEvent& event)
{
	float delta = event.GetWheelRotation();
	// 按住shift时滚动中键缩放纵轴
	float curRelative = mHoriStart - mSize.x / 2;
	if (delta > 0)
	{
		curRelative *= 1.1f;
		mPixelPerUnitHori *= 1.1f;
		if (mPixelPerUnitHori > MAX_UNIT_PIXELS)
		{
			curRelative /= 1.1f;
			mPixelPerUnitHori = MAX_UNIT_PIXELS;
		}
	}
	else
	{
		curRelative /= 1.1f;
		mPixelPerUnitHori /= 1.1f;
		if (mPixelPerUnitHori < MIN_UNIT_PIXELS)
		{
			curRelative *= 1.1f;
			mPixelPerUnitHori = MIN_UNIT_PIXELS;
		}
	}
	mHoriStart = (int)(curRelative + mSize.x / 2);
}

void ActionTreeGraph::notifyMouseLeftUp(wxMouseEvent& event)
{
	txAction* action = hitAction(wxPoint(event.GetPosition().x, transPosY(event.GetPosition().y)));
	if (action != NULL)
	{
		mActionTreeEditorCore->selectAction(action->getName());
	}
	else
	{
		mActionTreeEditorCore->selectAction(EMPTY_STRING);
	}
}

void ActionTreeGraph::notifyMouseRightUp(wxMouseEvent& event)
{
	mMenuPosition = event.GetPosition();
	mMenuPosition = wxPoint(mMenuPosition.x, transPosY(mMenuPosition.y));
	wxMenu menu;
	if (hitAction(mMenuPosition) != NULL)
	{
		menu.Append(ID_MENU_DELETE_ACTION, wxT("删除行为"));
	}
	PopupMenu(&menu, event.GetPosition());// 显示弹出菜单  
	event.Skip();
}

void ActionTreeGraph::notifyMiddleDown(wxMouseEvent& event)
{
	wxPoint curPos = event.GetPosition();
	curPos.y = transPosY(curPos.y);
	mMiddlePosOffset = wxPoint(mHoriStart, mVertStart) - curPos;
}

void ActionTreeGraph::notifyMouseMove(wxMouseEvent& event)
{
	// 按住中键可以移动整个坐标系
	if (event.MiddleIsDown())
	{
		wxPoint curPos = event.GetPosition();
		curPos.y = transPosY(curPos.y);
		wxPoint start = curPos + mMiddlePosOffset;
		mHoriStart = start.x;
		mVertStart = start.y;
	}
}

void ActionTreeGraph::notifyKeyDown(wxKeyEvent& event)
{
	unsigned int key = event.GetRawKeyCode();
	// 按O键可以恢复坐标系的移动
	if (key == 'O')
	{
		mHoriStart = 30.0f;
		mVertStart = mSize.y / 2.0f;
	}
}

void ActionTreeGraph::notifyKeyUp(wxKeyEvent& event)
{
	;
}

void ActionTreeGraph::OnDeleteAction(wxCommandEvent& event)
{
	;
}

void ActionTreeGraph::render(wxDC* dc)
{
	drawCoord(dc);
	drawAction(dc);
}

void ActionTreeGraph::drawAction(wxDC* dc)
{
	txActionTree* actionTree = mActionTreeEditorCore->getCurActionTree();
	if (actionTree != NULL)
	{
		drawAction(dc, actionTree->getRootAction(), 0, 0);
	}
}

void ActionTreeGraph::drawAction(wxDC* dc, txAction* action, const int& posY, const int& parentPosY)
{
	if (action == NULL)
	{
		return;
	}
	int posX = timeToPositionX(action->getTimeDepth());
	int width = timeToPositionX(action->getExecuteDoneTimeDepth()) - posX;
	if (width == 0)
	{
		width = 5;
	}
	// 每个行为的高是height,竖直方向上每个行为之间的间隔是heightInterval
	int height = 30;
	int heightInterval = 30;
	if (mActionTreeEditorCore->getCurAction() == action)
	{
		drawRect(dc, posX - 1, mVertStart + posY + height / 2 + 1, width + 2, height + 2, getColour(action->getTypeName()) / 2.0f);
	}
	// 先绘制当前行为
	drawSolidRect(dc, posX, mVertStart + posY + height / 2, width, height, getColour(action->getTypeName()));

	// 然后绘制所有行为子节点
	txVector<txAction*>& childList = action->getChildList();
	int childCount = childList.size();
	int minY = posY;
	if (childCount % 2 == 0)
	{
		minY += -childCount / 2 * height + height / 2 - (childCount / 2 - 1) * heightInterval - heightInterval / 2;
	}
	else
	{
		minY += -childCount / 2 * height - (childCount / 2) * heightInterval;
	}
	FOR_STL(childList, int i = 0; i < childCount; ++i)
	{
		drawAction(dc, childList[i], minY + i * 2 * height, posY);
	}
	END_FOR_STL(childList);
}

void ActionTreeGraph::drawCoord(wxDC* dc)
{
	// 横轴
	int vertStart = mVertStart;
	txMath::clamp(vertStart, 10, (int)mSize.y - 5);
	drawLine(dc, mHoriStart, vertStart, mSize.x, vertStart, txMath::COLOUR_RED);
	for (int i = 0; i < LEVEL_COUNT; ++i)
	{
		if (mPixelPerUnitHori <= LEVEL[i])
		{
			float tempUnit = mPixelPerUnitHori * (100.0f / LEVEL[i]);
			int horiScaleCount = (mSize.x - mHoriStart) / tempUnit + 1;
			for (int j = 0; j < horiScaleCount; ++j)
			{
				std::string text = txStringUtility::floatToString(j * 100.0f / LEVEL[i]);
				wxSize textSize = dc->GetTextExtent(text);
				drawText(dc, text, mHoriStart + j * tempUnit - textSize.x, vertStart, txMath::COLOUR_RED);
				if (j > 0)
				{
					// 格子竖线
					drawLine(dc, mHoriStart + j * tempUnit, 0, mHoriStart + j * tempUnit, mSize.y, txMath::COLOUR_BLUE);
					// 刻度线
					drawLine(dc, mHoriStart + j * tempUnit, vertStart, mHoriStart + j * tempUnit, vertStart - 5, txMath::COLOUR_RED);
				}
			}
			break;
		}
	}

	// 纵轴
	int horiStart = mHoriStart;
	txMath::clamp(horiStart, 25, (int)mSize.x - 2);
	drawLine(dc, horiStart, 0, horiStart, mSize.y, txMath::COLOUR_RED);
}

txAction* ActionTreeGraph::hitAction(const wxPoint& point)
{
	txActionTree* actionTree = mActionTreeEditorCore->getCurActionTree();
	if (actionTree == NULL)
	{
		return NULL;
	}
	return hitAction(actionTree->getRootAction(), point, 0, 0);
}

txAction* ActionTreeGraph::hitAction(txAction* action, const wxPoint& point, const int& posY, const int& parentY)
{
	if (action == NULL)
	{
		return NULL;
	}
	// 先判断是否选中了当前节点
	int posX = timeToPositionX(action->getTimeDepth());
	int width = timeToPositionX(action->getExecuteDoneTimeDepth()) - posX;
	if (width == 0)
	{
		width = 5;
	}
	// 每个行为的高是height,竖直方向上每个行为之间的间隔是heightInterval
	int height = 30;
	int heightInterval = 30;
	wxRect blockRect;
	blockRect.width = width;
	blockRect.height = height;
	blockRect.x = posX;
	blockRect.y = mVertStart + posY - height / 2;
	if (blockRect.Contains(point))
	{
		return action;
	}
	// 然后判断是否选中了子节点
	txVector<txAction*>& childList = action->getChildList();
	int childCount = childList.size();
	int minY = posY;
	if (childCount % 2 == 0)
	{
		minY += -childCount / 2 * height + height / 2 - (childCount / 2 - 1) * heightInterval - heightInterval / 2;
	}
	else
	{
		minY += -childCount / 2 * height - (childCount / 2) * heightInterval;
	}
	FOR_STL(childList, int i = 0; i < childCount; ++i)
	{
		txAction* hitAct = hitAction(childList[i], point, minY + i * 2 * height, posY);
		if (hitAct != NULL)
		{
			return hitAct;
		}
	}
	END_FOR_STL(childList);
	return NULL;
}

const VECTOR3& ActionTreeGraph::getColour(const std::string& actionName)
{
	auto iterColour = mActionColourList.find(actionName);
	if (iterColour != mActionColourList.end())
	{
		return iterColour->second;
	}
	return txMath::COLOUR_BLACK;
}