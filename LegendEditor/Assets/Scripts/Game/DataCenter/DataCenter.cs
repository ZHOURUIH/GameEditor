using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class DataCenter : FrameComponent
{
	public DCPlayerInfo mPlayerInfo;
	public DCMonsterInfo mMonsterInfo;
	public DataCenter(string name)
		:base(name)
	{
		mPlayerInfo = new DCPlayerInfo();
		mMonsterInfo = new DCMonsterInfo();
	}
}