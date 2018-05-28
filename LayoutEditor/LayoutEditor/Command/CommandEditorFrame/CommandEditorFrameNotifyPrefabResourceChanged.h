#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_PREFAB_RESOURCE_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_PREFAB_RESOURCE_CHANGED_H_

class CommandEditorFrameNotifyPrefabResourceChanged : public EditorCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif