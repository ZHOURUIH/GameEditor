using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class CommandDownloadManagerCancel : Command
{
	public override void init()
	{
		base.init();
	}
	public override void execute()
	{
		mEditorCore.sendEvent(CORE_EVENT_TYPE.CET_CANCEL_UPDATE);
	}
	public override string showDebugInfo()
	{
		return base.showDebugInfo();
	}
}