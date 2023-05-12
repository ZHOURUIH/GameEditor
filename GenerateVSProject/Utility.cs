using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;
using static Utility;

class Program
{
	static void Main(string[] args)
	{
		FileStream configFile = new FileStream("./GenerateVSProjectConfig.txt", FileMode.Open);
		byte[] bytes = new byte[configFile.Length];
		configFile.Read(bytes, 0, (int)configFile.Length);
		string[] lines = removeAll(Encoding.UTF8.GetString(bytes), '\r').Split('\n');
		Dictionary<string, string> paramMap = new Dictionary<string, string>();
		foreach (string line in lines)
		{
			string[] paramPair = line.Split('=');
			if (paramPair.Length != 2)
			{
				continue;
			}
			paramMap.Add(paramPair[0], paramPair[1]);
		}
		if (!paramMap.TryGetValue("ProjectFilePath", out string projectFilePath))
		{
			return;
		}
		string[] projectFileList = projectFilePath.Split(',');
		foreach (string file in projectFileList)
		{
			generateVCXProject(file);
			generateVCXProjectFilters(file + ".filters");
		}
	}
	// projectPath需要是绝对路径
	protected static void generateVCXProject(string fileName)
	{
		if (!fileName.EndsWith(".vcxproj"))
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
		DeleteChildWithInnerChild(itemGroupNode, "ItemGroup", new List<string>() { "ClInclude", "ClCompile" });

		XmlNode newNode = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		// 查找当前目录中的所有代码文件.h .cpp .c .inl
		List<string> sourceFiles = new List<string>();
		string projectPath = getFilePath(fileName) + "/";
		findFiles(projectPath, sourceFiles, new List<string>() { ".h", ".cpp", ".c", ".inl" });
		foreach (string source in sourceFiles)
		{
			// 只编译.c和UnityBuild.cpp文件
			string fileNameNoDir = getFileName(source);
			if (fileNameNoDir.EndsWith(".c") || (fileNameNoDir.StartsWith("UnityBuild") && fileNameNoDir.EndsWith(".cpp")))
			{
				XmlElement newChild = doc.CreateElement("ClCompile", doc.DocumentElement.NamespaceURI);
				newChild.SetAttribute("Include", source.Remove(0, projectPath.Length).Replace('/', '\\'));
				newNode.AppendChild(newChild);
			}
			else
			{
				XmlElement newChild = doc.CreateElement("ClInclude", doc.DocumentElement.NamespaceURI);
				newChild.SetAttribute("Include", source.Remove(0, projectPath.Length).Replace('/', '\\'));
				newNode.AppendChild(newChild);

				// inl文件还有一个子节点
				if (fileNameNoDir.EndsWith(".inl"))
				{
					XmlElement filterType = doc.CreateElement("FileType", doc.DocumentElement.NamespaceURI);
					filterType.InnerText = "Document";
					newChild.AppendChild(filterType);
				}
			}
		}
		if (newNode.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(newNode);
		}

		string newFileName = projectPath + "new_" + getFileName(fileName);
		doc.Save(newFileName);
	}
	protected static void generateVCXProjectFilters(string fileName)
	{
		XmlDocument doc = openXML(fileName);
		XmlNamespaceManager nsMgr = new XmlNamespaceManager(doc.NameTable);
		nsMgr.AddNamespace("ns", "http://schemas.microsoft.com/developer/msbuild/2003");

		XmlNode itemGroupNode = doc.SelectSingleNode("/ns:Project", nsMgr);
		// 删除有ClInclude或者Filter的ItemGroup
		DeleteChildWithInnerChild(itemGroupNode, "ItemGroup", new List<string>() { "ClInclude", "Filter", "ClCompile" });

		XmlElement filterItemGroup = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		string projectPath = getFilePath(fileName) + "/";
		List<string> sourceDirs = new List<string>();
		findFolders(projectPath, sourceDirs);
		foreach (string dir in sourceDirs)
		{
			List<string> tempFileList = new List<string>();
			findFiles(dir, tempFileList, new List<string>() { ".h", ".cpp", ".c", ".inl" });
			if (tempFileList.Count == 0)
			{
				continue;
			}
			XmlElement filterNode = doc.CreateElement("Filter", doc.DocumentElement.NamespaceURI);
			filterNode.SetAttribute("Include", dir.Remove(0, projectPath.Length).Replace('/', '\\'));
			XmlElement identifierNode = doc.CreateElement("UniqueIdentifier", doc.DocumentElement.NamespaceURI);
			identifierNode.InnerText = Guid.NewGuid().ToString();
			filterNode.AppendChild(identifierNode);
			filterItemGroup.AppendChild(filterNode);
		}
		if (filterItemGroup.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(filterItemGroup);
		}
		
		XmlElement includeItemGroup = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		List<string> fileList = new List<string>();
		findFiles(projectPath, fileList, new List<string>() { ".h", ".cpp", ".c", ".inl" });
		foreach (string file in fileList)
		{
			// 只编译.c和UnityBuild.cpp文件,其他的都是include
			string fileNameNoDir = getFileName(file);
			if (!fileNameNoDir.EndsWith(".c") && (!fileNameNoDir.StartsWith("UnityBuild") || !fileNameNoDir.EndsWith(".cpp")))
			{
				XmlElement includeNode = doc.CreateElement("ClInclude", doc.DocumentElement.NamespaceURI);
				string relativeFileName = file.Remove(0, projectPath.Length).Replace('/', '\\');
				includeNode.SetAttribute("Include", relativeFileName);
				string filter = getFilePath(relativeFileName);
				if (filter != null && filter.Length > 0)
				{
					XmlElement filterNode = doc.CreateElement("Filter", doc.DocumentElement.NamespaceURI);
					filterNode.InnerText = filter.Replace('/', '\\');
					includeNode.AppendChild(filterNode);
				}
				includeItemGroup.AppendChild(includeNode);
			}
		}
		if (includeItemGroup.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(includeItemGroup);
		}

		XmlElement compileItemGroup = doc.CreateElement("ItemGroup", doc.DocumentElement.NamespaceURI);
		foreach (string file in fileList)
		{
			// 只编译.c和UnityBuild.cpp文件
			string fileNameNoDir = getFileName(file);
			if (fileNameNoDir.EndsWith(".c") || (fileNameNoDir.StartsWith("UnityBuild") && fileNameNoDir.EndsWith(".cpp")))
			{
				XmlElement conpileNode = doc.CreateElement("ClCompile", doc.DocumentElement.NamespaceURI);
				string relativeFileName = file.Remove(0, projectPath.Length).Replace('/', '\\');
				conpileNode.SetAttribute("Include", relativeFileName);
				string filter = getFilePath(relativeFileName);
				if (filter != null && filter.Length > 0)
				{
					XmlElement filterNode = doc.CreateElement("Filter", doc.DocumentElement.NamespaceURI);
					filterNode.InnerText = filter.Replace('/', '\\');
					conpileNode.AppendChild(filterNode);
				}
				compileItemGroup.AppendChild(conpileNode);
			}
		}
		if (compileItemGroup.ChildNodes.Count > 0)
		{
			itemGroupNode.AppendChild(compileItemGroup);
		}

		string newFileName = projectPath + "new_" + getFileName(fileName);
		doc.Save(newFileName);
	}
}