#include "ECUndoChangeComponentProperty.h"
#include "SceneEditorCore.h"
#include "CoreCommandHeader.h"
#include "ECScene.h"
#include "ECEditor.h"

void ECUndoChangeComponentProperty::undo()
{
	ECScene* scene = mSceneEditorCore->getCurScene();
	if (scene == NULL)
	{
		return;
	}
	ECEditor* editor = scene->getEditor(mEditorName);
	if (editor == NULL)
	{
		return;
	}
	
	CommandEditorSetProperty* cmdProperty = NEW_CMD(cmdProperty);
	cmdProperty->mChangeOperator = this;
	cmdProperty->mComponentName = mComponentName;
	cmdProperty->mPropertyName = mPropertyName;
	cmdProperty->mPropertyValue = mPropertyValue;
	mCommandSystem->pushCommand(cmdProperty, editor);
}