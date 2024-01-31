using System.Data;

public class ExcelTable
{
	protected DataTable mTable;
	public void setTable(DataTable table) { mTable = table; }
	public string getTableName() { return mTable.TableName; }
	public int getRowCount() { return mTable.Rows.Count; }
	public DataColumn getColumn(int index) { return mTable.Columns[index]; }
	public int getColumnCount() { return mTable.Columns.Count; }
	public object getCell(int row, int col) { return mTable.Rows[row][col]; }
}