#pragma once

#include "FrameHeader.h"

class CSVEditor;
class EditorApp;
class EditorFrame;
class MainListWindow;
class UndoManager;

namespace EditorBase
{
	extern CSVEditor* mCSVEditor;
	extern EditorApp* mEditorApp;
	extern EditorFrame* mEditorFrame;
	extern MainListWindow* mMainListWindow;
	extern UndoManager* mUndoManager;
};

using namespace EditorBase;