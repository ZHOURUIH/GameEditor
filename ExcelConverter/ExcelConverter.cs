using System;
using System.Collections.Generic;

public class MemberInfo
{
	public string mMemberName;
	public string mMemberType;
}

public class ExcelConverter : FileUtility
{
	public static int ROW_HEADER = 2;		// 前两行不是数据区域
	public static void generateTableData(string filePath)
	{
		ExcelReader reader = new ExcelReader(filePath);
		int rowCount = reader.getRowCount();
		int colCount = reader.getColCount();
		if (rowCount <= ROW_HEADER || colCount <= 1)
		{
			Console.WriteLine("表格错误:行数:" + rowCount + ", 列数:" + colCount);
			reader.close();
			return;
		}

		string tableName = getFileNameNoSuffix(filePath, true);
		List<MemberInfo> memberList = new List<MemberInfo>();
		for (int i = 0; i < colCount; ++i)
		{
			// 第1行是每一列数据的类型
			MemberInfo info = new MemberInfo();
			info.mMemberName = (string)reader.getCell(0, i);
			info.mMemberType = (string)reader.getCell(1, i);
			memberList.Add(info);
		}
		if(memberList[0].mMemberName != "ID")
		{
			Console.WriteLine("第一列必须为ID");
			reader.close();
			return;
		}

		SerializerWrite fileWriter = new SerializerWrite();
		rowCount -= ROW_HEADER;
		for(int i = ROW_HEADER; i < rowCount; ++i)
		{
			for (int j = 0; j < colCount; ++j)
			{
				dynamic value = reader.getCell(i, j);
				string colType = memberList[j].mMemberType;
				if (colType == "int")
				{
					fileWriter.write((int)value);
				}
				else if (colType == "float")
				{
					fileWriter.write((float)value);
				}
				else if(colType == "string")
				{
					fileWriter.writeString((string)value);
				}
			}
		}
		writeFile(getFilePath(filePath) + "/" + tableName + ".data", fileWriter.getBuffer(), fileWriter.getDataSize());

		// 生成TableData.cs文件
		string dataClassName = "Data" + tableName;
		string csFile = "";
		line(ref csFile, "using System;");
		line(ref csFile, "using System.Collections.Generic;");
		line(ref csFile, "");
		line(ref csFile, "public class " + dataClassName + " : ExcelData");
		line(ref csFile, "{");
		for(int i = 1; i < memberList.Count; ++i)
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
		line(ref csFile, "}");
		writeTxtFile(getFilePath(filePath) + "/" + dataClassName + ".cs", csFile);

		reader.close();

		Console.WriteLine("已转换:" + tableName);
	}
	public static void line(ref string line, string text)
	{
		line += text + "\n";
	}
}