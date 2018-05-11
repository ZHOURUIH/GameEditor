using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FirewallCrossConsole
{
	class Program
	{
		static void Main(string[] args)
		{
			string path = Directory.GetCurrentDirectory();
			if (args.Length > 0)
			{
				path = args[0];
			}
			// 如果路径是以/或者\\结尾,则去除最后一个字符,方便使用
			if(path.EndsWith("/") || path.EndsWith("\\"))
			{
				path = path.Substring(0, path.Length - 1);
			}
			// 如果当前在HelperExe目录中,就在path向上3层查找文件,否则就查找path中的文件
			if (path.EndsWith("HelperExe"))
			{
				path = StringUtility.getFilePath(path);
				path = StringUtility.getFilePath(path);
				path = StringUtility.getFilePath(path);
			}
			EditorCore editorCore = new EditorCore();
			editorCore.init(path);
			// 将列表中的所有文件都加入防火墙例外
			foreach (var item in editorCore.getProgramPathList())
			{
				bool ret = EditorUtility.addFirewallExceptions(item.Key, item.Value);
			}
		}
	}
}
