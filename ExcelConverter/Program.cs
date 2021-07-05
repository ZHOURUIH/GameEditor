using ExcelDataReader;
using System;
using System.Collections.Generic;
using System.IO;

class Program
{
	static void Main(string[] args)
	{
		List<string> files = new List<string>();
		string curPath = StringUtility.rightToLeft(Environment.CurrentDirectory);
		FileUtility.findFiles(StringUtility.getFilePath(curPath) + "/", files, ".xlsx");
		for (int i = 0; i < files.Count; ++i)
		{
			ExcelConverter.generateTableData(files[i]);
		}
		Console.WriteLine("共" + files.Count + "个文件");
		Console.ReadKey();
    }
}