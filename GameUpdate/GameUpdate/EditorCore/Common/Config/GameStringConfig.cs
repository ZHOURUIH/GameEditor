using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

public class GameStringConfig : ConfigBase
{
	public GameStringConfig(string name)
		: base(name)
	{
		mFloatOrString = false;
	}
	public override void init()
	{
		base.init();
		addString();
	}
	//-----------------------------------------------------------------------------------------------------------------------
	protected  void addString()
	{
		addStringParam(GAME_DEFINE_STRING.GDS_REGISTE_CODE);
		addStringParam(GAME_DEFINE_STRING.GDS_REAL_SPEED_TO_VIDEO_SPEED);
		addStringParam(GAME_DEFINE_STRING.GDS_CIRCLE_TIME_MATCH);
		addStringParam(GAME_DEFINE_STRING.GDS_CIRCLE_TIME_COMBAT);
		addStringParam(GAME_DEFINE_STRING.GDS_SPEED_LEVEL_RANGE);
		addStringParam(GAME_DEFINE_STRING.GDS_LOADING_TIP);
		addStringParam(GAME_DEFINE_STRING.GDS_REWARDS);
		addStringParam(GAME_DEFINE_STRING.GDS_MODE_COUNT);
		addStringParam(GAME_DEFINE_STRING.GDS_SPEED_RATIO);
		addStringParam(GAME_DEFINE_STRING.GDS_CALCULATE_ROLL);
		if (mStringNameToDefine.Count != (int)GAME_DEFINE_STRING.GDS_GAME_MAX - (int)GAME_DEFINE_STRING.GDS_GAME_MIN - 1)
		{
			logError("not all string parameter added!");
		}
	}
}