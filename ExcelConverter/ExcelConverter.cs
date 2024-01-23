using System;
using System.Collections.Generic;
using static MathUtility;

public enum OWNER : byte
{
	NONE,
	CLIENT,
	SERVER,
	BOTH,
}

public class ExcelInfo
{
	public string mName;
	public string mComment;
	public ExcelTable mTable;
	public List<MemberInfo> mMemberList = new List<MemberInfo>();
	public OWNER mOwner;
}

public class MemberInfo
{
	public string mMemberName;
	public string mMemberType;
	public string mComment;
	public string mSeperate;        // 如果是列表类型,则表示列表类型的分隔符
	public OWNER mOwner;
}

public class ExcelConverter : FileUtility
{
	public const int ROW_HEADER = 7;        // 前7行不是数据区域
	public static void generate(List<ExcelReader> readerList)
	{
		// 解析
		List<ExcelInfo> allExcelInfo = new List<ExcelInfo>();
		for (int i = 0; i < readerList.Count; ++i)
		{
			allExcelInfo.Add(parseTableMemberInfo(readerList[i].getTable(0)));
		}

		foreach (var item in allExcelInfo)
		{
			if (item.mOwner == OWNER.SERVER || item.mOwner == OWNER.NONE)
			{
				continue;
			}
			// 转换表格文件
			if (!convertTable(Config.mExcelBytesPath, item.mTable, item.mMemberList))
			{
				Console.ReadKey();
			}
			// 生成ExcelData.cs
			generateExcelDataFile(item, Config.mExcelDataHotFixPath);
			// 生成ExcelTable.cs
			generateExcelTableFile(item.mName, Config.mExcelTableHotFixPath);
		}

		// 在上一层目录生成ExcelRegister.cs
		generateCSharpExcelRegisteFileFile(allExcelInfo, getFilePath(Config.mExcelDataHotFixPath) + "/");
		generateCSharpExcelDeclare(allExcelInfo, Config.mHotFixCommonPath);
	}
	// ExcelTable.cs
	protected static void generateExcelTableFile(string tableName, string path)
	{
		string dataClassName = "ED" + tableName;
		string tableClassName = "Excel" + tableName;
		string table = "";
		line(ref table, "using System;");
		line(ref table, "using System.Collections.Generic;");
		line(ref table, "");
		line(ref table, "public partial class " + tableClassName + " : ExcelTable");
		line(ref table, "{");
		line(ref table, "\t// 由于基类无法知道子类的具体类型,所以将List类型的列表定义到子类中.因为大部分时候外部使用的都是List类型的列表");
		line(ref table, "\t// 并且ILRuntime热更对于模板支持不太好,所以尽量避免使用模板");
		line(ref table, "\t// 此处定义一个List是为了方便外部可直接获取,避免每次queryAll时都会创建列表");
		line(ref table, "\tprotected List<" + dataClassName + "> mDataList;");
		line(ref table, "\tprotected bool mDataAvailable;");
		line(ref table, "\tpublic " + tableClassName + "()");
		line(ref table, "\t{");
		line(ref table, "\t\tmDataList = new List<" + dataClassName + ">();");
		line(ref table, "\t\tmDataAvailable = false;");
		line(ref table, "\t}");
		line(ref table, "\tpublic " + dataClassName + " query(int id, bool errorIfNull = true)");
		line(ref table, "\t{");
		line(ref table, "\t\treturn getData<" + dataClassName + ">(id, errorIfNull);");
		line(ref table, "\t}");
		line(ref table, "\tpublic List<" + dataClassName + "> queryAll()");
		line(ref table, "\t{");
		line(ref table, "\t\tif (!mDataAvailable)");
		line(ref table, "\t\t{");
		line(ref table, "\t\t\tforeach (var item in getDataList())");
		line(ref table, "\t\t\t{");
		line(ref table, "\t\t\t\tmDataList.Add(item.Value as " + dataClassName + ");");
		line(ref table, "\t\t\t}");
		line(ref table, "\t\t\tmDataAvailable = true;");
		line(ref table, "\t\t}");
		line(ref table, "\t\treturn mDataList;");
		line(ref table, "\t}");
		line(ref table, "}", false);
		writeTxtFileBOM(path + tableClassName + ".cs", GB2312ToUTF8(table));
	}
	// ExcelData.cs
	protected static void generateExcelDataFile(ExcelInfo info, string path)
	{
		string file = "";
		string dataClassName = "ED" + info.mName;
		line(ref file, "using System;");
		line(ref file, "using System.Collections.Generic;");
		line(ref file, "using UnityEngine;");
		line(ref file, "");
		line(ref file, "// " + info.mComment);
		line(ref file, "public class " + dataClassName + " : ExcelData");
		line(ref file, "{");
		int memberCount = info.mMemberList.Count;
		HashSet<string> listMemberSet = new HashSet<string>();
		List<KeyValuePair<string, string>> listMemberList = new List<KeyValuePair<string, string>>();
		for (int i = 0; i < memberCount; ++i)
		{
			MemberInfo member = info.mMemberList[i];
			if (member.mMemberName == "ID")
			{
				continue;
			}
			if (member.mOwner == OWNER.SERVER || member.mOwner == OWNER.NONE)
			{
				continue;
			}
			string typeName = member.mMemberType;
			// 列表类型的成员变量存储到单独的列表,因为需要分配内存
			if (typeName.Contains("List"))
			{
				listMemberList.Add(new KeyValuePair<string, string>(typeName, member.mMemberName));
				listMemberSet.Add(member.mMemberName);
			}
			string memberLine = "\tpublic " + typeName + " m" + member.mMemberName + ";";
			int tabCount = generateAlignTableCount(memberLine, 44);
			for (int j = 0; j < tabCount; ++j)
			{
				memberLine += '\t';
			}
			memberLine += "// " + info.mMemberList[i].mComment;
			line(ref file, memberLine);
		}
		if (listMemberList.Count > 0)
		{
			line(ref file, "\tpublic " + dataClassName + "()");
			line(ref file, "\t{");
			foreach (var item in listMemberList)
			{
				line(ref file, "\t\tm" + item.Value + " = new " + item.Key + "();");
			}
			line(ref file, "\t}");
		}
		line(ref file, "\tpublic override void read(SerializerRead reader)");
		line(ref file, "\t{");
		line(ref file, "\t\tbase.read(reader);");
		for (int i = 0; i < memberCount; ++i)
		{
			MemberInfo memberInfo = info.mMemberList[i];
			if (memberInfo.mMemberName == "ID")
			{
				continue;
			}
			if (memberInfo.mOwner == OWNER.SERVER || memberInfo.mOwner == OWNER.NONE)
			{
				continue;
			}
			string typeName = memberInfo.mMemberType;
			if (typeName == "string")
			{
				line(ref file, "\t\treader.readString(out m" + memberInfo.mMemberName + ");");
			}
			else if (listMemberSet.Contains(memberInfo.mMemberName))
			{
				line(ref file, "\t\treader.readList(m" + memberInfo.mMemberName + ");");
			}
			else
			{
				line(ref file, "\t\treader.read(out m" + memberInfo.mMemberName + ");");
			}
		}
		line(ref file, "\t}");
		line(ref file, "}", false);
		writeTxtFileBOM(path + dataClassName + ".cs", GB2312ToUTF8(file));
	}
	// ExcelRegister.cs文件
	protected static void generateCSharpExcelRegisteFileFile(List<ExcelInfo> excelInfoList, string path)
	{
		string hotFixfile = "";
		line(ref hotFixfile, "using System;");
		line(ref hotFixfile, "using static GBR;");
		line(ref hotFixfile, "using static FrameBase;");
		line(ref hotFixfile, "");
		line(ref hotFixfile, "public class ExcelRegisterILR");
		line(ref hotFixfile, "{");
		line(ref hotFixfile, "\tpublic static void registeAll()");
		line(ref hotFixfile, "\t{");
		foreach (var item in excelInfoList)
		{
			if (item.mOwner == OWNER.SERVER || item.mOwner == OWNER.NONE)
			{
				continue;
			}
			string lineStr = "\t\tregisteTable(out mExcel%s, typeof(ED%s), \"%s\");";
			lineStr = replaceAll(lineStr, "%s", item.mName);
			line(ref hotFixfile, lineStr);
		}
		line(ref hotFixfile, "");
		line(ref hotFixfile, "\t\t// 进入热更以后,所有资源都处于可用状态");
		line(ref hotFixfile, "\t\tmExcelManager.resourceAvailable();");
		line(ref hotFixfile, "\t}");
		line(ref hotFixfile, "\t//------------------------------------------------------------------------------------------------------------------------------");
		line(ref hotFixfile, "\tprotected static void registeTable<T>(out T table, Type dataType, string tableName) where T : ExcelTable");
		line(ref hotFixfile, "\t{");
		line(ref hotFixfile, "\t\ttable = mExcelManager.registe(tableName, typeof(T), dataType) as T;");
		line(ref hotFixfile, "\t}");
		line(ref hotFixfile, "}", false);

		writeTxtFileBOM(path + "ExcelRegisterILR.cs", GB2312ToUTF8(hotFixfile));
	}
	// GameBaseExcel.cs文件
	protected static void generateCSharpExcelDeclare(List<ExcelInfo> infoList, string path)
	{
		// 热更工程中的表格注册
		string hotFixfile = "";
		line(ref hotFixfile, "using System;");
		line(ref hotFixfile, "");
		line(ref hotFixfile, "// FrameBase的部分类,用于定义Excel表格的对象");
		line(ref hotFixfile, "public partial class GBR");
		line(ref hotFixfile, "{");
		foreach (ExcelInfo info in infoList)
		{
			if (info.mOwner == OWNER.SERVER || info.mOwner == OWNER.NONE)
			{
				continue;
			}
			line(ref hotFixfile, "\tpublic static Excel" + info.mName + " mExcel" + info.mName + ";");
		}
		line(ref hotFixfile, "}", false);

		writeTxtFileBOM(path + "GameBaseExcelILR.cs", GB2312ToUTF8(hotFixfile));
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------------
	protected static OWNER stringToOwner(string str)
	{
		if (str == "Client")
		{
			return OWNER.CLIENT;
		}
		if (str == "Server")
		{
			return OWNER.SERVER;
		}
		if (str == "Both")
		{
			return OWNER.BOTH;
		}
		if (str == "None")
		{
			return OWNER.NONE;
		}
		Console.WriteLine("归属转换错误:" + str);
		Console.ReadKey();
		return OWNER.NONE;
	}
	protected static ExcelInfo parseTableMemberInfo(ExcelTable table)
	{
		ExcelInfo excelInfo = new ExcelInfo();
		excelInfo.mTable = table;
		excelInfo.mName = table.getTableName();
		int rowCount = table.getRowCount();
		int colCount = table.getColumnCount();
		if (rowCount <= ROW_HEADER || colCount <= 1)
		{
			Console.WriteLine("表格错误:行数:" + rowCount + ", 列数:" + colCount + ", 表名:" + excelInfo.mName);
			Console.ReadKey();
			return null;
		}

		try
		{
			excelInfo.mComment = (string)table.getCell(0, 0);
			excelInfo.mOwner = stringToOwner((string)table.getCell(0, 1));
			for (int i = 0; i < colCount; ++i)
			{
				// 第0行是每一列数据的类型,第1行是字段类型
				MemberInfo info = new MemberInfo();
				info.mOwner = stringToOwner((string)table.getCell(1, i));
				if (info.mOwner == OWNER.SERVER || info.mOwner == OWNER.NONE)
				{
					info.mComment = (string)table.getCell(6, i);
					excelInfo.mMemberList.Add(info);
					continue;
				}
				info.mMemberName = (string)table.getCell(2, i);
				string memberType = (string)table.getCell(3, i);
				if (memberType.StartsWith("List<"))
				{
					// 没有填则是以换行符为分隔
					if (memberType[memberType.Length - 1] == '>')
					{
						info.mMemberType = memberType;
						info.mSeperate = "\n";
					}
					else
					{
						int preIndex = memberType.IndexOf('(');
						int endIndex = memberType.IndexOf(')');
						if (preIndex >= 0 && endIndex >= 0)
						{
							info.mMemberType = memberType.Substring(0, preIndex);
							info.mSeperate = memberType.Substring(preIndex + 1, endIndex - preIndex - 1);
						}
						else
						{
							Console.WriteLine("列表分隔符错误: column:" + i + ", table:" + excelInfo.mName);
							Console.ReadKey();
						}
					}
				}
				else
				{
					info.mMemberType = memberType;
					info.mSeperate = null;
				}
				info.mComment = (string)table.getCell(6, i);
				excelInfo.mMemberList.Add(info);
			}
		}
		catch (Exception e)
		{
			Console.WriteLine("解析表头时错误,表格:" + table.getTableName() + ", info:" + e.Message);
			Console.ReadKey();
		}
		return excelInfo;
	}
	protected static bool convertTable(string path, ExcelTable table, List<MemberInfo> memberInfoList)
	{
		try
		{
			string tableName = table.getTableName();
			int rowCount = table.getRowCount();
			int colCount = table.getColumnCount();
			// 生成二进制的data文件
			SerializerWrite fileWriter = new SerializerWrite();
			for (int i = ROW_HEADER; i < rowCount; ++i)
			{
				for (int j = 0; j < colCount; ++j)
				{
					object value = table.getCell(i, j);
					MemberInfo member = memberInfoList[j];
					if (member.mOwner == OWNER.SERVER || member.mOwner == OWNER.NONE)
					{
						continue;
					}
					string colType = member.mMemberType;
					if (colType == "int")
					{
						if (value is DBNull)
						{
							value = 0;
						}
						else if (value is double)
						{
							value = (int)(double)value;
						}
						else if (value is string)
						{
							value = SToI((string)value);
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
						else if (value is string)
						{
							value = SToF((string)value);
						}
						fileWriter.write((float)value);
					}
					else if (colType == "bool")
					{
						if (value is DBNull)
						{
							value = 0;
						}
						else if (value is double)
						{
							value = (int)(double)value;
						}
						else if (value is string)
						{
							value = SToI((string)value);
						}
						fileWriter.write((int)value > 0);
					}
					else if (colType == "List<int>")
					{
						if (value is DBNull)
						{
							value = null;
						}
						string str = value?.ToString();
						if (str != null && str[0] == '[' && str[str.Length - 1] == ']')
						{
							str = str.Remove(0, 1);
							str = str.Remove(str.Length - 1, 1);
						}
						fileWriter.writeList(stringToInts(str));
					}
					else if (colType == "List<float>")
					{
						if (value is DBNull)
						{
							value = null;
						}
						string str = value?.ToString();
						if (str != null && str[0] == '[' && str[str.Length - 1] == ']')
						{
							str = str.Remove(0, 1);
							str = str.Remove(str.Length - 1, 1);
						}
						fileWriter.writeList(stringToFloats(str));
					}
					else if (colType == "List<string>")
					{
						if (value is DBNull)
						{
							value = null;
						}
						if (member.mSeperate == "\n")
						{
							splitLine(GB2312ToUTF8(value?.ToString()), out string[] lines);
							if (lines != null)
							{
								fileWriter.writeList(new List<string>(lines));
							}
							else
							{
								fileWriter.writeList(new List<string>());
							}
						}
						else
						{
							fileWriter.writeList(stringToStrings(GB2312ToUTF8(value?.ToString()), member.mSeperate));
						}
					}
					else if (colType == "Vector2")
					{
						if (value is DBNull)
						{
							value = null;
						}
						if (!(value is string))
						{
							Console.WriteLine("Vector2单元格的格式必须为string");
							return false;
						}
						string[] splitList = split((string)value, true, ",");
						if (splitList == null || splitList.Length != 2)
						{
							Console.WriteLine("Vector2单元格的内容错误,字段名:" + member.mMemberName + ", 表格:" + tableName + ", ID:" + (int)table.getCell(i, 0));
							return false;
						}
						fileWriter.write(SToVector2(value?.ToString()));
					}
					else if (colType == "Vector2Int")
					{
						if (value is DBNull)
						{
							value = null;
						}
						if (!(value is string))
						{
							Console.WriteLine("Vector2Int单元格的格式必须为string");
							return false;
						}
						string[] splitList = split((string)value, true, ",");
						if (splitList == null || splitList.Length != 2)
						{
							Console.WriteLine("Vector2Int单元格的内容错误,字段名:" + member.mMemberName + ", 表格:" + tableName + ", ID:" + (int)table.getCell(i, 0));
							return false;
						}
						fileWriter.write(SToVector2Int(value?.ToString()));
					}
					else if (colType == "Vector3")
					{
						if (value is DBNull)
						{
							value = null;
						}
						if (!(value is string))
						{
							Console.WriteLine("Vector3单元格的格式必须为string");
							return false;
						}
						string[] splitList = split((string)value, true, ",");
						if (splitList == null || splitList.Length != 3)
						{
							Console.WriteLine("Vector3单元格的内容错误,字段名:" + member.mMemberName + ", 表格:" + tableName + ", ID:" + (int)table.getCell(i, 0));
							return false;
						}
						fileWriter.write(SToVector3(value?.ToString()));
					}
					else if (colType == "string")
					{
						if (value is DBNull)
						{
							value = null;
						}
						fileWriter.writeString(GB2312ToUTF8(value?.ToString()));
					}
				}
			}
			if (fileWriter.getBuffer() == null)
			{
				Console.WriteLine("表格数据为空:" + tableName);
				return false;
			}
			// 重新计算密钥
			string key = generateMD5("ASLD" + tableName) + "23y35y983";
			byte[] buffer = fileWriter.getBuffer();
			int dataSize = fileWriter.getDataSize();
			for (int i = 0; i < dataSize; ++i)
			{
				buffer[i] = (byte)((buffer[i] - ((i << 1) & 0xFF)) ^ key[i % key.Length]);
			}

			writeFile(path + tableName + ".bytes", buffer, dataSize);
			Console.WriteLine("已转换:" + tableName);
		}
		catch (Exception e)
		{
			Console.WriteLine("转换表格内容时报错,表格:" + table.getTableName() + ", info:" + e.Message);
			return false;
		}
		return true;
	}
	protected static void line(ref string line, string text, bool returnLine = true)
	{
		if (returnLine)
		{
			line += text + "\r\n";
		}
		else
		{
			line += text;
		}
	}
}