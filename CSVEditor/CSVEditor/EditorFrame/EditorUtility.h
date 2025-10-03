#pragma once

#include "EditorHeader.h"

namespace EditorUtility
{
	// 解析一个csv文件的内容
	void parseCSV(const string& fullContent, Vector<Vector<string>>& result);
	// 给csv文件添加一个字符串数据,addCommaOrReturn为true则在最后添加逗号,否则添加换行
	void appendString(string& file, const string& value, bool addCommaOrReturn);
	string getOwnerString(OWNER owner);
	OWNER getOwner(string owner);
};

using namespace EditorUtility;