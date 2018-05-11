using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public interface EventHandler
{
	void registeEventHandler();
	void unregisteEventHandler();
	void notifyEvent(CORE_EVENT_TYPE type, List<string> param);
}