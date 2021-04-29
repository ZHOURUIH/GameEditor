#include "StringUtility.h"
#include "MathUtility.h"

const char StringUtility::BOM[4]{ (char)0xEF, (char)0xBB, (char)0xBF, 0 };
array<string, 4097> StringUtility::mIntString;
const array<ullong, 11> StringUtility::POWER_INT_10{ 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 10000000000 };
const array<ullong, 20> StringUtility::POWER_ULLONG_10
{
	1ul,
	10ul,
	100ul,
	1000ul,
	10000ul,
	100000ul,
	1000000ul,
	10000000ul,
	100000000ul,
	1000000000ul,
	10000000000ul,
	100000000000ul,
	1000000000000ul,
	10000000000000ul,
	100000000000000ul,
	1000000000000000ul,
	10000000000000000ul,
	100000000000000000ul,
	1000000000000000000ul,
	10000000000000000000ul
};

void StringUtility::initStringUtility()
{
	FOR_I(mIntString.size())
	{
		char buffer[16];
		_itoa_s(i, buffer, 16);
		mIntString[i] = buffer;
	}
}

string StringUtility::removeSuffix(const string& str)
{
	size_t dotPos = str.find_last_of('.');
	if (dotPos != NOT_FIND)
	{
		return str.substr(0, dotPos);
	}
	return str;
}

void StringUtility::removeLastComma(string& stream)
{
	uint streamSize = (uint)stream.length();
	FOR_I(streamSize)
	{
		if (stream[streamSize - i - 1] == ',')
		{
			stream.erase(streamSize - i - 1, 1);
			break;
		}
	}
}

string StringUtility::getFileName(string str)
{
	rightToLeft(str);
	size_t dotPos = str.find_last_of('/');
	if (dotPos != NOT_FIND)
	{
		return str.substr(dotPos + 1, str.length() - 1);
	}
	return str;
}

string StringUtility::getFileNameNoSuffix(string str, bool removePath)
{
	rightToLeft(str);
	size_t dotPos = str.find_last_of('.');
	if (removePath)
	{
		size_t namePos = str.find_last_of('/');
		if (namePos != NOT_FIND)
		{
			if (dotPos != NOT_FIND)
			{
				return str.substr(namePos + 1, dotPos - namePos - 1);
			}
			else
			{
				return str.substr(namePos + 1);
			}
		}
	}
	else
	{
		if (dotPos != NOT_FIND)
		{
			return str.substr(0, dotPos);
		}
	}
	return str;
}

string StringUtility::getFirstFolderName(const string& dir)
{
	string temp = dir;
	rightToLeft(temp);
	size_t index = (int)temp.find_first_of('/');
	if (index == NOT_FIND)
	{
		return dir;
	}
	return temp.substr(0, index);
}

string StringUtility::removeFirstPath(const string& dir)
{
	string temp = dir;
	rightToLeft(temp);
	size_t index = temp.find_first_of('/');
	if (index == NOT_FIND)
	{
		return dir;
	}
	return temp.substr(index + 1, temp.length() - index - 1);
}

string StringUtility::getFilePath(const string& dir)
{
	string tempDir = dir;
	rightToLeft(tempDir);
	size_t pos = tempDir.find_last_of('/');
	return pos != NOT_FIND ? dir.substr(0, pos) : "./";
}

string StringUtility::getFileSuffix(const string& fileName)
{
	size_t dotPos = fileName.find_last_of('.');
	if (dotPos == NOT_FIND)
	{
		return fileName;
	}
	return fileName.substr(dotPos + 1, fileName.length() - dotPos - 1);
}

string StringUtility::removeStartString(const string& fileName, const string& startStr)
{
	if (startWith(fileName, startStr.c_str()))
	{
		return fileName.substr(startStr.length(), fileName.length() - startStr.length());
	}
	return fileName;
}

string StringUtility::removeEndString(const string& fileName, const string& endStr)
{
	if (endWith(fileName, endStr.c_str()))
	{
		return fileName.substr(0, fileName.length() - endStr.length());
	}
	return fileName;
}

int StringUtility::getLastNotNumberPos(const string& str)
{
	uint strLen = (uint)str.length();
	FOR_I(strLen)
	{
		if (str[strLen - i - 1] > '9' || str[strLen - i - 1] < '0')
		{
			return strLen - i - 1;
		}
	}
	return -1;
}

int StringUtility::getLastNumber(const string& str)
{
	size_t lastPos = getLastNotNumberPos(str);
	if (lastPos == NOT_FIND)
	{
		return -1;
	}
	string numStr = str.substr(lastPos + 1, str.length() - lastPos - 1);
	if (numStr.length() == 0)
	{
		return 0;
	}
	return stringToInt(numStr);
}

int StringUtility::getLastChar(const char* str, char value)
{
	uint length = (uint)strlen(str);
	FOR_I(length)
	{
		if (str[length - i - 1] == value)
		{
			return length - i - 1;
		}
	}
	return -1;
}

int StringUtility::getLastNotChar(const char* str, char value)
{
	uint length = (uint)strlen(str);
	FOR_I(length)
	{
		if (str[length - i - 1] != value)
		{
			return length - i - 1;
		}
	}
	return -1;
}

void StringUtility::split(const char* str, const char* key, myVector<string>& vec, bool removeEmpty)
{
	int startPos = 0;
	uint keyLen = (uint)strlen(key);
	uint sourceLen = (uint)strlen(str);
	const int STRING_BUFFER = 1024;
	char curString[STRING_BUFFER];
	int devidePos = -1;
	bool ret = true;
	while (ret)
	{
		ret = findString(str, key, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		if (devidePos - startPos >= STRING_BUFFER)
		{
			ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + intToString(STRING_BUFFER) + "字节");
			return;
		}
		MEMCPY(curString, STRING_BUFFER, str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 放入列表
		if (curString[0] != '\0' || !removeEmpty)
		{
			vec.push_back(curString);
		}
	}
}

uint StringUtility::split(const char* str, const char* key, string* stringBuffer, uint bufferSize, bool removeEmpty)
{
	int startPos = 0;
	int keyLen = (uint)strlen(key);
	int sourceLen = (uint)strlen(str);
	const int STRING_BUFFER = 1024;
	char curString[STRING_BUFFER];
	int devidePos = -1;
	uint curCount = 0;
	bool ret = true;
	while (ret)
	{
		ret = findString(str, key, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		if (devidePos - startPos >= STRING_BUFFER)
		{
			ERROR("分隔字符串失败,缓冲区太小,当前缓冲区为" + intToString(STRING_BUFFER) + "字节");
			return 0;
		}
		MEMCPY(curString, STRING_BUFFER, str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 放入列表
		if (curString[0] == '\0' || !removeEmpty)
		{
			continue;
		}
		if (curCount >= bufferSize)
		{
			ERROR("string buffer is too small! bufferSize:" + intToString(bufferSize));
			break;
		}
		stringBuffer[curCount++] = curString;
	}
	return curCount;
}

void StringUtility::replace(char* str, int strBufferSize, int begin, int end, const char* reStr)
{
	int curLength = (int)strlen(str);
	int replaceLength = (int)strlen(reStr);
	if (begin + replaceLength + curLength - end >= strBufferSize)
	{
		ERROR("buffer is too small!");
		return;
	}
	if (begin + replaceLength != end)
	{
		memmove(str + begin + replaceLength, str + end, curLength - end);
	}
	MEMCPY(str + begin, strBufferSize - begin, reStr, replaceLength);
}

void StringUtility::replace(string& str, int begin, int end, const string& reStr)
{
	int replaceLength = (int)reStr.length();
	if (end - begin == replaceLength)
	{
		MEMCPY((void*)(str.c_str() + begin), str.length() - begin, reStr.c_str(), replaceLength);
	}
	else
	{
		str = str.substr(0, begin) + reStr + str.substr(end, str.length() - end);
	}
}

void StringUtility::replaceAll(char* str, int strBufferSize, const char* key, const char* newWords)
{
	uint keyLength = (uint)strlen(key);
	uint newWordsLength = (uint)strlen(newWords);
	int startPos = 0;
	while (true)
	{
		int pos = 0;
		if (!findString(str, key, &pos, startPos))
		{
			break;
		}
		replace(str, strBufferSize, pos, pos + keyLength, newWords);
		startPos = pos + newWordsLength;
	}
}

void StringUtility::replaceAll(string& str, const string& key, const string& newWords)
{
	uint keyLength = (uint)key.length();
	uint newWordsLength = (uint)newWords.length();
	int startPos = 0;
	while (true)
	{
		int pos = 0;
		if (!findString(str, key, &pos, startPos))
		{
			break;
		}
		replace(str, pos, pos + keyLength, newWords);
		startPos = pos + newWordsLength;
	}
}

void StringUtility::removeAll(string& str, char value)
{
	int strLength = (uint)str.length();
	char* tempBuffer = newCharArray(MathUtility::getGreaterPower2(strLength + 1));
	setString(tempBuffer, strLength + 1, str.c_str(), strLength);
	for (int i = strLength; i >= 0; --i)
	{
		if (tempBuffer[i] == value)
		{
			// 移动数据
			if (i != strLength - 1)
			{
				memmove(tempBuffer + i, tempBuffer + i + 1, strLength - i - 1);
			}
			--strLength;
		}
	}
	tempBuffer[strLength] = '\0';
	str.assign(tempBuffer);
	deleteCharArray(tempBuffer);
}

void StringUtility::removeAll(string& str, char value0, char value1)
{
	int strLength = (uint)str.length();
	char* tempBuffer = newCharArray(MathUtility::getGreaterPower2(strLength + 1));
	setString(tempBuffer, strLength + 1, str.c_str(), strLength);
	for (int i = strLength; i >= 0; --i)
	{
		if (tempBuffer[i] == value0 || tempBuffer[i] == value1)
		{
			// 移动数据
			if (i != strLength - 1)
			{
				memmove(tempBuffer + i, tempBuffer + i + 1, strLength - i - 1);
			}
			--strLength;
		}
	}
	tempBuffer[strLength] = '\0';
	str.assign(tempBuffer);
	deleteCharArray(tempBuffer);
}

bool StringUtility::removeString(char* str, uint length, const char* subString)
{
	int subPos = 0;
	if (!findString(str, subString, &subPos, 0))
	{
		return false;
	}
	// 从子字符串的位置,后面的数据覆盖前面的数据
	uint subLength = (uint)strlen(subString);
	memmove(str + subPos, str + subPos + subLength, length - subLength - subPos);
	return true;
}

// 从一个ullong数组的字符串中移除指定的value的字符串
bool StringUtility::removeULLongsString(char* valueArrayString, uint length, ullong value)
{
	ULLONG_STR(valueString, value);
	// 如果value是在最后,则只移除value字符串
	if (endWith(valueArrayString, valueString))
	{
		return removeString(valueArrayString, length, valueString);
	}
	// value不在最后,则移除value字符串加后面的逗号
	else
	{
		array<char, 32> needRemoveString{ 0 };
		STR_APPEND2(needRemoveString, valueString, ",");
		return removeString(valueArrayString, length, needRemoveString.data());
	}
}

// 将value添加到一个ullong数组的字符串中
void StringUtility::addULLongsString(char* valueArrayString, uint length, ullong value)
{
	ULLONG_CHARS(idStr, value);
	if (valueArrayString[0] != '\0')
	{
		STR_APPEND2_N(valueArrayString, length, ",", idStr.data());
	}
	else
	{
		MEMCPY(valueArrayString, length, idStr.data(), idStr.size());
	}
}

void StringUtility::_itoa_s(int value, char* charArray, uint size)
{
	if (value == 0)
	{
		charArray[0] = '0';
		charArray[1] = '\0';
		return;
	}
	int sign = 1;
	if (value < 0)
	{
		value = -value;
		sign = -1;
	}
	if ((ullong)value > POWER_INT_10[POWER_INT_10.size() - 1])
	{
		ERROR("int value is too large:" + ullongToString((ullong)value));
		return;
	}
	uint index = 0;
	while (true)
	{
		// 如果是正数,则数字个数不能超过size - 1
		// 如果是负数,则数字个数不能超过size - 2
		if ((sign > 0 && index >= size) ||
			(sign < 0 && index >= size - 1))
		{
			ERROR("buffer is too small!");
			break;
		}
		// 将数字放入numberArray的尾部
		if ((ullong)value < POWER_INT_10[index])
		{
			break;
		}
		charArray[size - 1 - index] = (int)((ullong)value % POWER_INT_10[index + 1] / POWER_INT_10[index]);
		++index;
	}
	// 将数字从数组末尾移动到开头,并且将数字转换为数字字符
	if (sign > 0)
	{
		uint lengthToHead = size - index;
		FOR_I(index)
		{
			charArray[i] = charArray[i + lengthToHead] + '0';
		}
		charArray[index] = '\0';
	}
	else
	{
		uint lengthToHead = size - index;
		FOR_I(index)
		{
			charArray[i + 1] = charArray[i + lengthToHead] + '0';
		}
		charArray[0] = '-';
		charArray[index + 1] = '\0';
	}
}

void StringUtility::_i64toa_s(ullong value, char* charArray, uint size)
{
	if (value == 0)
	{
		charArray[0] = '0';
		charArray[1] = '\0';
		return;
	}
	if (value > POWER_ULLONG_10[POWER_ULLONG_10.size() - 1])
	{
		ERROR("long long value is too large");
		return;
	}
	uint index = 0;
	while (true)
	{
		// 如果是正数,则数字个数不能超过size - 1
		if (index >= size)
		{
			ERROR("buffer is too small!");
			break;
		}
		// 将数字放入numberArray的尾部
		if (value < POWER_ULLONG_10[index])
		{
			break;
		}
		charArray[size - 1 - index] = (int)(value % POWER_ULLONG_10[index + 1] / POWER_ULLONG_10[index]);
		++index;
	}
	// 将数字从数组末尾移动到开头,并且将数字转换为数字字符
	uint lengthToHead = size - index;
	FOR_I(index)
	{
		charArray[i] = charArray[i + lengthToHead] + '0';
	}
	charArray[index] = '\0';
}

void StringUtility::strcat_s(char* destBuffer, uint size, const char* source)
{
	if (source == NULL || destBuffer == NULL)
	{
		return;
	}
	uint destIndex = (uint)-1;
	FOR_I((uint)size)
	{
		// 找到字符串的末尾
		if (destBuffer[i] == '\0')
		{
			destIndex = i;
			break;
		}
	}
	if (destIndex == (uint)-1)
	{
		return;
	}
	uint index = 0;
	while (true)
	{
		if (destIndex >= (uint)size)
		{
			ERROR("strcat_s buffer is too small");
			break;
		}
		destBuffer[destIndex] = source[index];
		if (source[index] == '\0')
		{
			break;
		}
		++index;
		++destIndex;
	}
}

void StringUtility::strcat_s(char* destBuffer, uint size, const char** sourceArray, uint sourceCount)
{
	if (sourceArray == NULL || destBuffer == NULL)
	{
		return;
	}
	uint destIndex = (uint)-1;
	FOR_I((uint)size)
	{
		// 找到字符串的末尾
		if (destBuffer[i] == '\0')
		{
			destIndex = i;
			break;
		}
	}
	if (destIndex == (uint)-1)
	{
		return;
	}
	FOR_I(sourceCount)
	{
		uint index = 0;
		const char* curSource = sourceArray[i];
		if (curSource == NULL)
		{
			continue;
		}
		while (true)
		{
			if (destIndex >= (uint)size)
			{
				ERROR("strcat_s buffer is too small");
				break;
			}
			destBuffer[destIndex] = curSource[index];
			if (curSource[index] == '\0')
			{
				break;
			}
			++index;
			++destIndex;
		}
	}
}

void StringUtility::strcpy_s(char* destBuffer, uint size, const char* source)
{
	if (source == NULL)
	{
		return;
	}
	uint index = 0;
	while (true)
	{
		if (index >= size)
		{
			ERROR("strcat_s buffer is too small");
			break;
		}
		destBuffer[index] = source[index];
		if (source[index] == '\0')
		{
			break;
		}
		++index;
	}
}

string StringUtility::intToString(int value, uint limitLen)
{
	array<char, 16> temp{ 0 };
	_itoa_s(value, temp);
	uint len = (uint)strlen(temp.data());
	if (limitLen > len)
	{
		return zeroString(limitLen - len) + temp.data();
	}
	return temp.data();
}

void StringUtility::intToString(char* charArray, uint size, int value, uint limitLen)
{
	if (limitLen == 0)
	{
		_itoa_s(value, charArray, size);
		return;
	}
	// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
	array<char, 16> temp{ 0 };
	_itoa_s(value, temp);
	// 判断是否需要在前面补0
	if (limitLen > 0)
	{
		uint len = (uint)strlen(temp.data());
		if (limitLen > len)
		{
			// 因为当前函数设计为线程安全,所以只能使用栈内存中的数组
			array<char, 16> zeroArray{ 0 };
			zeroString(zeroArray, limitLen - len);
			STRCAT2_N(charArray, size, zeroArray.data(), temp.data());
			return;
		}
	}
	strcpy_s(charArray, size, temp.data());
}

string StringUtility::ullongToString(ullong value, uint limitLen)
{
	array<char, 32> temp{ 0 };
	_i64toa_s(value, temp);
	uint len = (uint)strlen(temp.data());
	if (limitLen > len)
	{
		return zeroString(limitLen - len) + temp.data();
	}
	return temp.data();
}

void StringUtility::ullongToString(char* charArray, uint size, ullong value, uint limitLen)
{
	if (limitLen == 0)
	{
		_i64toa_s(value, charArray, size);
		return;
	}
	array<char, 32> temp{ 0 };
	_i64toa_s(value, temp);
	// 判断是否需要在前面补0
	if (limitLen > 0)
	{
		uint len = (uint)strlen(temp.data());
		if (limitLen > len)
		{
			array<char, 16> zeroArray{ 0 };
			zeroString(zeroArray, limitLen - len);
			STRCAT2_N(charArray, size, zeroArray.data(), temp.data());
			return;
		}
	}
	strcpy_s(charArray, size, temp.data());
}

string StringUtility::ullongsToString(ullong* valueList, uint llongCount, uint limitLen, const char* seperate)
{
	// 根据列表长度选择适当的数组长度,每个llong默认数字长度为32个字符
	int arrayLen = 32 * MathUtility::getGreaterPower2(llongCount);
	char* charArray = newCharArray(arrayLen);
	charArray[0] = 0;
	array<char, 32> temp{ 0 };
	FOR_I(llongCount)
	{
		ullongToString(temp, valueList[i], limitLen);
		if (i != llongCount - 1)
		{
			STR_APPEND2_N(charArray, arrayLen, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(charArray, arrayLen, temp.data());
		}
	}
	string str(charArray);
	deleteCharArray(charArray);
	return str;
}

void StringUtility::ullongsToString(char* buffer, uint bufferSize, const ullong* valueList, uint count, const char* seperate)
{
	buffer[0] = '\0';
	if (valueList == NULL || count == 0)
	{
		return;
	}
	array<char, 32> temp{ 0 };
	FOR_I(count)
	{
		ullongToString(temp, valueList[i]);
		if (i != count - 1)
		{
			STR_APPEND2_N(buffer, bufferSize, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(buffer, bufferSize, temp.data());
		}
	}
}

string StringUtility::uintsToString(uint* valueList, uint count, uint limitLen, const char* seperate)
{
	if (valueList == NULL || count == 0)
	{
		return "";
	}
	// 根据列表长度选择适当的数组长度,每个uint默认数字长度为16个字符
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	charArray[0] = 0;
	array<char, 16> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i], limitLen);
		if (i != count - 1)
		{
			STR_APPEND2_N(charArray, arrayLen, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(charArray, arrayLen, temp.data());
		}
	}
	string str(charArray);
	deleteCharArray(charArray);
	return str;
}

void StringUtility::uintsToString(char* buffer, uint bufferSize, uint* valueList, uint count, const char* seperate)
{
	buffer[0] = '\0';
	if (valueList == NULL || count == 0)
	{
		return;
	}
	array<char, 16> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i]);
		if (i != count - 1)
		{
			STR_APPEND2_N(buffer, bufferSize, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(buffer, bufferSize, temp.data());
		}
	}
}

string StringUtility::bytesToString(byte* valueList, uint count, uint limitLen, const char* seperate)
{
	if (valueList == NULL || count == 0)
	{
		return "";
	}
	int arrayLen = 4 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	charArray[0] = 0;
	array<char, 4> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i], limitLen);
		if (i != count - 1)
		{
			STR_APPEND2_N(charArray, arrayLen, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(charArray, arrayLen, temp.data());
		}
	}
	string str(charArray);
	deleteCharArray(charArray);
	return str;
}

void StringUtility::bytesToString(char* buffer, uint bufferSize, byte* valueList, uint count, const char* seperate)
{
	buffer[0] = '\0';
	if (valueList == NULL || count == 0)
	{
		return;
	}
	array<char, 4> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i]);
		if (i != count - 1)
		{
			STR_APPEND2_N(buffer, bufferSize, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(buffer, bufferSize, temp.data());
		}
	}
}

string StringUtility::ushortsToString(ushort* valueList, uint count, uint limitLen, const char* seperate)
{
	if (valueList == NULL || count == 0)
	{
		return "";
	}
	int arrayLen = 8 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	charArray[0] = 0;
	array<char, 8> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i], limitLen);
		if (i != count - 1)
		{
			STR_APPEND2_N(charArray, arrayLen, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(charArray, arrayLen, temp.data());
		}
	}
	string str(charArray);
	deleteCharArray(charArray);
	return str;
}

void StringUtility::ushortsToString(char* buffer, uint bufferSize, ushort* valueList, uint count, const char* seperate)
{
	buffer[0] = '\0';
	if (valueList == NULL || count == 0)
	{
		return;
	}
	array<char, 8> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i]);
		if (i != count - 1)
		{
			STR_APPEND2_N(buffer, bufferSize, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(buffer, bufferSize, temp.data());
		}
	}
}

string StringUtility::intsToString(int* valueList, uint count, uint limitLen, const char* seperate)
{
	if (valueList == NULL || count == 0)
	{
		return "";
	}
	// 根据列表长度选择适当的数组长度,每个int默认数字长度为16个字符
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	charArray[0] = 0;
	array<char, 16> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i], limitLen);
		if (i != count - 1)
		{
			STR_APPEND2_N(charArray, arrayLen, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(charArray, arrayLen, temp.data());
		}
	}
	string str(charArray);
	deleteCharArray(charArray);
	return str;
}

void StringUtility::intsToString(char* buffer, uint bufferSize, int* valueList, uint count, const char* seperate)
{
	buffer[0] = '\0';
	if (valueList == NULL || count == 0)
	{
		return;
	}
	array<char, 16> temp{ 0 };
	FOR_I(count)
	{
		intToString(temp, valueList[i]);
		if (i != count - 1)
		{
			STR_APPEND2_N(buffer, bufferSize, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(buffer, bufferSize, temp.data());
		}
	}
}

void StringUtility::stringToBytes(const string& str, myVector<byte>& valueList, const char* seperate)
{
	VECTOR(string, strList);
	split(str.c_str(), seperate, strList);
	FOR_VECTOR_CONST(strList)
	{
		valueList.push_back(stringToInt(strList[i]));
	}
	UN_VECTOR(string, strList);
}

uint StringUtility::stringToBytes(const char* str, byte* buffer, uint bufferSize, const char* seperate)
{
	uint curCount = 0;
	uint startPos = 0;
	uint keyLen = (uint)strlen(seperate);
	uint sourceLen = (uint)strlen(str);
	array<char, 4> curString{ 0 };
	int devidePos = -1;
	bool ret = true;
	while (ret)
	{
		ret = findString(str, seperate, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		MEMCPY(curString.data(), curString.size(), str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 转换为整数放入列表
		if (curString[0] == '\0')
		{
			continue;
		}
		if (curCount >= bufferSize)
		{
			ERROR("int buffer size is too small, bufferSize:" + intToString(bufferSize));
			break;
		}
		buffer[curCount++] = stringToInt(curString.data());
	}
	return curCount;
}

void StringUtility::stringToUShorts(const string& str, myVector<ushort>& valueList, const char* seperate)
{
	VECTOR(string, strList);
	split(str.c_str(), seperate, strList);
	valueList.reserve(strList.size());
	FOR_VECTOR_CONST(strList)
	{
		valueList.push_back(stringToInt(strList[i]));
	}
	UN_VECTOR(string, strList);
}

uint StringUtility::stringToUShorts(const char* str, ushort* buffer, uint bufferSize, const char* seperate)
{
	uint curCount = 0;
	uint startPos = 0;
	uint keyLen = (uint)strlen(seperate);
	uint sourceLen = (uint)strlen(str);
	array<char, 8> curString{ 0 };
	int devidePos = -1;
	bool ret = true;
	while (ret)
	{
		ret = findString(str, seperate, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		MEMCPY(curString.data(), curString.size(), str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 转换为整数放入列表
		if (curString[0] == '\0')
		{
			continue;
		}
		if (curCount >= bufferSize)
		{
			ERROR("int buffer size is too small, bufferSize:" + intToString(bufferSize));
			break;
		}
		buffer[curCount++] = stringToInt(curString.data());
	}
	return curCount;
}

void StringUtility::stringToInts(const string& str, myVector<int>& valueList, const char* seperate)
{
	VECTOR(string, strList);
	split(str.c_str(), seperate, strList);
	FOR_VECTOR_CONST(strList)
	{
		valueList.push_back(stringToInt(strList[i]));
	}
	UN_VECTOR(string, strList);
}

uint StringUtility::stringToInts(const char* str, int* buffer, uint bufferSize, const char* seperate)
{
	uint curCount = 0;
	uint startPos = 0;
	uint keyLen = (uint)strlen(seperate);
	uint sourceLen = (uint)strlen(str);
	array<char, 16> curString{ 0 };
	int devidePos = -1;
	bool ret = true;
	while (ret)
	{
		ret = findString(str, seperate, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		MEMCPY(curString.data(), curString.size(), str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 转换为整数放入列表
		if (curString[0] == '\0')
		{
			continue;
		}
		if (curCount >= bufferSize)
		{
			ERROR("int buffer size is too small, bufferSize:" + intToString(bufferSize));
			break;
		}
		buffer[curCount++] = stringToInt(curString.data());
	}
	return curCount;
}

void StringUtility::stringToUInts(const string& str, myVector<uint>& valueList, const char* seperate)
{
	VECTOR(string, strList);
	split(str.c_str(), seperate, strList);
	FOR_VECTOR_CONST(strList)
	{
		valueList.push_back((uint)stringToInt(strList[i]));
	}
	UN_VECTOR(string, strList);
}

uint StringUtility::stringToUInts(const char* str, uint* buffer, uint bufferSize, const char* seperate)
{
	uint curCount = 0;
	uint startPos = 0;
	uint keyLen = (uint)strlen(seperate);
	uint sourceLen = (uint)strlen(str);
	array<char, 16> curString{ 0 };
	int devidePos = -1;
	bool ret = true;
	while (ret)
	{
		ret = findString(str, seperate, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		MEMCPY(curString.data(), curString.size(), str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 转换为长整数放入列表
		if (curString[0] == '\0')
		{
			continue;
		}
		if (curCount >= bufferSize)
		{
			ERROR("uint buffer size is too small, bufferSize:" + intToString(bufferSize));
			break;
		}
		buffer[curCount++] = stringToInt(curString.data());
	}
	return curCount;
}

void StringUtility::stringToULLongs(const char* str, myVector<ullong>& valueList, const char* seperate)
{
	VECTOR(string, strList);
	split(str, seperate, strList);
	FOR_VECTOR_CONST(strList)
	{
		valueList.push_back(stringToULLong(strList[i]));
	}
	UN_VECTOR(string, strList);
}

uint StringUtility::stringToULLongs(const char* str, ullong* buffer, uint bufferSize, const char* seperate)
{
	uint curCount = 0;
	uint startPos = 0;
	uint keyLen = (uint)strlen(seperate);
	uint sourceLen = (uint)strlen(str);
	array<char, 32> curString{ 0 };
	int devidePos = -1;
	bool ret = true;
	while (ret)
	{
		ret = findString(str, seperate, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		MEMCPY(curString.data(), curString.size(), str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 转换为长整数放入列表
		if (curString[0] == '\0')
		{
			continue;
		}
		if (curCount >= bufferSize)
		{
			ERROR("ullong buffer size is too small, bufferSize:" + intToString(bufferSize));
			break;
		}
		buffer[curCount++] = stringToULLong(curString.data());
	}
	return curCount;
}

string StringUtility::zeroString(uint zeroCount)
{
	array<char, 16> charArray{ 0 };
	FOR_I(zeroCount)
	{
		charArray[i] = '0';
	}
	charArray[zeroCount] = '\0';
	return charArray.data();
}

void StringUtility::zeroString(char* charArray, uint size, uint zeroCount)
{
	if (size < zeroCount)
	{
		ERROR("buffer is too small");
		return;
	}
	FOR_I(zeroCount)
	{
		charArray[i] = '0';
	}
	charArray[zeroCount] = '\0';
}

string StringUtility::floatToStringExtra(float f, uint precision, bool removeTailZero)
{
	const static string zeroDot = "0.";
	string retString;
	if (!MathUtility::isFloatZero(f))
	{
		float powerValue = MathUtility::powerFloat(10.0f, precision);
		float totalValue = f * powerValue + MathUtility::sign(f) * 0.5f;
		if ((int)totalValue == 0)
		{
			if (precision > 0)
			{
				array<char, 16> temp{ 0 };
				zeroString(temp, precision);
				retString += zeroDot + temp.data();
			}
			else
			{
				retString = "0";
			}
		}
		else
		{
			INT_STR(temp, abs((int)totalValue));
			retString = temp;
			int dotPosition = (uint)strlen(retString.c_str()) - precision;
			if (dotPosition <= 0)
			{
				array<char, 16> tempZero{ 0 };
				zeroString(tempZero, -dotPosition);
				retString = zeroDot + tempZero.data() + retString;
			}
			else
			{
				retString.insert(dotPosition, 1, '.');
			}
			// 为负数时,确保负号始终在最前面
			if ((int)totalValue < 0)
			{
				retString = "-" + retString;
			}
		}
	}
	else
	{
		if (precision > 0)
		{
			array<char, 16> tempZero{ 0 };
			zeroString(tempZero, precision);
			retString = zeroDot + tempZero.data();
		}
		else
		{
			retString = "0";
		}
	}
	// 移除末尾无用的0
	if (removeTailZero && retString[retString.length() - 1] == '0')
	{
		size_t dotPos = retString.find_last_of('.');
		if (dotPos != NOT_FIND)
		{
			string floatPart = retString.substr(dotPos + 1, retString.length() - dotPos - 1);
			// 找到最后一个不是0的位置,然后将后面的所有0都去掉
			size_t notZeroPos = floatPart.find_last_not_of('0');
			// 如果小数部分全是0,则将小数点也一起去掉
			if (notZeroPos == NOT_FIND)
			{
				retString = retString.substr(0, dotPos);
			}
			// 去除小数部分末尾所有0
			else if (notZeroPos != floatPart.length() - 1)
			{
				retString = retString.substr(0, dotPos + 1) + floatPart.substr(0, notZeroPos + 1);
			}
		}
	}
	return retString;
}

string StringUtility::floatToString(float f)
{
	array<char, 16> temp{ 0 };
	floatToString(temp, f);
	return temp.data();
}

void StringUtility::floatToString(char* charArray, uint size, float f) 
{
	SPRINTF(charArray, size, "%f", f);
	// 先查找小数点和结束符的位置
	int dotPos = -1;
	uint strLen = 0;
	FOR_I(size)
	{
		if (charArray[i] == '.')
		{
			dotPos = i;
		}
		else if (charArray[i] == '\0')
		{
			strLen = i;
			break;
		}
	}
	if (dotPos >= 0)
	{
		// 从结束符往前查找
		FOR_I(strLen)
		{
			uint index = strLen - 1 - i;
			// 如果找到了小数点仍然没有找到一个不为'0'的字符,则从小数点部分截断字符串
			if (index == (uint)dotPos)
			{
				charArray[dotPos] = '\0';
				break;
			}
			// 找到小数点后的从后往前的第一个不为'0'的字符,从此处截断字符串
			if (charArray[index] != '0' && index + 1 < strLen)
			{
				charArray[index + 1] = '\0';
				break;
			}
		}
	}
}

string StringUtility::floatsToString(float* valueList, uint count, const char* seperate)
{
	// 根据列表长度选择适当的数组长度,每个float默认数字长度为16个字符
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	charArray[0] = 0;
	array<char, 16> temp{ 0 };
	FOR_I(count)
	{
		floatToString(temp, valueList[i]);
		if (i != count - 1)
		{
			STR_APPEND2_N(charArray, arrayLen, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(charArray, arrayLen, temp.data());
		}
	}
	string str(charArray);
	deleteCharArray(charArray);
	return str;
}

void StringUtility::floatsToString(char* buffer, uint bufferSize, float* valueList, uint count, const char* seperate)
{
	buffer[0] = '\0';
	array<char, 16> temp{ 0 };
	FOR_I(count)
	{
		floatToString(temp, valueList[i]);
		if (i != count - 1)
		{
			STR_APPEND2_N(buffer, bufferSize, temp.data(), seperate);
		}
		else
		{
			STR_APPEND1_N(buffer, bufferSize, temp.data());
		}
	}
}

void StringUtility::stringToFloats(const string& str, myVector<float>& valueList, const char* seperate)
{
	VECTOR(string, strList);
	split(str.c_str(), seperate, strList);
	uint count = strList.size();
	FOR_I(count)
	{
		if (strList[i].length() > 0)
		{
			valueList.push_back(stringToFloat(strList[i]));
		}
	}
	UN_VECTOR(string, strList);
}

uint StringUtility::stringToFloats(const char* str, float* buffer, uint bufferSize, const char* seperate)
{
	uint curCount = 0;
	uint startPos = 0;
	uint keyLen = (uint)strlen(seperate);
	uint sourceLen = (uint)strlen(str);
	array<char, 32> curString{ 0 };
	int devidePos = -1;
	bool ret = true;
	while (true)
	{
		ret = findString(str, seperate, &devidePos, startPos);
		// 无论是否查找到,都将前面一段字符串截取出来
		devidePos = ret ? devidePos : sourceLen;
		MEMCPY(curString.data(), curString.size(), str + startPos, devidePos - startPos);
		curString[devidePos - startPos] = '\0';
		startPos = devidePos + keyLen;
		// 转换为长整数放入列表
		if (curString[0] == '\0')
		{
			continue;
		}
		if (curCount >= bufferSize)
		{
			ERROR("float buffer size is too small, bufferSize:" + intToString(bufferSize));
			break;
		}
		buffer[curCount++] = stringToFloat(curString.data());
	}
	return curCount;
}


string StringUtility::bytesToString(const char* buffer, uint length)
{
	int size = MathUtility::getGreaterPower2(length + 1);
	char* tempBuffer = newCharArray(size);
	tempBuffer[length] = '\0';
	MEMCPY(tempBuffer, size, buffer, length);
	string str(tempBuffer);
	deleteCharArray(tempBuffer);
	return str;
}

bool StringUtility::endWith(const char* oriString, const char* pattern, bool sensitive)
{
	uint originLength = (uint)strlen(oriString);
	uint patternLength = (uint)strlen(pattern);
	if (originLength < patternLength)
	{
		return false;
	}
	if (sensitive)
	{
		FOR_I(patternLength)
		{
			if (oriString[i + originLength - patternLength] != pattern[i])
			{
				return false;
			}
		}
	}
	else
	{
		FOR_I(patternLength)
		{
			if (toLower(oriString[i + originLength - patternLength]) != toLower(pattern[i]))
			{
				return false;
			}
		}
	}
	return true;
}

bool StringUtility::startWith(const char* oriString, const char* pattern, bool sensitive)
{
	uint originLength = (uint)strlen(oriString);
	uint patternLength = (uint)strlen(pattern);
	if (originLength < patternLength)
	{
		return false;
	}
	if (sensitive)
	{
		FOR_I(patternLength)
		{
			if (oriString[i] != pattern[i])
			{
				return false;
			}
		}
	}
	else
	{
		FOR_I(patternLength)
		{
			if (toLower(oriString[i]) != toLower(pattern[i]))
			{
				return false;
			}
		}
	}
	return true;
}

string StringUtility::removePreNumber(const string& str)
{
	uint length = (uint)str.length();
	FOR_I(length)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return str.substr(i);
		}
	}
	return str;
}

string StringUtility::stringsToString(string* strList, uint stringCount, const char* seperate)
{
	string str;
	FOR_I(stringCount)
	{
		str += strList[i];
		if (i != stringCount - 1)
		{
			str += seperate;
		}
	}
	return str;
}

void StringUtility::stringsToString(char* buffer, uint bufferSize, const char** strList, uint stringCount, const char* seperate)
{
	buffer[0] = '\0';
	FOR_I(stringCount)
	{
		strcat_s(buffer, bufferSize, strList[i]);
		if (i != stringCount - 1)
		{
			strcat_s(buffer, bufferSize, seperate);
		}
	}
}

#if RUN_PLATFORM == PLATFORM_WINDOWS
wstring StringUtility::ANSIToUnicode(const char* str)
{
	if (str == NULL || str[0] == 0)
	{
		return L"";
	}
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	int allocSize = MathUtility::getGreaterPower2(unicodeLen + 1);
	wchar_t* pUnicode = new wchar_t[allocSize];
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)pUnicode, unicodeLen);
	wstring ret(pUnicode);
	delete[] pUnicode;
	return ret;
}

void StringUtility::ANSIToUnicode(const char* str, wchar_t* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		output[0] = L'\0';
		return;
	}
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	if (unicodeLen >= (int)maxLength)
	{
		ERROR("buffer is too small");
		output[0] = L'\0';
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)output, unicodeLen);
}

string StringUtility::UnicodeToANSI(const wchar_t* str)
{
	if (str == NULL || str[0] == 0)
	{
		return "";
	}
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	int allocSize = MathUtility::getGreaterPower2(iTextLen + 1);
	char* pElementText = newCharArray(allocSize);
	WideCharToMultiByte(CP_ACP, 0, str, -1, pElementText, iTextLen, NULL, NULL);
	string strText(pElementText);
	deleteCharArray(pElementText);
	return strText;
}

void StringUtility::UnicodeToANSI(const wchar_t* str, char* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		output[0] = '\0';
		return;
	}
	int iTextLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	if (iTextLen >= (int)maxLength)
	{
		ERROR("buffer is too small");
		output[0] = '\0';
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, str, -1, output, iTextLen, NULL, NULL);
}

string StringUtility::UnicodeToUTF8(const wchar_t* str)
{
	if (str == NULL || str[0] == 0)
	{
		return "";
	}
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	int allocSize = MathUtility::getGreaterPower2(iTextLen + 1);
	char* pElementText = newCharArray(allocSize);
	WideCharToMultiByte(CP_UTF8, 0, str, -1, pElementText, iTextLen, NULL, NULL);
	string strText(pElementText);
	deleteCharArray(pElementText);
	return strText;
}

void StringUtility::UnicodeToUTF8(const wchar_t* str, char* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		output[0] = '\0';
		return;
	}
	// wide char to multi char
	int iTextLen = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
	if (iTextLen >= (int)maxLength)
	{
		ERROR("buffer is too small");
		output[0] = '\0';
		return;
	}
	WideCharToMultiByte(CP_UTF8, 0, str, -1, output, iTextLen, NULL, NULL);
}

wstring StringUtility::UTF8ToUnicode(const char* str)
{
	if (str == NULL || str[0] == 0)
	{
		return L"";
	}
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	int allocSize = MathUtility::getGreaterPower2(unicodeLen + 1);
	wchar_t* pUnicode = new wchar_t[allocSize];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)pUnicode, unicodeLen);
	wstring rt(pUnicode);
	delete[] pUnicode;
	return rt;
}

void StringUtility::UTF8ToUnicode(const char* str, wchar_t* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		output[0] = L'\0';
		return;
	}
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	if (unicodeLen >= (int)maxLength)
	{
		ERROR("buffer is too small");
		output[0] = L'\0';
		return;
	}
	MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)output, unicodeLen);
}

#elif RUN_PLATFORM == PLATFORM_LINUX
wstring StringUtility::ANSIToUnicode(const char* str)
{
	if (str == NULL || str[0] == 0)
	{
		return L"";
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_GBK);
	}
	catch (exception)
	{
		ERROR("当前系统不支持GBK编码");
		return L"";
	}
	int dSize = mbstowcs(NULL, str, 0) + 1;
	int allocSize = MathUtility::getGreaterPower2(dSize);
	wchar_t* dBuf = ArrayPool::newArray<wchar_t>(allocSize);
	mbstowcs(dBuf, str, dSize);
	setlocale(LC_ALL, oldname);
	wstring strText(dBuf);
	ArrayPool::deleteArray(dBuf);
	return strText;
}

void StringUtility::ANSIToUnicode(const char* str, wchar_t* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		output[0] = L'\0';
		return;
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_GBK);
	}
	catch (exception)
	{
		ERROR("当前系统不支持GBK编码");
		output[0] = L'\0';
		return;
	}
	int dSize = mbstowcs(NULL, str, 0) + 1;
	if (dSize >= maxLength)
	{
		ERROR("buffer is too small");
		output[0] = L'\0';
		return;
	}
	mbstowcs(output, str, dSize);
	setlocale(LC_ALL, oldname);
}

string StringUtility::UnicodeToANSI(const wchar_t* str)
{
	if (str == NULL || str[0] == 0)
	{
		return EMPTY;
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_GBK);
	}
	catch (exception)
	{
		ERROR("当前系统不支持GBK编码");
		return EMPTY;
	}
	int dSize = wcstombs(NULL, str, 0) + 1;
	int allocSize = MathUtility::getGreaterPower2(dSize);
	char* dBuf = newCharArray(allocSize);
	wcstombs(dBuf, str, dSize);
	setlocale(LC_ALL, oldname);
	string strText(dBuf);
	deleteCharArray(dBuf);
	return strText;
}

void StringUtility::UnicodeToANSI(const wchar_t* str, char* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		output[0] = '\0';
		return;
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_GBK);
	}
	catch (exception)
	{
		ERROR("当前系统不支持GBK编码");
		output[0] = '\0';
		return;
	}
	int dSize = wcstombs(NULL, str, 0) + 1;
	if (dSize >= maxLength)
	{
		ERROR("buffer is too small");
		output[0] = '\0';
		return;
	}
	wcstombs(output, str, dSize);
	setlocale(LC_ALL, oldname);
}

string StringUtility::UnicodeToUTF8(const wchar_t* str)
{
	if (str == NULL || str[0] == 0)
	{
		return EMPTY;
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);
	}
	catch (exception)
	{
		ERROR("当前系统不支持UTF8编码");
		return EMPTY;
	}
	int dSize = wcstombs(NULL, str, 0) + 1;
	int allocSize = MathUtility::getGreaterPower2(dSize);
	char* dBuf = newCharArray(allocSize);
	wcstombs(dBuf, str, dSize);
	setlocale(LC_ALL, oldname);
	string strText(dBuf);
	deleteCharArray(dBuf);
	return strText;
}

void StringUtility::UnicodeToUTF8(const wchar_t* str, char* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		output[0] = '\0';
		return;
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);
	}
	catch (exception)
	{
		ERROR("当前系统不支持UTF8编码");
		output[0] = '\0';
		return;
	}
	int dSize = wcstombs(NULL, str, 0) + 1;
	if (dSize >= maxLength)
	{
		ERROR("buffer is too small");
		output[0] = '\0';
		return;
	}
	wcstombs(output, str, dSize);
	setlocale(LC_ALL, oldname);
}

wstring StringUtility::UTF8ToUnicode(const char* str)
{
	if (str == NULL || str[0] == 0)
	{
		return L"";
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);
	}
	catch (exception)
	{
		ERROR("当前系统不支持UTF8编码");
		return L"";
	}
	int dSize = mbstowcs(NULL, str, 0) + 1;
	int allocSize = MathUtility::getGreaterPower2(dSize);
	wchar_t* dBuf = ArrayPool::newArray<wchar_t>(allocSize);
	mbstowcs(dBuf, str, dSize);
	setlocale(LC_ALL, oldname);
	wstring strText(dBuf);
	ArrayPool::deleteArray(dBuf);
	return strText;
}

void StringUtility::UTF8ToUnicode(const char* str, wchar_t* output, uint maxLength)
{
	if (str == NULL || str[0] == 0)
	{
		maxLength = L'\0';
		return;
	}
	char* oldname = setlocale(LC_ALL, NULL);
	try
	{
		setlocale(LC_ALL, LC_NAME_zh_CN_UTF8);
	}
	catch (exception)
	{
		ERROR("当前系统不支持UTF8编码");
		maxLength = L'\0';
		return;
	}
	int dSize = mbstowcs(NULL, str, 0) + 1;
	if (dSize >= maxLength)
	{
		ERROR("buffer is too small");
		output[0] = L'\0';
		return;
	}
	mbstowcs(output, str, dSize);
	setlocale(LC_ALL, oldname);
}
#endif

void StringUtility::ANSIToUTF8(const char* str, string utf8, bool addBOM)
{
	uint length = (uint)strlen(str);
	uint unicodeLength = MathUtility::getGreaterPower2((length + 1) << 1);
	wchar_t* unicodeStr = new wchar_t[unicodeLength];
	ANSIToUnicode(str, unicodeStr, unicodeLength);
	utf8 = UnicodeToUTF8(unicodeStr);
	if (addBOM)
	{
		utf8 = BOM + utf8;
	}
	delete[] unicodeStr;
}

void StringUtility::ANSIToUTF8(const char* str, char* output, uint maxLength, bool addBOM)
{
	uint unicodeLength = MathUtility::getGreaterPower2((maxLength + 1) << 1);
	wchar_t* unicodeStr = new wchar_t[unicodeLength];
	ANSIToUnicode(str, unicodeStr, unicodeLength);
	if (addBOM)
	{
		MEMCPY(output, maxLength, BOM, 3);
		UnicodeToUTF8(unicodeStr, output + 3, maxLength - 3);
	}
	else
	{
		UnicodeToUTF8(unicodeStr, output, maxLength);
	}
	delete[] unicodeStr;
}

void StringUtility::UTF8ToANSI(const char* str, string& ansi, bool eraseBOM)
{
	uint length = (uint)strlen(str);
	uint unicodeLength = MathUtility::getGreaterPower2((length + 1) << 1);
	wchar_t* unicodeStr = new wchar_t[unicodeLength];
	if (eraseBOM)
	{
		string newStr = str;
		removeBOM(newStr);
		UTF8ToUnicode(newStr.c_str(), unicodeStr, unicodeLength);
	}
	else
	{
		UTF8ToUnicode(str, unicodeStr, unicodeLength);
	}
	ansi = UnicodeToANSI(unicodeStr);
	delete[] unicodeStr;
}

void StringUtility::UTF8ToANSI(const char* str, char* output, uint maxLength, bool eraseBOM)
{
	uint unicodeLength = MathUtility::getGreaterPower2((maxLength + 1) << 1);
	wchar_t* unicodeStr = new wchar_t[unicodeLength];
	if (eraseBOM)
	{
		const char* newInput = str;
		uint length = (uint)strlen(str);
		if (length >= 3 && str[0] == BOM[0] && str[1] == BOM[1] && str[2] == BOM[2])
		{
			newInput += 3;
		}
		UTF8ToUnicode(newInput, unicodeStr, unicodeLength);
	}
	else
	{
		UTF8ToUnicode(str, unicodeStr, unicodeLength);
	}
	UnicodeToANSI(unicodeStr, output, maxLength);
	delete[] unicodeStr;
}

void StringUtility::removeBOM(string& str)
{
	if (str.length() >= 3 && str[0] == BOM[0] && str[1] == BOM[1] && str[2] == BOM[2])
	{
		str.erase(0, 3);
	}
}

void StringUtility::removeBOM(char* str, uint length)
{
	if (length == 0)
	{
		length = (uint)strlen(str);
	}
	if (length >= 3 && str[0] == BOM[0] && str[1] == BOM[1] && str[2] == BOM[2])
	{
		memmove(str, str + 3, length - 3 + 1);
	}
}

void StringUtility::jsonStartArray(string& str, uint preTableCount, bool returnLine)
{
	FOR_I(preTableCount)
	{
		str += "\t";
	}
	str += "[";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonEndArray(string& str, uint preTableCount, bool returnLine)
{
	removeLastComma(str);
	FOR_I(preTableCount)
	{
		str += "\t";
	}
	str += "],";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonStartStruct(string& str, uint preTableCount, bool returnLine)
{
	FOR_I(preTableCount)
	{
		str += "\t";
	}
	str += "{";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonEndStruct(string& str, uint preTableCount, bool returnLine)
{
	removeLastComma(str);
	FOR_I(preTableCount)
	{
		str += "\t";
	}
	str += "},";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonAddPair(string& str, const string& name, const string& value, uint preTableCount, bool returnLine)
{
	FOR_I(preTableCount)
	{
		str += "\t";
	}
	str += "\"" + name + "\": \"" + value + "\",";
	if (returnLine)
	{
		str += "\r\n";
	}
}

void StringUtility::jsonAddObject(string& str, const string& name, const string& value, uint preTableCount, bool returnLine)
{
	FOR_I(preTableCount)
	{
		str += "\t";
	}
	str += "\"" + name + "\": " + value + ",";
	if (returnLine)
	{
		str += "\r\n";
	}
}

string StringUtility::toLower(const string& str)
{
	string ret = str;
	uint size = (uint)ret.length();
	FOR_I(size)
	{
		ret[i] = toLower(ret[i]);
	}
	return ret;
}

string StringUtility::toUpper(const string& str)
{
	string ret = str;
	uint size = (uint)ret.length();
	FOR_I(size)
	{
		ret[i] = toUpper(ret[i]);
	}
	return ret;
}

void StringUtility::rightToLeft(string& str)
{
	uint pathLength = (uint)str.length();
	FOR_I(pathLength)
	{
		if (str[i] == '\\')
		{
			str[i] = '/';
		}
	}
}
void StringUtility::leftToRight(string& str)
{
	uint pathLength = (uint)str.length();
	FOR_I(pathLength)
	{
		if (str[i] == '/')
		{
			str[i] = '\\';
		}
	}
}
bool StringUtility::findStringLower(const string& res, const string& sub, int* pos, uint startIndex, bool direction)
{
	// 全转换为小写
	return findString(toLower(res), toLower(sub), pos, startIndex, direction);
}

bool StringUtility::findString(const string& res, const string& sub, int* pos, uint startIndex, bool direction)
{
	return findString(res.c_str(), sub.c_str(), pos, startIndex, direction);
}

bool StringUtility::findString(const char* res, const char* sub, int* pos, uint startIndex, bool direction)
{
	int posFind = -1;
	uint subLen = (uint)strlen(sub);
	int searchLength = (uint)strlen(res) - subLen + 1;
	if (searchLength <= 0)
	{
		return false;
	}
	if (direction)
	{
		for (int i = startIndex; i < searchLength; ++i)
		{
			uint j = 0;
			for (j = 0; j < subLen; ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}
			if (j == subLen)
			{
				posFind = i;
				break;
			}
		}
	}
	else
	{
		for (uint i = searchLength - 1; i >= startIndex; --i)
		{
			uint j = 0;
			for (j = 0; j < subLen; ++j)
			{
				if (res[i + j] != sub[j])
				{
					break;
				}
			}
			if (j == subLen)
			{
				posFind = i;
				break;
			}
		}
	}
	if (pos != NULL)
	{
		*pos = posFind;
	}
	return posFind != -1;
}

int StringUtility::findStringPos(const char* res, const char* dst, uint startIndex, bool direction)
{
	int pos = -1;
	findString(res, dst, &pos, startIndex, direction);
	return pos;
}

int StringUtility::findStringPos(const string& res, const string& dst, uint startIndex, bool direction)
{
	int pos = -1;
	findString(res, dst, &pos, startIndex, direction);
	return pos;
}

bool StringUtility::checkString(const string& str, const string& valid)
{
	uint oldStrLen = (uint)str.length();
	FOR_I(oldStrLen)
	{
		if (!valid.find_first_of(str[i]))
		{
			return false;
		}
	}
	return true;
}

bool StringUtility::checkFloatString(const string& str, const string& valid)
{
	return checkIntString(str, "." + valid);
}

bool StringUtility::checkIntString(const string& str, const string& valid)
{
	return checkString(str, "0123456789" + valid);
}

string StringUtility::charToHexString(byte value, bool upper)
{
	char byteHex[3]{ 0 };
	const char* charPool = upper ? "ABCDEF" : "abcdef";
	byte highBit = value >> 4;
	// 高字节的十六进制
	byteHex[0] = (highBit < (byte)10) ? ('0' + highBit) : charPool[highBit - 10];
	// 低字节的十六进制
	byte lowBit = value & 0x0F;
	byteHex[1] = (lowBit < (byte)10) ? ('0' + lowBit) : charPool[lowBit - 10];
	return byteHex;
}

string StringUtility::charArrayToHexString(byte* data, uint dataCount, bool addSpace, bool upper)
{
	uint oneLength = addSpace ? 3 : 2;
	uint showCount = MathUtility::getGreaterPower2(dataCount * oneLength + 1);
	char* byteData = newCharArray(showCount);
	FOR_J(dataCount)
	{
		string byteStr = charToHexString(data[j]);
		byteData[j * oneLength + 0] = byteStr[0];
		byteData[j * oneLength + 1] = byteStr[1];
		if (oneLength >= 3)
		{
			byteData[j * oneLength + 2] = ' ';
		}
	}
	byteData[dataCount * oneLength] = '\0';
	string str(byteData);
	deleteCharArray(byteData);
	return str;
}

uint StringUtility::getCharCount(const string& str, char key)
{
	uint count = 0;
	uint length = (uint)str.length();
	FOR_I(length)
	{
		if (str[i] == key)
		{
			++count;
		}
	}
	return count;
}

uint StringUtility::getCharCount(const char* str, char key)
{
	uint count = 0;
	int index = 0;
	while (true)
	{
		if (str[index] == '\0')
		{
			break;
		}
		if (str[index] == key)
		{
			++count;
		}
		++index;
	}
	return count;
}

void StringUtility::appendValueString(char* queryStr, uint size, const char* str, bool toUTF8, bool addComma)
{
	const char* end = addComma ? "\"," : "\"";
	if (toUTF8)
	{
		char* utf8String = newCharArray(size);
		ANSIToUTF8(str, utf8String, size, false);
		STR_APPEND3_N(queryStr, size, "\"", utf8String, end);
		deleteCharArray(utf8String);
	}
	else
	{
		STR_APPEND3_N(queryStr, size, "\"", str, end);
	}
}

void StringUtility::appendValueInt(char* queryStr, uint size, int value, bool addComma)
{
	INT_STR(temp, value);
	STR_APPEND2_N(queryStr, size, temp, addComma ? "," : NULL);
}

void StringUtility::appendValueFloat(char* queryStr, uint size, float value, bool addComma)
{
	FLOAT_STR(temp, value);
	STR_APPEND2_N(queryStr, size, temp, addComma ? "," : NULL);
}

void StringUtility::appendValueULLong(char* queryStr, uint size, ullong value, bool addComma)
{
	ULLONG_STR(temp, value);
	STR_APPEND2_N(queryStr, size, temp, addComma ? "," : NULL);
}

void StringUtility::appendValueBytes(char* queryStr, uint size, byte* ushortArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	bytesToString(charArray, arrayLen, ushortArray, count);
	appendValueString(queryStr, size, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendValueUShorts(char* queryStr, uint size, ushort* ushortArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	ushortsToString(charArray, arrayLen, ushortArray, count);
	appendValueString(queryStr, size, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendValueInts(char* queryStr, uint size, int* intArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	intsToString(charArray, arrayLen, intArray, count);
	appendValueString(queryStr, size, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendValueUInts(char* queryStr, uint size, uint* intArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	uintsToString(charArray, arrayLen, intArray, count);
	appendValueString(queryStr, size, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendValueFloats(char* queryStr, uint size, float* floatArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	floatsToString(charArray, arrayLen, floatArray, count);
	appendValueString(queryStr, size, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendValueULLongs(char* queryStr, uint size, ullong* longArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	ullongsToString(charArray, arrayLen, longArray, count);
	appendValueString(queryStr, size, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendConditionString(char* condition, uint size, const char* col, const char* str, bool toUTF8, const char* relationalOperator, const char* logicalOperator)
{
	if (toUTF8)
	{
		char* utf8String = newCharArray(size);
		ANSIToUTF8(str, utf8String, size, false);
		STR_APPEND6_N(condition, size, col, relationalOperator, "\"", utf8String, "\"", logicalOperator);
		deleteCharArray(utf8String);
	}
	else
	{
		STR_APPEND6_N(condition, size, col, relationalOperator, "\"", str, "\"", logicalOperator);
	}
}

void StringUtility::appendConditionInt(char* condition, uint size, const char* col, int value, const char* relationalOperator, const char* logicalOperator)
{
	INT_STR(temp, value);
	STR_APPEND4_N(condition, size, col, relationalOperator, temp, logicalOperator);
}

void StringUtility::appendConditionFloat(char* condition, uint size, const char* col, float value, const char* relationalOperator, const char* logicalOperator)
{
	FLOAT_STR(temp, value);
	STR_APPEND4_N(condition, size, col, relationalOperator, temp, logicalOperator);
}

void StringUtility::appendConditionULLong(char* condition, uint size, const char* col, ullong value, const char* relationalOperator, const char* logicalOperator)
{
	ULLONG_STR(temp, value);
	STR_APPEND4_N(condition, size, col, relationalOperator, temp, logicalOperator);
}

void StringUtility::appendUpdateString(char* updateInfo, uint size, const char* col, const char* str, bool toUTF8, bool addComma)
{
	const char* end = addComma ? "\"," : "\"";
	if (toUTF8)
	{
		char* utf8String = newCharArray(size);
		ANSIToUTF8(str, utf8String, size, false);
		STR_APPEND4_N(updateInfo, size, col, " = \"", utf8String, end);
		deleteCharArray(utf8String);
	}
	else
	{
		STR_APPEND4_N(updateInfo, size, col, " = \"", str, end);
	}
}

void StringUtility::appendUpdateInt(char* updateInfo, uint size, const char* col, int value, bool addComma)
{
	INT_STR(temp, value);
	const char* end = addComma ? "," : NULL;
	STR_APPEND4_N(updateInfo, size, col, " = ", temp, end);
}

void StringUtility::appendUpdateFloat(char* updateInfo, uint size, const char* col, float value, bool addComma)
{
	FLOAT_STR(temp, value);
	const char* end = addComma ? "," : NULL;
	STR_APPEND4_N(updateInfo, size, col, " = ", temp, end);
}

void StringUtility::appendUpdateULLong(char* updateInfo, uint size, const char* col, ullong value, bool addComma)
{
	ULLONG_STR(temp, value);
	const char* end = addComma ? "," : NULL;
	STR_APPEND4_N(updateInfo, size, col, " = ", temp, end);
}

void StringUtility::appendUpdateBytes(char* updateInfo, uint size, const char* col, byte* ushortArray, uint count, bool addComma)
{
	int arrayLen = 4 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	bytesToString(charArray, arrayLen, ushortArray, count);
	appendUpdateString(updateInfo, size, col, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendUpdateUShorts(char* updateInfo, uint size, const char* col, ushort* ushortArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	ushortsToString(charArray, arrayLen, ushortArray, count);
	appendUpdateString(updateInfo, size, col, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendUpdateInts(char* updateInfo, uint size, const char* col, int* intArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	intsToString(charArray, arrayLen, intArray, count);
	appendUpdateString(updateInfo, size, col, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendUpdateUInts(char* updateInfo, uint size, const char* col, uint* intArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	uintsToString(charArray, arrayLen, intArray, count);
	appendUpdateString(updateInfo, size, col, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendUpdateFloats(char* updateInfo, uint size, const char* col, float* floatArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	floatsToString(charArray, arrayLen, floatArray, count);
	appendUpdateString(updateInfo, size, col, charArray, false, addComma);
	deleteCharArray(charArray);
}

void StringUtility::appendUpdateULLongs(char* updateInfo, uint size, const char* col, ullong* longArray, uint count, bool addComma)
{
	int arrayLen = 16 * MathUtility::getGreaterPower2(count);
	char* charArray = newCharArray(arrayLen);
	ullongsToString(charArray, arrayLen, longArray, count);
	appendUpdateString(updateInfo, size, col, charArray, false, addComma);
	deleteCharArray(charArray);
}

uint StringUtility::greaterPower2(uint value)
{
	return MathUtility::getGreaterPower2(value);
}

char* StringUtility::newCharArray(uint length)
{
	return new char[length];
}

void StringUtility::deleteCharArray(char* charArray)
{
	delete[] charArray;
}