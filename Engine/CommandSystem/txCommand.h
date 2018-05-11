#ifndef _TX_COMMAND_H_
#define _TX_COMMAND_H_

#include "Utility.h"
#include "txEngineBase.h"

class txCommandReceiver;
class txCommand : public txEngineBase
{
public:
	txCommand(){}
	virtual ~txCommand(){}
	virtual void init()
	{
		mReceiver = NULL;
		mShowDebugInfo = true;
		mDelayCommand = false;
		mValid = false;
		mAssignID = -1;
		mResult = NULL;
		mExecuteState = ES_NOT_EXECUTE;
		mFile = EMPTY_STRING;
		mLine = 0;
		mEndCallback.clear();
		mStartCallback.clear();
		reset();
	}
	virtual void reset() = 0;
	// 命令执行
	virtual void execute() = 0;
	// 调试信息，由CommandSystem调用
	virtual std::string showDebugInfo() = 0;
	const bool& getShowDebugInfo()						{ return mShowDebugInfo; }
	const bool& isDelayCommand()						{ return mDelayCommand; }
	txCommandReceiver* getReceiver()					{ return mReceiver; }
	const bool& isValid()								{ return mValid; }
	const std::string& getType()						{ return mType; }
	const EXECUTE_STATE& getExecuteState()				{ return mExecuteState; }
	const int& getAssignID()							{ return mAssignID; }
	const int& getCmdID()								{ return mCmdID; }
	const std::string& getFile()						{ return mFile; }
	const int& getLine()								{ return mLine; }
	void setShowDebugInfo(const bool& show)				{ mShowDebugInfo = show; }
	void setDelayCommand(const bool& delay)				{ mDelayCommand = delay; }
	void setReceiver(txCommandReceiver* Reciver)		{ mReceiver = Reciver; }
	void setValid(const bool& valid)					{ mValid = valid; }
	void setType(const std::string& type)				{ mType = type; }
	void setExecuteState(const EXECUTE_STATE& state)	{ mExecuteState = state; }
	void setAssignID(const int& id)						{ mAssignID = id; }
	void setID(const int& id)							{ mCmdID = id; }
	void setFile(const std::string& file)				{ mFile = file; }
	void setLine(const int& line)						{ mLine = line; }
	void setResult(bool* result)						{ mResult = result; }
	void addEndCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback != NULL)
		{
			mEndCallback.push_back(std::make_pair(cmdCallback, userData));
		}
	}
	void addStartCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback != NULL)
		{
			mStartCallback.push_back(std::make_pair(cmdCallback, userData));
		}
	}
	void runEndCallBack();
	void runStartCallBack();
protected:
	txCommandReceiver* mReceiver;	// 命令接受者
	bool mShowDebugInfo;			// 是否显示调试信息
	bool mDelayCommand;				// 是否是延迟执行的命令
	bool mValid;					// 是否为有效命令
	EXECUTE_STATE mExecuteState;	// 命令执行状态
	std::string mType;				// 命令类型
	std::string mFile;				// 命令发出时所在的文件名
	int mLine;						// 命令发出时所在的行号
	int	mCmdID;						// 命令唯一ID,在命令被创建时设置
	int mAssignID;					// 被分配为延迟命令时的唯一ID,每次分配都会设置一个新的唯一执行ID
	bool* mResult;					// 命令的执行结果,只用于部分需要知道执行结果的命令使用
	txVector<std::pair<CommandCallback, void*>> mEndCallback;		// 命令执行完毕时的回调函数
	txVector<std::pair<CommandCallback, void*>> mStartCallback;	// 命令开始执行时的回调函数
};

#endif