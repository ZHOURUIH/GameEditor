#include "Utility.h"
#include "txTrembling.h"
#include "txTremblingManager.h"
#include "txCommandHeader.h"
#include "GameCamera.h"
#include "CameraTrembling.h"

void CommandCameraTremble::execute()
{
	GameCamera* gameCamera = CMD_CAST<GameCamera*>(mReceiver);
	CameraTrembling* comTrembling = static_cast<CameraTrembling*>(gameCamera->getFirstActiveComponent(TOSTRING(CameraTrembling)));
	if (comTrembling != NULL)
	{
		comTrembling->play(mName, mLoop, mOnceLength, mOffset, mFullOnce, mAmplitude);
	}
}

std::string CommandCameraTremble::showDebugInfo()
{
	COMMAND_DEBUG("name : %s, loop : %s, speed : %f, offset : %f, fullOnce : %s, amplitude : %f", 
		mName.c_str(), txStringUtility::boolToString(mLoop).c_str(), mOnceLength, mOffset, txStringUtility::boolToString(mFullOnce).c_str(), mAmplitude);
}