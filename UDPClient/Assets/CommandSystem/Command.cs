using System;
using System.Collections.Generic;

// 命令基类
public class Command
{
	protected static int seed;
	protected CommandReceiver mReceiver;			// 命令接受者
	protected float mDelayTime;						// 命令当前延迟时间
	protected int mCmdID;							// 命令ID,每一个命令对象拥有一个唯一ID
	protected bool mDelayCommand;					// 是否是延迟执行的命令
	public Command()
	{
		mCmdID = ++seed;
	}
	// 命令执行
	public virtual void execute() { }
	public bool isDelayCommand() { return mDelayCommand; }
	public CommandReceiver getReceiver() { return mReceiver; }
	public float getDelayTime() { return mDelayTime; }
	public int getID() { return mCmdID; }
	public void setDelayCommand(bool delay) { mDelayCommand = delay; }
	public void setReceiver(CommandReceiver Reciver) { mReceiver = Reciver; }
	public void setDelayTime(float time) { mDelayTime = time; }
}