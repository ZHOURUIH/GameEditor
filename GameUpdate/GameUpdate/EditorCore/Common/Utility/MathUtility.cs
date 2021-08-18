using System;
using System.Collections.Generic;
using System.Text;

public class MathUtility : StringUtility
{
	public static float KMHtoMS(float kmh) { return kmh / 3.6f; }		// km/h转m/s
	public static float MStoKMH(float ms) { return ms * 3.6f; }
	public static float MtoKM(float m) { return m / 1000.0f; }
	public static int abs(int v) { return v >= 0 ? v : -v; }
	public static float abs(float v) { return v >= 0.0f ? v : -v; }
	public static float calculateFloat(string str)
	{
		// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
		str = checkFloatString(str, "+-*/()");
		// 判断左右括号数量是否相等
		int leftBracketCount = 0;
		int rightBracketCount = 0;
		int newStrLen = str.Length;
		for (int i = 0; i < newStrLen; ++i)
		{
			if (str[i] == '(')
			{
				++leftBracketCount;
			}
			else if (str[i] == ')')
			{
				++rightBracketCount;
			}
		}
		if (leftBracketCount != rightBracketCount)
		{
			// 计算错误,左右括号数量不对应
			return 0;
		}

		// 循环判断传入的字符串有没有括号
		while (true)
		{
			// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
			if (str.IndexOf("(") == -1 && str.IndexOf(")") == -1)
			{
				break;
			}
			int curPos = str.LastIndexOf("(");
			string strInBracket = str.Substring(curPos + 1);
			strInBracket = strInBracket.Substring(0, strInBracket.IndexOf(")"));
			float ret = calculateFloat(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = ret < 0;
			ret = abs(ret);
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			str = strReplace(str, curPos, curPos + strInBracket.Length + 2, Math.Round(ret, 4).ToString());
			byte[] strchar = stringToBytes(str, Encoding.ASCII);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curPos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (strchar[i] == '+')
					{
						strchar[i] = (byte)'-';
						str = bytesToString(strchar, Encoding.ASCII);
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (strchar[i] == '-')
					{
						if (strchar[i - 1] >= '0' && strchar[i - 1] <= '9')
						{
							strchar[i] = (byte)'+';
							str = bytesToString(strchar, Encoding.ASCII);
						}
						else
						{
							str = strReplace(str, i, i + 1, "");
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		List<float> numbers = new List<float>();
		List<char> factors = new List<char>();
		// 表示上一个运算符的下标+1
		int beginPos = 0;
		for (int i = 0; i < str.Length; ++i)
		{
			// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
			if (i == str.Length - 1)
			{
				numbers.Add(float.Parse(str.Substring(beginPos)));
				break;
			}
			// 找到第一个运算符
			if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
			{
				if (i != 0)
				{
					numbers.Add(float.Parse(str.Substring(beginPos, i - beginPos)));
				}
				// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
				else
				{
					numbers.Add(0);
				}
				factors.Add(str[i]);
				beginPos = i + 1;
			}
		}
		// 计算错误,运算符与数字数量不符
		if (factors.Count + 1 != numbers.Count)
		{
			return 0;
		}
		// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
		while (true)
		{
			// 表示是否还有乘除表达式
			bool hasMS = false;
			for (int i = 0; i < factors.Count; ++i)
			{
				// 先遍历到哪个就先计算哪个
				if (factors[i] == '*' || factors[i] == '/')
				{
					// 第一个运算数的下标与运算符的下标是相同的
					float num1 = numbers[i];
					float num2 = numbers[i + 1];
					float num3 = 0.0f;
					if (factors[i] == '*')
					{
						num3 = num1 * num2;
					}
					else if (factors[i] == '/')
					{
						num3 = num1 / num2;
					}
					// 删除第i + 1个数,然后将第i个数替换为计算结果
					numbers.RemoveAt(i + 1);
					if (numbers.Count == 0)
					{
						// 计算错误
						return 0;
					}
					numbers[i] = num3;
					// 删除第i个运算符
					factors.RemoveAt(i);
					hasMS = true;
					break;
				}
			}
			if (!hasMS)
			{
				break;
			}
		}
		// 再计算加减法
		while (true)
		{
			if (factors.Count == 0)
			{
				break;
			}
			if (factors[0] == '+' || factors[0] == '-')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				float num1 = numbers[0];
				float num2 = numbers[1];
				float num3 = 0.0f;
				if (factors[0] == '+')
				{
					num3 = num1 + num2;
				}
				else if (factors[0] == '-')
				{
					num3 = num1 - num2;
				}
				// 删除第1个数,然后将第0个数替换为计算结果
				numbers.RemoveAt(1);
				if (numbers.Count == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[0] = num3;
				// 删除第0个运算符
				factors.RemoveAt(0);
			}
		}
		// 计算错误
		if (numbers.Count != 1)
		{
			return 0;
		}
		return numbers[0];
	}
	// 得到大于等于value的第一个整数,只能是0或者整数
	public static int getForwardInt(float value)
	{
		if (value >= 0.0f)
		{
			int intValue = (int)value;
			if (value - intValue > 0.0f)
			{
				return intValue + 1;
			}
			return intValue;
		}
		return (int)value;
	}
	public static void checkInt(ref float value, float precision = 0.0001f)
	{
		// 先判断是否为0
		if (isFloatZero(value, precision))
		{
			value = 0.0f;
			return;
		}
		int intValue = (int)value;
		// 大于0
		if (value > 0.0f)
		{
			// 如果原值减去整数值小于0.5f,则表示原值可能接近于整数值
			if (value - intValue < 0.5f)
			{
				if (isFloatZero(value - intValue, precision))
				{
					value = intValue;
				}
			}
			// 如果原值减去整数值大于0.5f, 则表示原值可能接近于整数值+1
			else
			{
				if (isFloatZero(value - (intValue + 1), precision))
				{
					value = intValue + 1;
				}
			}
		}
		// 小于0
		else if (value < 0.0f)
		{
			// 如果原值减去整数值的结果的绝对值小于0.5f,则表示原值可能接近于整数值
			if (Math.Abs(value - intValue) < 0.5f)
			{
				if (isFloatZero(value - intValue, precision))
				{
					value = intValue;
				}
			}
			else
			{
				// 如果原值减去整数值的结果的绝对值大于0.5f, 则表示原值可能接近于整数值-1
				if (isFloatZero(value - (intValue - 1), precision))
				{
					value = intValue - 1;
				}
			}
		}
	}
	public static int getMin(int a, int b) { return a < b ? a : b; }
	public static int getMax(int a, int b) { return a > b ? a : b; }
	public static float getMin(float a, float b) { return a < b ? a : b; }
	public static float getMax(float a, float b) { return a > b ? a : b; }
	public static float inverseLerp(float a, float b, float value) { return (value - a) / (b - a); }
	public static float lerp(float start, float end, float t)
	{
		clamp(ref t, 0.0f, 1.0f);
		return start + (end - start) * t;
	}
	public static void clamp(ref float value, float min, float max)
	{
		if (min > max || isFloatEqual(min, max))
		{
			value = min;
			return;
		}
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}
	}
	public static void clamp(ref int value, int min, int max)
	{
		if(min > max)
		{
			return;
		}
		if (min == max)
		{
			value = min;
			return;
		}
		if (value < min)
		{
			value = min;
		}
		else if (value > max)
		{
			value = max;
		}
	}
	public static bool isFloatZero(float value, float precision = 0.0001f)
	{
		return value >= -precision && value <= precision;
	}
	public static bool isFloatEqual(float value1, float value2, float precision = 0.0001f)
	{
		return isFloatZero(value1 - value2, precision);
	}
	public static void clampValue(ref float value, float min, float max, float cycle)
	{
		while (value < min)
		{
			value += cycle;
		}
		while (value > max)
		{
			value -= cycle;
		}
	}
	public static void clampAngle(ref float angle, float min, float max, float pi)
	{
		clampValue(ref angle, min, max, pi * 2.0f);
	}
	public static bool isInRange(float value, float range0, float range1)
	{
		return value >= getMin(range0, range1) && value <= getMax(range0, range1);
	}
	public static void swap<T>(ref T value0, ref T value1)
	{
		T temp = value0;
		value0 = value1;
		value1 = temp;
	}
	public static void secondsToMinutesSeconds(int seconds, out int outMin, out int outSec)
	{
		outMin = seconds / 60;
		outSec = seconds - outMin * 60;
	}
	public static void secondsToHoursMinutesSeconds(int seconds, out int outHour, out int outMin, out int outSec)
	{
		outHour = seconds / 3600;
		outMin = (seconds - outHour * 3600) / 60;
		outSec = seconds - outHour * 3600 - outMin * 60;
	}
	//将时间转化成时间戳
	public static long convertDateTimeUnixTime(DateTime dateTime) 
	{
		DateTime startTime = TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1)); // 当地时区
		long timeStamp = (long)(dateTime - startTime).TotalSeconds; // 相差秒数
		return timeStamp;
	}
	//将时间戳转化成时间
	public static DateTime ConvertUnixTimeDateTime(long unixTimeStamp)
	{
		DateTime startTime = TimeZone.CurrentTimeZone.ToLocalTime(new DateTime(1970, 1, 1)); // 当地时区
		DateTime dt = startTime.AddSeconds(unixTimeStamp);
		return dt;
	}
}