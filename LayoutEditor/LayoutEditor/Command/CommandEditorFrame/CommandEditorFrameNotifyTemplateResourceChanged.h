#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_TEMPLATE_RESOURCE_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_TEMPLATE_RESOURCE_CHANGED_H_

class CommandEditorFrameNotifyTemplateResourceChanged : public EditorCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif