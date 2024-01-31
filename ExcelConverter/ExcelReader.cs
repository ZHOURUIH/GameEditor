using ExcelDataReader;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Text;

public class ExcelReader
{
	protected List<ExcelTable> mTableList;
	protected IExcelDataReader mReader;
	protected Stream mStream;
	protected DataSet mDataSet;
	public ExcelReader(string filePath)
	{
		mTableList = new List<ExcelTable>();
		Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
		try
		{
			mStream = new FileStream(filePath, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
		}
		catch
		{
			Console.WriteLine("文件打开失败,是否已在其他应用程序打开? 文件名:" + filePath);
			return;
		}
		mReader = ExcelReaderFactory.CreateOpenXmlReader(mStream);
		mDataSet = mReader.AsDataSet();
		int tableCount = mDataSet.Tables.Count;
		for(int i = 0; i < tableCount; ++i)
		{
			ExcelTable table = new ExcelTable();
			table.setTable(mDataSet.Tables[i]);
			mTableList.Add(table);
		}
	}
	public bool isValid() { return mReader != null; }
	public int getTableCount() { return mTableList.Count; }
	public ExcelTable getTable(int index) { return mTableList[index]; }
	public void close()
	{
		mReader?.Close();
		mStream?.Close();
	}
}