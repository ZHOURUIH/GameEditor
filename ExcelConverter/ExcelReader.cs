using ExcelDataReader;
using System;
using System.Data;
using System.IO;
using System.Text;

public class ExcelReader
{
	protected IExcelDataReader mReader;
	protected Stream mStream;
	protected DataSet mDataSet;
	protected DataTable mTable;
	protected int mRowCount;
	protected int mColCount;
	public ExcelReader(string filePath)
	{
		System.Text.Encoding.RegisterProvider(System.Text.CodePagesEncodingProvider.Instance);
		mStream = File.OpenRead(filePath);
		mReader = ExcelReaderFactory.CreateOpenXmlReader(mStream);
		mDataSet = mReader.AsDataSet();
		if(mDataSet.Tables.Count == 0)
		{
			Console.WriteLine("Error: 文件中没有表格:" + filePath);
		}
		mTable = mDataSet.Tables[0];
		mRowCount = mTable.Rows.Count;
		mColCount = mTable.Columns.Count;
	}
	public int getRowCount() { return mRowCount; }
	public int getColCount() { return mColCount; }
	public object getCell(int row, int col) { return mTable.Rows[row][col]; }
	public void close()
	{
		mReader.Close();
		mStream.Close();
	}
}