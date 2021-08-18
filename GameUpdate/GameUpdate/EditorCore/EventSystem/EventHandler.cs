using System;
using System.Collections.Generic;

public interface EventHandler
{
	void registeEventHandler();
	void unregisteEventHandler();
	void notifyEvent(CORE_EVENT_TYPE type, List<string> param);
}