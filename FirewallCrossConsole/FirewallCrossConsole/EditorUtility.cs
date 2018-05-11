using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NetFwTypeLib;

public class EditorUtility
{
	public static T createInstance<T>(Type classType, params object[] param) where T : class
	{
		return Activator.CreateInstance(classType, param) as T;
	}
	// WINDOWS防火墙添加例外程序,name程序名称,filename程序的绝对路径(包含文件名)
	public static bool addFirewallExceptions(string name, string filename)
	{
		try
		{
			StringUtility.leftToRight(ref filename);
			// 创建firewall管理接口的实例
			INetFwMgr netFwMgr = createInstance<INetFwMgr>(Type.GetTypeFromProgID("HNetCfg.FwMgr"));
			INetFwAuthorizedApplications profileApplication = netFwMgr.LocalPolicy.CurrentProfile.AuthorizedApplications;
			// 创建一个认证程序接口的实例
			INetFwAuthorizedApplication app = createInstance<INetFwAuthorizedApplication>
				(Type.GetTypeFromProgID("HNetCfg.FwAuthorizedApplication"));
			app.Name = name;					// 在例外列表里，程序显示的名称
			app.ProcessImageFileName = filename;// 程序的绝对路径
			app.Enabled = true;                 // 是否启用该规则
			profileApplication.Add(app);
			return true;
		}
		catch
		{
			return false;
		}
	}
	// WINDOWS防火墙添加例外端口,name端口名,portNum端口号,type端口类型
	public static bool addFirewallPort(string name, int portNum, NET_FW_IP_PROTOCOL_ type)
	{
		try
		{
			// 创建端口接口实例
			INetFwOpenPort port = createInstance<INetFwOpenPort>(Type.GetTypeFromProgID("HNetCfg.FwOpenPort"));
			port.Name = name;           //端口名
			port.Port = portNum;        //端口号
			port.Protocol = type;       //协议类型
			port.Enabled = true;        //启用
			// 创建firewall管理接口的实例
			INetFwMgr netFwMgr = createInstance<INetFwMgr>(Type.GetTypeFromProgID("HNetCfg.FwMgr"));
			netFwMgr.LocalPolicy.CurrentProfile.GloballyOpenPorts.Add(port);
			return true;
		}
		catch
		{
			return false;
		}
	}
}