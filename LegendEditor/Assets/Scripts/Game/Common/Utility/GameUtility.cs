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
		string variableStr = "(" + floatToString(variableValue, 2) + ")";
		string expression = mGameConfig.getStringParam(CommonDefine);
		expression = expression.Replace("i", variableStr);
		float expressionValue = calculateFloat(expression);
		return expressionValue;
	}
	public static int makeID() { return ++mIDMaker; }
	public static void messageOK(string info, bool delay = false, string buttonLabel = "")
	{
	}
	public static string getClothLabel(int id)
	{
		ClothData data;
		mSQLiteCloth.query(id, out data);
		return data.mLabel;
	}
	public static string getClothResource(int id)
	{
		ClothData data;
		mSQLiteCloth.query(id, out data);
		return data.mResource;
	}
	public static string getWeaponLabel(int id)
	{
		WeaponData data;
		mSQLiteWeapon.query(id, out data);
		return data.mLabel;
	}
	public static string getWeaponResource(int id)
	{
		WeaponData data;
		mSQLiteWeapon.query(id, out data);
		return data.mResource;
	}
	public static void sortByFileNumber(List<string> fileList)
	{
		// 根据文件名的数字进行排序
		int totalCount = 0;
		SortedDictionary<string, SortedDictionary<int, string>> sortedList = new SortedDictionary<string, SortedDictionary<int, string>>();
		int count = fileList.Count;
		for (int i = 0; i < count; ++i)
		{
			string curStr = fileList[i];
			string preStr = getNotNumberSubString(curStr);
			if (!sortedList.ContainsKey(preStr))
			{
				sortedList.Add(preStr, new SortedDictionary<int, string>());
			}
			int number = getLastNumber(curStr);
			if (!sortedList[preStr].ContainsKey(number))
			{
				sortedList[preStr].Add(number, curStr);
				++totalCount;
			}
		}
		if (totalCount != fileList.Count)
		{
			return;
		}
		fileList.Clear();
		foreach(var item in sortedList)
		{
			foreach(var item0 in item.Value)
			{
				fileList.Add(item0.Value);
			}
		}
	}
	public static Vector2 getPositionByFile(string fileNameNoSuffix)
	{
		string str = openTxtFile(fileNameNoSuffix + ".txt");
		return stringToVector2(str);
	}
}