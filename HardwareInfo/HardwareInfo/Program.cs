using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Management;
using System.Collections;
using System.Runtime.InteropServices;
using System.IO;

class Program
{
	[STAThread]
	static void Main(string[] args)
	{
        if(args.Length != 1)
        {
            return;
        }
        HardwareInfo info = new HardwareInfo();
		info.init();
		// 将硬件信息计算出的申请码写入文件
		string content = "";
		content += info.getOriMAC();
		content += "\r\n";
		content += info.getHDD();
		content += "\r\n";
		content += info.getMainbord();
		content += "\r\n";
		content += info.getCPU();
		content += "\r\n";
		content += info.getBIOS();
		content += "\r\n";
		content += info.getMainboardType();
		content += "\r\n";
		content += info.getCurMAC();
        string fileName = args[0] + "Hardware.txt";
        File.Delete(fileName);
        FileStream file = new FileStream(fileName, FileMode.Create);
        byte[] bytes = Encoding.UTF8.GetBytes(content);
        file.Write(bytes, 0, bytes.Length);
        file.Close();
	}
}