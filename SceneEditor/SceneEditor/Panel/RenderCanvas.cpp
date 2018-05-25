#include "txEngineRoot.h"
#include "txRenderWindow.h"
#include "txCamera.h"
#include "txSkyBox.h"
#include "txSceneManager.h"
#include "txTextureManager.h"

#include "GameCamera.h"
#include "EditorCoreRoot.h"
#include "InputSystem.h"
#include "CameraManager.h"

#include "RenderCanvas.h"
#include "EditorApp.h"
#include "EditorFrame.h"
#include "EditorCommandHeader.h"

BEGIN_EVENT_TABLE(RenderCanvas, wxWindow)

EVT_SHOW(RenderCanvas::OnShow)
EVT_SIZE(RenderCanvas::OnSize)
EVT_PAINT(RenderCanvas::OnPaint)
EVT_LEFT_DOWN(RenderCanvas::OnMouseLeftDown)
EVT_LEFT_UP(RenderCanvas::OnMouseLeftUp)
EVT_RIGHT_DOWN(RenderCanvas::OnMouseRightDown)
EVT_RIGHT_UP(RenderCanvas::OnMouseRightUp)
EVT_MOTION(RenderCanvas::OnMouseMove)
EVT_CHAR(RenderCanvas::OnChar)

END_EVENT_TABLE()

RenderCanvas::RenderCanvas(wxWindow* parent, const wxPoint& position, const wxSize& size, long style, const wxString& name)
:
wxWindow(parent, wxID_ANY, position, size, style, name)
{
	;
}

void RenderCanvas::OnSize(wxSizeEvent& event)
{
	if (mRenderWindow == NULL)
	{
		return;
	}
	// 渲染窗口中的EditorCoreRoot需要实时获取
	wxSize size = event.GetSize();
	mRenderWindow->setWindowSize(size.x, size.y);
	// 设置主摄像机的视口尺寸,如果还有其他摄像机,也需要在这里设置
	GameCamera* mainCamera = mCameraManager->getMainCamera();
	if (mainCamera != NULL)
	{
		mainCamera->getCamera()->setViewport(0, 0, size.x, size.y);
	}
}

void RenderCanvas::OnShow(wxShowEvent& event)
{
	CommandEditorFrameNotifyPanelShow* cmd = NEW_CMD(cmd);
	cmd->mPanelName = GetName().ToStdString();
	cmd->mShow = event.IsShown();
	mCommandSystem->pushCommand(cmd, mEditorFrame);
}

void RenderCanvas::OnPaint(wxPaintEvent& event)
{
	;
}

void RenderCanvas::OnKeyUp(wxKeyEvent& event)
{
	mInputSystem->keyUp(event.GetRawKeyCode());
}

void RenderCanvas::OnKeyDown(wxKeyEvent& event)
{
	mInputSystem->keyDown(event.GetRawKeyCode());
}

void RenderCanvas::OnChar(wxKeyEvent& event)
{
	;
}

void RenderCanvas::OnMouseLeftDown(wxMouseEvent& event)
{
	mInputSystem->mouseLeftDown(txIPoint(event.GetPosition().x, event.GetPosition().y));
}

void RenderCanvas::OnMouseLeftUp(wxMouseEvent& event)
{
	mInputSystem->mouseLeftUp(txIPoint(event.GetPosition().x, event.GetPosition().y));
}

void RenderCanvas::OnMouseRightDown(wxMouseEvent& event)
{
	mInputSystem->mouseRightDown(txIPoint(event.GetPosition().x, event.GetPosition().y));
}

void RenderCanvas::OnMouseRightUp(wxMouseEvent& event)
{
	mInputSystem->mouseRightUp(txIPoint(event.GetPosition().x, event.GetPosition().y));
}

void RenderCanvas::OnMouseMove(wxMouseEvent& event)
{
	mInputSystem->mouseMove(txIPoint(event.GetPosition().x, event.GetPosition().y));
}

void RenderCanvas::OnMouseWheel(wxMouseEvent& event)
{
	mInputSystem->mouseWheel(-event.GetWheelRotation() / 120.0f);
}