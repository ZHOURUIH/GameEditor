#ifndef _COMMAND_RECEIVER_H_
#define _COMMAND_RECEIVER_H_

#include "Utility.h"

class CommandReceiver : public Utility
{
public:
	~CommandReceiver();
	virtual void setName(const string& name) 
	{
		mName = name;
		mUTF8Name = name; 
	}
	const string& getName() const { return mName; }
	const string& getUTF8Name() const { return mUTF8Name; }
protected:
	string mUTF8Name;		// UTF8编码的名字,因为windows和linux下不一样,为了windows调试方便,
	string mName;			// GB2312编码的名字
};

#endif