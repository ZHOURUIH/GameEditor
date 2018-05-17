#include "MathUtility.h"
#include "StringUtility.h"

float MathUtility::calculateFloat(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
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
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			float ret = calculateFloat(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string floatStr = StringUtility::floatToString(ret, 4);
			str = StringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, floatStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = StringUtility::strReplace(str, i, i + 1, "");
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
		else
		{
			break;
		}
	}

	txVector<float> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			float fNum = StringUtility::stringToFloat(num);
			numbers.push_back(fNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i);
				float fNum = StringUtility::stringToFloat(num);
				numbers.push_back(fNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
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
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
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
		if (factors.size() == 0)
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
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

int MathUtility::calculateInt(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%'
			&& str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	for (int i = 0; i < (int)str.length(); ++i)
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
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			int ret = calculateInt(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string intStr = StringUtility::intToString(ret, 4);
			str = StringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, intStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = StringUtility::strReplace(str, i, i + 1, "");
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
		else
		{
			break;
		}
	}

	txVector<int> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	for (int i = 0; i < (int)str.length(); ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == str.length() - 1)
		{
			std::string num = str.substr(beginpos, str.length() - beginpos);
			int iNum = StringUtility::stringToInt(num);
			numbers.push_back(iNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i);
				int iNum = StringUtility::stringToInt(num);
				numbers.push_back(iNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				int num1 = numbers[i];
				int num2 = numbers[i + 1];
				int num3 = 0;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				else if (factors[i] == '%')
				{
					num3 = num1 % num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
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
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			int num1 = numbers[0];
			int num2 = numbers[1];
			int num3 = 0;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}