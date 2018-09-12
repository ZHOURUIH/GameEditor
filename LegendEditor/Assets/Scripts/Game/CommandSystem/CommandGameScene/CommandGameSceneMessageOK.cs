using UnityEngine;
using System.Collections;

public class CommandGameSceneMessageOK : Command
{
	public string mMessage;
	public string mButtonLabel;
	public override void init()
	{
		base.init();
		mMessage = "";
		mButtonLabel = "";
	}
	public override void execute()
	{
		;
	}
	public override string showDebugInfo()
	{
		return this.GetType().ToString() + " : mMessage : " + mMessage + ", mButtonLabel" + mButtonLabel;
	}
}