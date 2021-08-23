using System;
using System.Collections.Generic;

public class MemberInfo
{
	public string mMemberName;
	public string mMemberType;
}

public class ExcelConverter : FileUtility
{
	public static int ROW_HEADER = 3;		// 前3行不是数据区域
	public static void generateTableData(List<ExcelReader> readerList)
	{
		for (int i = 0; i < readerList.Count; ++i)
		{
			ExcelReader reader = readerList[i];
			int tableCount = reader.getTableCount();
			for (int j = 0; j < tableCount; ++j)
			{
				convertTable(reader.getTable(j));
			}
		}
	}
	public static void generateRegister(List<ExcelReader> readerList)
	{
		// 生成ExcelRegister.cs
		string registerFile = "";
		line(ref registerFile, "using System;");
		line(ref registerFile, "using UnityEngine;");
		line(ref registerFile, "using System.Collections;");
		line(ref registerFile, "using System.Collections.Generic;");
		line(ref registerFile, "");
		line(ref registerFile, "public class ExcelRegister : FrameBase");
		line(ref registerFile, "{");
		line(ref registerFile, "\tpublic static void registeAll()");
		line(ref registerFile, "\t{");
		for (int i = 0; i < readerList.Count; ++i)
		{
			ExcelReader reader = readerList[i];
			int tableCount = reader.getTableCount();
			for (int j = 0; j < tableCount; ++j)
			{
				string tableName = reader.getTable(j).getTableName();

				line(ref registerFile, "\t\tregiste<" + tableName + ">(EXCEL_DATA." + nameToUpper(tableName, false) + ", \"" + tableName + "\");");
			}
		}
		line(ref registerFile, "\t}");
		line(ref registerFile, "\t//------------------------------------------------------------------------------------------------------------------------------");
		line(ref registerFile, "\tprotected static void registe<T>(int type, string name) where T : ExcelData");
		line(ref registerFile, "\t{");
		line(ref registerFile, "\t\tmExcelManager.registe(type, name, typeof(T));");
		line(ref registerFile, "\t}");
		line(ref registerFile, "}", false);
		writeTxtFile(Config.SCRIPT_PATH + "../" + "ExcelRegister.cs", registerFile);
	}
	public static void generateExcelDefine(List<ExcelReader> readerList)
	{
		List<string> nameList = new List<string>();
		nameList.Add("NONE");
		for (int i = 0; i < readerList.Count; ++i)
		{
			ExcelReader reader = readerList[i];
			int tableCount = reader.getTableCount();
			for (int j = 0; j < tableCount; ++j)
			{
				nameList.Add(nameToUpper(reader.getTable(j).getTableName(), false));
			}
		}

		string registerFile = "";
		line(ref registerFile, "using System;");
		line(ref registerFile, "");
		line(ref registerFile, "// Excel数据表格的表格定义");
		line(ref registerFile, "public class EXCEL_DATA");
		line(ref registerFile, "{");
		for (int i = 0; i < nameList.Count; ++i)
		{
			line(ref registerFile, "\tpublic static int " + nameList[i] + " = " + IToS(i) + ";");
		}
		line(ref registerFile, "}", false);
		writeTxtFile(Config.SCRIPT_PATH + "../" + "ExcelDefine.cs", registerFile);
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	protected static void convertTable(ExcelTable table)
	{
		string tableName = table.getTableName();
		int rowCount = table.getRowCount();
		int colCount = table.getColumnCount();
		if (rowCount <= ROW_HEADER || colCount <= 1)
		{
			Console.WriteLine("表格错误:行数:" + rowCount + ", 列数:" + colCount + ", 表名:" + tableName);
			return;
		}

		List<MemberInfo> memberList = new List<MemberInfo>();
		for (int i = 0; i < colCount; ++i)
		{
			// 第0行是每一列数据的类型(服务器使用),第1行是每一列数据的类型(客户端使用),第2行是字段类型
			MemberInfo info = new MemberInfo();
			info.mMemberName = (string)table.getCell(1, i);
			info.mMemberType = (string)table.getCell(2, i);
			memberList.Add(info);
		}

		// 生成二进制的data文件
		SerializerWrite fileWriter = new SerializerWrite();
		for (int i = ROW_HEADER; i < rowCount; ++i)
		{
			for (int j = 0; j < colCount; ++j)
			{
				object value = table.getCell(i, j);
				string colType = memberList[j].mMemberType;
				if (colType == "int")
				{
					if (value is DBNull)
					{
						value = 0;
					}
					else if(value is double)
					{
						value = (int)(double)value;
					}
					fileWriter.write((int)value);
				}
				else if (colType == "float")
				{
					if (value is DBNull)
					{
						value = 0.0f;
					}
					else if (value is double)
					{
						value = (float)(double)value;
					}
					fileWriter.write((float)value);
				}
				else if (colType == "string")
				{
					if (value is DBNull)
					{
						value = null;
					}
					fileWriter.writeString(value?.ToString());
				}
			}
		}
		if (fileWriter.getBuffer() != null)
		{
			writeFile(Config.DATA_PATH + tableName + ".data", fileWriter.getBuffer(), fileWriter.getDataSize());
		}
		else
		{
			Console.WriteLine("表格数据为空:" + tableName);
		}

		// 生成TableData.cs文件
		string csFile = "";
		line(ref csFile, "using System;");
		line(ref csFile, "using System.Collections.Generic;");
		line(ref csFile, "");
		line(ref csFile, "public class " + tableName + " : ExcelData");
		line(ref csFile, "{");
		for (int i = 1; i < memberList.Count; ++i)
		{
			line(ref csFile, "\tpublic " + memberList[i].mMemberType + " m" + memberList[i].mMemberName + ";");
		}
		line(ref csFile, "\tpublic override void read(SerializerRead reader)");
		line(ref csFile, "\t{");
		line(ref csFile, "\t\tbase.read(reader);");
		for (int i = 1; i < memberList.Count; ++i)
		{
			MemberInfo info = memberList[i];
			if (info.mMemberType == "string")
			{
				line(ref csFile, "\t\treader.readString(out m" + info.mMemberName + ");");
			}
			else
			{
				line(ref csFile, "\t\treader.read(out m" + info.mMemberName + ");");
			}
		}
		line(ref csFile, "\t}");
		line(ref csFile, "}", false);
		writeTxtFile(Config.SCRIPT_PATH + tableName + ".cs", csFile);

		Console.WriteLine("已转换:" + tableName);
	}
	protected static void line(ref string line, string text, bool returnLine = true)
	{
		if (returnLine)
		{
			line += text + "\n";
		}
		else
		{
			line += text;
		}
	}
	protected static string nameToUpper(string sqliteName, bool preUnderLine)
	{
		// 根据大写字母拆分
		List<string> macroList = new List<string>();
		int length = sqliteName.Length;
		int lastIndex = 0;
		// 从1开始,因为第0个始终都是大写,会截取出空字符串,最后一个字母也肯不会被分割
		for (int i = 1; i<length; ++i)
		{
			// 以大写字母为分隔符,但是连续的大写字符不能被分隔
			// 非连续数字也会分隔
			char curChar = sqliteName[i];
			char lastChar = sqliteName[i - 1];
			char nextChar = i + 1 < length ? sqliteName[i + 1] : '\0';
			if (isUpper(curChar) && (!isUpper(lastChar) || (nextChar != '\0' && !isUpper(nextChar))) ||
				isNumber(curChar) && (!isNumber(lastChar) || (nextChar != '\0' && !isNumber(nextChar))))
			{
				macroList.Add(sqliteName.Substring(lastIndex, i - lastIndex));
				lastIndex = i;
			}
		}
		macroList.Add(sqliteName.Substring(lastIndex, length - lastIndex));
		string headerMacro = "";
		for(int i = 0; i < macroList.Count; ++i)
		{
			headerMacro += "_" + toUpper(macroList[i]);
		}
		if (!preUnderLine)
		{
			headerMacro = headerMacro.Remove(0, 1);
		}
		return headerMacro;
	}
}