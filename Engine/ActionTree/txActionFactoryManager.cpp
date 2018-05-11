#include "txActionFactory.h"
#include "txActionFactoryManager.h"
#include "txAction.h"
#include "WindowActionHeader.h"

void txActionFactoryManager::init()
{
	ADD_ACTION_FACTORY(txAction);
	ADD_ACTION_FACTORY(ActionWindow);
	ADD_ACTION_FACTORY(ActionWindowAlpha);
	ADD_ACTION_FACTORY(ActionWindowHSL);
	ADD_ACTION_FACTORY(ActionWindowMove);
	ADD_ACTION_FACTORY(ActionWindowRotate);
	ADD_ACTION_FACTORY(ActionWindowScale);
}