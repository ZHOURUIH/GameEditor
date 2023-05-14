#ifndef _CODE_EVENT_H_
#define _CODE_EVENT_H_

#include "CodeUtility.h"

class CodeEvent : public CodeUtility
{
public:
	static void generate();
protected:
	static void generateStringDefineEvent(const myVector<pair<string, string>>& eventList, const string& stringDefineFile);
	static void generateEventType(const myVector<pair<string, string>>& eventList, const string& headerPath);
	static void generateEventFile(const string& eventName, const string& eventPath);
protected:
};

#endif