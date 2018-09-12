using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;
using System.Globalization;
using System.IO;

public class GameUtility : GameBase
{
	protected static int mIDMaker;
	public static float calcuteConfigExpression(GAME_DEFINE_STRING CommonDefine, float variableValue)
	{
		string variableStr = "(" + StringUtility.floatToString(variableValue, 2) + ")";
		string expression = mGameConfig.getStringParam(CommonDefine);
		expression = expression.Replace("i", variableStr);
		float expressionValue = MathUtility.calculateFloat(expression);
		return expressionValue;
	}
	public static int makeID() { return ++mIDMaker; }
	public static void messageOK(string info, bool delay = false, string buttonLabel = "")
	{
	}
}