#pragma once

#include "wxHeader.h"
#include "EditorEnum.h"
#include "FrameHeader.h"

namespace EditorUtility
{
	// ����һ��csv�ļ�������
	void parseCSV(const string& fullContent, Vector<Vector<string>>& result);
	// ��csv�ļ����һ���ַ�������,addCommaOrReturnΪtrue���������Ӷ���,������ӻ���
	void appendString(string& file, const string& value, bool addCommaOrReturn);
	string getOwnerString(OWNER owner);
	OWNER getOwner(const string& owner);
};

using namespace EditorUtility;