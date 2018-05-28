#ifndef _ACTION_TREE_GRAPH_H_
#define _ACTION_TREE_GRAPH_H_

#include "GraphPanelBase.h"

class ActionTreeGraph : public GraphPanelBase
{
public:
	ActionTreeGraph(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~ActionTreeGraph(){}
	virtual void setup(){}
	virtual void render(wxDC* dc);
	virtual void notifyMouseWheel(wxMouseEvent& event);
	virtual void notifyMouseLeftUp(wxMouseEvent& event);
	virtual void notifyMouseRightUp(wxMouseEvent& event);
	virtual void notifyMiddleDown(wxMouseEvent& event);
	virtual void notifyMouseMove(wxMouseEvent& event);
	virtual void notifyKeyDown(wxKeyEvent& key);
	virtual void notifyKeyUp(wxKeyEvent& key);
	DECLARE_EVENT_TABLE()
	void OnDeleteAction(wxCommandEvent& event);
protected:
	void drawAction(wxDC* dc);	// 绘制所有关键帧
	void drawCoord(wxDC* dc);	// 绘制坐标轴和刻度线
	void drawAction(wxDC* dc, txAction* action, const int& posY, const int& parentPosY);
	txAction* hitAction(const wxPoint& point);	// 通过一个坐标点获得一个关键帧
	txAction* hitAction(txAction* action, const wxPoint& point, const int& posY, const int& parentY);
	float positionXToTime(const int& posX){ return (posX - mHoriStart) / (float)mPixelPerUnitHori; }
	int timeToPositionX(const float& time) { return time * mPixelPerUnitHori + mHoriStart; }
	const VECTOR3& getColour(const std::string& actionName);
protected:
	float mPixelPerUnitHori;		// 横轴多少个像素为一个单位的距离,浮点数方便计算
	int mHoriStart;
	int mVertStart;
	wxPoint mMouseOffset;
	wxPoint mMiddlePosOffset;		// 中键按下时鼠标位置相对于坐标系起点的偏移
	wxPoint mMenuPosition;			// 弹出菜单时鼠标的位置
	txMap<std::string, VECTOR3> mActionColourList;
};

#endif