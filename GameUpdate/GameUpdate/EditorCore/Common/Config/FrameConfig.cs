using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

public class FrameConfig : ConfigBase
{
	public FrameConfig(string name)
		: base(name)
	{
		mFloatOrString = true;
	}
	public override void init()
	{
		base.init();
		addFloat();
	}
	//-----------------------------------------------------------------------------------------------------------------------
	protected  void addFloat()
	{
		addFloatParam(GAME_DEFINE_FLOAT.GDF_SOCKET_PORT);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_BROADCAST_PORT);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_LOAD_RESOURCES);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_LOG_LEVEL);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_ENABLE_KEYBOARD);
		if (mFloatNameToDefine.Count != (int)GAME_DEFINE_FLOAT.GDF_FRAME_MAX - (int)GAME_DEFINE_FLOAT.GDF_FRAME_MIN - 1)
		{
			logError("not all float parameter added!");
		}
	}
}