#ifndef _TX_COMMAND_H_
#define _TX_COMMAND_H_

#include <string>

#include "txMemeryTrace.h"
#include "CommonDefine.h"

class txCommandReceiver;
class txCommand
{
public:
	txCommand(const char* file, int line, bool showInfo = true);
	virtual ~txCommand(){}

	void setReceiver(txCommandReceiver* Reciver){ mReceiver = Reciver; }
	void setEndCommandCallback(CommandCallback cmdCallback, void* userdata)
	{
		mEndCallback = cmdCallback;
		mEndUserData = userdata;
	}
	void setStartCommandCallback(CommandCallback cmdCallback, void* userdata)
	{
		mStartCallback = cmdCallback;
		mStartUserData = userdata;
	}

	void runEndCallBack()
	{
		if (mEndCallback != NULL)
		{
			mEndCallback(mEndUserData, this);
		}
	}

	void runStartCallBack()
	{
		if (mStartCallback != NULL)
		{
			mStartCallback(mStartUserData, this);
		}
	}

	// 命令执行
	virtual void execute() = 0;
	// 调试信息，由CommandSystem调用
	virtual std::string showDebugInfo() = 0;
	const char*& getFile() { return mFile; }
	int& getLine() { return mLine; }
	bool& getShowDebugInfo() { return mShowDebugInfo; }
	bool& isDelayCommand() { return mDelayCommand; }
	// 创建延时执行的命令,会由命令系统执行完后自动销毁
	template<typename T>
	static T* createDelayCommand(const char* file, int line, bool showInfo = true)
	{
		T* cmd = TRACE_NEW(T, cmd, file, line, showInfo);
		cmd->mDelayCommand = true;
		return cmd;
	}

protected:
	txCommandReceiver* mReceiver;				// 命令接受者
	const char* mFile;							// 命令发出时所在的文件名
	int mLine;									// 命令发出时所在的行号
	bool mShowDebugInfo;						// 是否显示调试信息
	CommandCallback mEndCallback;				// 命令执行完毕时的回调函数
	CommandCallback mStartCallback;				// 命令开始执行时的回调函数
	void* mEndUserData;
	void* mStartUserData;
	bool mDelayCommand;							// 是否是延迟执行的命令
};

#endif