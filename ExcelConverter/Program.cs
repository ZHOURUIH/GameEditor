using System;
using System.Collections.Generic;

class Program : FileUtility
{
	static void Main(string[] args)
	{
		if (!Config.parse("./Config.txt"))
		{
			Console.ReadKey();
			return;
		}

		// 先删除本地的文件
		deleteFolder(Config.SCRIPT_PATH);
		deleteFolder(Config.DATA_PATH);

		List<ExcelReader> readerList = new List<ExcelReader>();

		List<string> files = new List<string>();
		findFiles(Config.EXCEL_PATH, files, ".xlsx");
		for (int i = 0; i < files.Count; ++i)
		{
			// ~开头的是临时文件,不处理
			if(startWith(getFileName(files[i]), "~"))
			{
				continue;
			}
			ExcelReader reader = new ExcelReader(files[i]);
			if (!reader.isValid())
			{
				Console.ReadKey();
				return;
			}
			readerList.Add(reader);
		}

		ExcelConverter.generateTableData(readerList);
		ExcelConverter.generateRegister(readerList);
		ExcelConverter.generateExcelDefine(readerList);

		for (int i = 0; i < readerList.Count; ++i)
		{
			readerList[i].close();
		}

		Console.WriteLine("共" + files.Count + "个文件");
		Console.ReadKey();
    }
}