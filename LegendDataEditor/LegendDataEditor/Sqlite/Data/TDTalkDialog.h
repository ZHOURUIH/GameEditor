#ifndef _TD_TALK_DIALOG_H_
#define _TD_TALK_DIALOG_H_

#include "SQLiteData.h"

class TDTalkDialog : public SQLiteData
{
public:
	static const char* ID;
	static const char* Description;
	static const char* NPC;
	static const char* NamePosition;
	static const char* Speaker;
	static const char* Content;
public:
	int mID;
	string mDescription;
	ushort mNPC;
	byte mNamePosition;
	string mSpeaker;
	string mContent;
public:
	TDTalkDialog()
	{
		registeParam(mID, ID);
		registeParam(mDescription, Description);
		registeParam(mNPC, NPC);
		registeParam(mNamePosition, NamePosition);
		registeParam(mSpeaker, Speaker);
		registeParam(mContent, Content);
	}
};

#endif