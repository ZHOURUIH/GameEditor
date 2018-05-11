#ifndef _TX_COMMAND_RECEIVER_H_
#define _TX_COMMAND_RECEIVER_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txCommand;
class txCommandReceiver : public txEngineBase
{
public:
	txCommandReceiver(const std::string& name);
	virtual ~txCommandReceiver();
	virtual void receiveCommand(txCommand* cmd);
	const std::string& getName() { return mName; }
	// 谨慎使用设置名字
	void setName(const std::string& name) { mName = name; }
protected:
	std::string mName;
};

#endif