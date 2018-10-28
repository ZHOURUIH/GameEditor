using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

public class GameFloatConfig : ConfigBase
{
	public GameFloatConfig(string name)
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
		addFloatParam(GAME_DEFINE_FLOAT.GDF_WIRELESS_PORT);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_KICK_OUT_TIME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_RUNNING_COUNT_DOWN);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_CIRCLE_COUNT);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_COACH_KICK_OUT_TIME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_ALL_MATCH_MAX_LAST_TIME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_SEND_FRICTION_INTERVAL);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_READ_POWER);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_VOLUME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_DIFFICULTY_LEVEL);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_DEFAULT_FRICTION);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_FIRST_MODE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_RACE_TIME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_LOAD_ASYNC);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_FAKE_HEART_RATE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_HEART_RATE_KICK_OUT_TIME);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_BINARY_CODE_X);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_BINARY_CODE_Y);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_UPLOAD_LOG);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_ENABLE_WIRELESS_CADENCE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_VIRTUAL_RIDE_SPEED_SCALE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_NORMAL_FRICTION);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MIN_UPHILL_ANGLE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MAX_UPHILL_ANGLE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MIN_DOWNHILL_ANGLE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MAX_DOWNHILL_ANGLE);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MIN_UPHILL_FRICTION);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MAX_UPHILL_FRICTION);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MIN_DOWNHILL_FRICTION);
		addFloatParam(GAME_DEFINE_FLOAT.GDF_MAX_DOWNHILL_FRICTION);
		if (mFloatNameToDefine.Count != (int)GAME_DEFINE_FLOAT.GDF_GAME_MAX - (int)GAME_DEFINE_FLOAT.GDF_GAME_MIN - 1)
		{
			logError("not all float parameter added!");
		}
	}
}  