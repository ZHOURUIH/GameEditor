#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txCamera.h"
#include "txSkyBox.h"
#include "txSceneManager.h"
#include "txTextureManager.h"
#include "InputSystem.h"

#include "EditorCoreRoot.h"
#include "EditorCommandHeader.h"

#include "RenderCanvas.h"
#include "EditorApp.h"
#include "EditorFrame.h"

BEGIN_EVENT_TABLE(RenderCanvas, wxPanel)
EVT_SIZE(RenderCanvas::OnSize)
EVT_PAINT(RenderCanvas::OnPaint)
EVT_LEFT_DOWN(RenderCanvas::OnMouseLeftDown)
EVT_LEFT_UP(RenderCanvas::OnMouseLeftUp)
EVT_RIGHT_DOWN(RenderCanvas::OnMouseRightDown)
EVT_RIGHT_UP(RenderCanvas::OnMouseRightUp)
EVT_MOTION(RenderCanvas::OnMouseMove)
EVT_CHAR(RenderCanvas::OnChar)
EVT_SHOW(RenderCanvas::OnShow)
END_EVENT_TABLE()

RenderCanvas::RenderCanvas(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
wxPanel(parent, wxID_ANY, position, size, style, name),
mAdjustMousePosition(false)
{
	;
}

void RenderCanvas::OnShow(wxShowEvent& event)
{
	// 改变渲染窗口大小时,强制不调整鼠标位置,如果需要调整,则在设置完窗口大小后再通知
	notifyAdjustMousePosition(false);

	CommandEditorFrameNotifyPanelShow* cmd = NEW_CMD(cmd);
	cmd->mPanelName = GetName().ToStdString();
	cmd->mShow = event.IsShown();
	mCommandSystem->pushCommand(cmd, mEditorFrame);
}

void RenderCanvas::OnSize(wxSizeEvent& event)
{
	// 通知编辑核心,渲染窗口的大小发生了改变,此处不能直接使用mEditorCoreRoot
	if (mEditorCoreRoot != NULL)
	{
		wxSize size = event.GetSize();
		mEditorCoreRoot->resize(size.x, size.y);
	}
}

void RenderCanvas::OnPaint(wxPaintEvent& event)
{
	;
}

void RenderCanvas::OnKeyUp(wxKeyEvent& event)
{
	if (mInputSystem != NULL)
	{
		mInputSystem->keyUp(event.GetRawKeyCode());
	}
}

void RenderCanvas::OnKeyDown(wxKeyEvent& event)
{
	if (mInputSystem != NULL)
	{
		mInputSystem->keyDown(event.GetRawKeyCode());
	}
}

void RenderCanvas::OnChar(wxKeyEvent& event)
{
	;
}

txIPoint RenderCanvas::getMouseEventPosition(const wxMouseEvent& event)
{
	wxPoint pt = event.GetPosition();
	// 如果是win10系统,则将y坐标加上35,其他系统,则加上17
	if (txUtility::getWindowsSystem() == WS_WIN_10)
	{
		return txIPoint(pt.x, pt.y + (mAdjustMousePosition ? 35 : 0));
	}
	else
	{
		return txIPoint(pt.x, pt.y + (mAdjustMousePosition ? 17 : 0));
	}
}

void RenderCanvas::OnMouseLeftDown(wxMouseEvent& event)
{
	txIPoint mousePos = getMouseEventPosition(event);
	if (mInputSystem != NULL)
	{
		mInputSystem->mouseLeftDown(mousePos);
	}
}

void RenderCanvas::OnMouseLeftUp(wxMouseEvent& event)
{
	if (mInputSystem != NULL)
	{
		txIPoint mousePos = getMouseEventPosition(event);
		mInputSystem->mouseLeftUp(mousePos);
	}
}

void RenderCanvas::OnMouseRightDown(wxMouseEvent& event)
{
	
	if (mInputSystem != NULL)
	{
		txIPoint mousePos = getMouseEventPosition(event);
		mInputSystem->mouseRightDown(mousePos);
	}
}

void RenderCanvas::OnMouseRightUp(wxMouseEvent& event)
{
	if (mInputSystem != NULL)
	{
		txIPoint mousePos = getMouseEventPosition(event);
		mInputSystem->mouseRightUp(mousePos);
	}
}

void RenderCanvas::OnMouseMove(wxMouseEvent& event)
{
	if (mInputSystem != NULL)
	{
		txIPoint mousePos = getMouseEventPosition(event);
		mInputSystem->mouseMove(mousePos);
	}
}

void RenderCanvas::OnMouseWheel(wxMouseEvent& event)
{
	;
}