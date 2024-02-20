using System;
using System.Collections.Generic;
using System.Xml;
using static Utility;

class Program
{
	static void Main(string[] args)
	{
		if (!Config.init("./GenerateCSharpVSProject_Config.txt"))
		{
			Console.Read();
			return;
		}
		
		foreach (var item in Config.mParamMap)
		{
			if (!item.Value.TryGetValue("ProjectFilePath", out string projectFilePath))
			{
				return;
			}
			item.Value.TryGetValue("IncludePath", out string includePath);
			generateVCXProject(projectFilePath, includePath?.Split(','));
		}
	}
	// projectPath需要是绝对路径
	protected static void generateVCXProject(string fileName, string[] includePathList)
	{
		if (!fileName.EndsWith(".csproj"))
		{
			Console.WriteLine("项目文件名错误:" + fileName);
			Console.Read();
			return;
		}

		XmlDocument doc = openXML(fileName);
		XmlNamespaceManager nsMgr = new XmlNamespaceManager(doc.NameTable);
		nsMgr.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003");

		XmlNode itemGroupNode = doc.SelectSingleNode("/ns:Project", nsMgr);
		// 删除有ClInclude或者ClCompile的ItemGroup
		DeleteChildWithInnerChild(itemGroupNode, "ItemGroup", new List<string>() { "Compile" });

		XmlNode newNode = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		// 查找当前目录中的所有代码文件.cs
		List<string> sourceFiles = new List<string>();
		string projectPath = getFilePath(fileName) + "/";
		if (includePathList == null || includePathList.Length == 0)
		{
			findFiles(projectPath, sourceFiles, new List<string>() { ".cs" });
		}
		else
		{
			for (int i = 0; i < includePathList.Length; ++i)
			{
				findFiles(includePathList[i], sourceFiles, new List<string>() { ".cs" });
			}
		}
		foreach (string source in sourceFiles)
		{
			XmlElement newChild = doc.CreateElement("Compile", doc.DocumentElement.NamespaceURI);
			newChild.SetAttribute("Include", source.Remove(0, projectPath.Length).Replace('/', '\\'));
			newNode.AppendChild(newChild);
		}
		if (newNode.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(newNode);
		}
		doc.Save(fileName);
	}
	protected static XmlNode findChild(XmlNode node, string name)
	{
		for (int i = 0; i < node.ChildNodes.Count; ++i)
		{
			XmlNode child = node.ChildNodes[i];
			if (child.Name == name)
			{
				return child;
			}
		}
		return null;
	}
}