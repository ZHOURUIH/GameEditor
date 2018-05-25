#ifndef _KEY_FRAME_GRAPH_H_
#define _KEY_FRAME_GRAPH_H_

#include "GraphPanelBase.h"

class txTremblingKeyFrame;
class KeyFrameGraph : public GraphPanelBase
{
public:
	KeyFrameGraph(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name);
	virtual ~KeyFrameGraph(){}
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
	void OnInsertKeyFrame(wxCommandEvent& event);
	void OnDeleteKeyFrame(wxCommandEvent& event);
protected:
	void drawKeyFrame(wxDC* dc);	// 绘制所有关键帧
	void drawCoord(wxDC* dc);		// 绘制坐标轴和刻度线
	txTremblingKeyFrame* hitKeyFrame(const wxPoint& point, wxPoint& offset);	// 通过一个坐标点获得一个关键帧,offset时返回鼠标位置相对于关键帧滑块中心点的偏移
	float positionXToTime(const int& posX){ return (posX - mHoriStart) / (float)mPixelPerUnitHori; }
	float positionYToOffset(const int& posY){ return (posY - mVertStart) / (float)mPixelPerUnitVert; }
	int timeToPositionX(const float& time) { return time * mPixelPerUnitHori + mHoriStart; }
	int offsetToPositionY(const float& offset) { return offset * mPixelPerUnitVert + mVertStart; }
protected:
	float mPixelPerUnitHori;		// 横轴多少个像素为一个单位的距离,浮点数方便计算
	float mPixelPerUnitVert;		// 纵轴多少个像素为一个单位的距离,浮点数方便计算
	int mHoriStart;
	int mVertStart;
	bool mIsShiftDown;
	wxPoint mMouseOffset;
	wxPoint mMiddlePosOffset;		// 中键按下时鼠标位置相对于坐标系起点的偏移
	wxPoint mMenuPosition;			// 弹出菜单时鼠标的位置
};

#endif