using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

public class ApplicationConfig : ConfigBase
{
	public ApplicationConfig(string name)
		: base(name)
	{
		mFloatOrString = true;
	}
	public override void init()
	{
		base.init();
		addFloat();
	}
	//---------------------------------------------------------------------------------------------------------------------------------
	protected  void addFloat()
	{
		addFloatParam(GAME_DEFINE_FLOAT.GDF_FULL_SCREEN);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_SCREEN_WIDTH);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_SCREEN_HEIGHT);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_ADAPT_SCREEN);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_SCREEN_COUNT);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_USE_FIXED_TIME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_FIXED_TIME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_VSYNC);
		if (mFloatNameToDefine.Count != (int)GAME_DEFINE_FLOAT.GDF_APPLICATION_MAX - (int)GAME_DEFINE_FLOAT.GDF_APPLICATION_MIN - 1)
		{
			logError("not all float parameter added!");
		}
	}
}