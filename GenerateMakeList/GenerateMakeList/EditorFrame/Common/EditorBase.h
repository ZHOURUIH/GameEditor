#ifndef _EDITOR_BASE_H_
#define _EDITOR_BASE_H_

#include "EditorHeader.h"

class EditorFrame;
class EditorBase
{
public:
	static void notifyConstructDone();
protected:
	static EditorFrame* mEditorFrame;
};

#endif