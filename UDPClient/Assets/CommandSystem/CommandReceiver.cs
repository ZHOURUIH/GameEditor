using System;
using System.Threading;

// 命令接收者基类,只有命令接收者的子类可以接收命令
public class CommandReceiver
{
	protected string mName;				// 接收者名字
	protected long mDelayCmdCount;		// 此对象剩余未执行的延迟命令数量
	public string getName() { return mName; }
	public virtual void setName(string name) { mName = name; }
	public void addReceiveDelayCmd() { Interlocked.Increment(ref mDelayCmdCount); }
	public void removeReceiveDelayCmd() { Interlocked.Decrement(ref mDelayCmdCount); }
	public virtual void destroy()
	{
		;
	}
}