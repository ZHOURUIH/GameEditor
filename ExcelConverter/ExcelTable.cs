using ExcelDataReader;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Text;

public class ExcelTable
{
	protected DataTable mTable;
	public void setTable(DataTable table) { mTable = table; }
	public string getTableName() { return mTable.TableName; }
	public int getRowCount() { return mTable.Rows.Count; }
	public int getColumnCount() { return mTable.Columns.Count; }
	public object getCell(int row, int col) { return mTable.Rows[row][col]; }
}