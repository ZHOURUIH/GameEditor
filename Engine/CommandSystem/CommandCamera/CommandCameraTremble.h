#ifndef _COMMAND_CAMERA_TREMBLE_H_
#define _COMMAND_CAMERA_TREMBLE_H_

#include "txCommand.h"

// Ïà»úÕð¶¯
class CommandCameraTremble : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mOnceLength = 1.0f;
		mLoop = false;
		mOffset = 0.0f;
		mAmplitude = 1.0f;
		mFullOnce = false;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mName;  
	float mOnceLength;
	float mOffset;
	bool mLoop;
	float mAmplitude;
	bool mFullOnce;
};

#endif

