#include "Command.h"
#include "CommandReceiver.h"

Command::Command(const char* file, int line, bool showInfo)
:
mReceiver(NULL),
mFile(file),
mLine(line),
mShowDebugInfo(showInfo),
mEndCallback(NULL),
mStartCallback(NULL),
mEndUserData(NULL),
mStartUserData(NULL),
mDelayCommand(false)
{
	;
}