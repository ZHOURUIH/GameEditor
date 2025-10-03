#pragma once

#include "FrameHeader.h"
#include "EditorEnum.h"

class ColumnData
{
public:
	string mName;
	string mType;
	OWNER mOwner = OWNER::NONE;
	string mComment;
	string mLinkTable;
};