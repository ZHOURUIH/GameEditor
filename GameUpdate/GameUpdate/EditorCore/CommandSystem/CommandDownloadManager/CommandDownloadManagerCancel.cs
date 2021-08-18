using System;
using System.Collections.Generic;

public class CommandDownloadManagerCancel : Command
{
	public override void init()
	{
		base.init();
	}
	public override void execute()
	{
		mEditorCore.sendEvent(CORE_EVENT_TYPE.CANCEL_UPDATE);
	}
	public override string showDebugInfo()
	{
		return base.showDebugInfo();
	}
}