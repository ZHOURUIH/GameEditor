#ifndef _TX_COMMAND_RECEIVER_H_
#define _TX_COMMAND_RECEIVER_H_

#include "EngineDefine.h"

class txCommand;
class txCommandReceiver
{
public:
	txCommandReceiver(std::string name);
	virtual ~txCommandReceiver();
	virtual void receiveCommand(txCommand* cmd);
	std::string& getName() { return mName; }
	// 谨慎使用设置名字
	void setName(std::string name) { mName = name; }
protected:
	std::string mName;
};

#endif