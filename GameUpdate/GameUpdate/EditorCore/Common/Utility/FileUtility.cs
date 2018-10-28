using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Security.Cryptography;
using System.Threading;

public class FileUtility
{
	public static void validPath(ref string path)
	{
		if (path.Length > 0)
		{
			// 不以/结尾,则加上/
			if (path[path.Length - 1] != '/')
			{
				path += "/";
			}
		}
	}
	// 打开一个二进制文件,fileName为绝对路径
	public static void openFile(string fileName, ref byte[] fileBuffer, ref int fileSize)
	{
		FileStream fs = new FileStream(fileName, FileMode.Open, FileAccess.Read);
		fileSize = (int)fs.Length;
		fileBuffer = new byte[fileSize];
		fs.Read(fileBuffer, 0, fileSize);
		fs.Close();
	}
	// 写入一个二进制文件,fileName为绝对路径
	public static bool writeFile(string fileName, byte[] buffer, int size, bool appenData = false)
	{
		// 检测路径是否存在,如果不存在就创建一个
		createDir(StringUtility.getFilePath(fileName));
		try
		{
			FileStream file = new FileStream(fileName, FileMode.OpenOrCreate, FileAccess.Write, FileShare.ReadWrite);
			if (appenData)
			{
				file.Seek(0, SeekOrigin.End);
			}
			file.Write(buffer, 0, size);
			file.Flush();
			file.Close();
		}
		catch (Exception e)
		{
			EditorBase.logConsole("exception!! write file : " + fileName + "----thread : " + Thread.CurrentThread.ManagedThreadId + ", message : " + e.Message);
			return false;
		}
		return true;
	}
	// 打开一个文本文件,fileName为绝对路径
	public static string openTxtFile(string fileName)
	{
		try
		{
			StreamReader streamReader = new StreamReader(fileName, Encoding.UTF8);
			if(streamReader == null)
			{
				return "";
			}
			string fileBuffer = streamReader.ReadToEnd();
			streamReader.Close();
			return fileBuffer;
		}
		catch(Exception)
		{
			return "";
		}
	}
	// 写一个文本文件,fileName为绝对路径,content是写入的字符串
	public static void writeTxtFile(string fileName, string content)
	{
		// 检测路径是否存在,如果不存在就创建一个
		createDir(StringUtility.getFilePath(fileName));
		StreamWriter writer = new StreamWriter(fileName, false, Encoding.UTF8);
		writer.Write(content);
		writer.Close();
	}
	public static bool renameFile(string fileName, string newName, bool forceCover = false)
	{
		if(!isFileExist(fileName))
		{
			return false;
		}
		if(isFileExist(newName))
		{
			if(forceCover)
			{
				deleteFile(newName);
			}
			else
			{
				return false;
			}
		}
		try
		{
			Directory.Move(fileName, newName);
		}
		catch(Exception e)
		{
			string message = e.Message;
		}
		return true;
	}
	public static void deleteFolder(string path)
	{
		validPath(ref path);
		string[] dirList = Directory.GetDirectories(path);
		// 先删除所有文件夹
		foreach (var item in dirList)
		{
			deleteFolder(item);
		}
		// 再删除所有文件
		string[] fileList = Directory.GetFiles(path);
		foreach(var item in fileList)
		{
			deleteFile(item);
		}
		// 再删除文件夹自身
		Directory.Delete(path);
	}
	public static bool deleteEmptyFolder(string path)
	{
		validPath(ref path);
		// 先删除所有空的文件夹
		string[] dirList = Directory.GetDirectories(path);
		bool isEmpty = true;
		foreach (var item in dirList)
		{
			isEmpty = deleteEmptyFolder(item) && isEmpty;
		}
		isEmpty = isEmpty && Directory.GetFiles(path).Length == 0;
		if(isEmpty)
		{
			Directory.Delete(path);
		}
		return isEmpty;
	}
	public static void copyFile(string source, string dest, bool overwrite = true)
	{
		// 如果目标文件所在的目录不存在,则先创建目录
		string parentDir = StringUtility.getFilePath(dest);
		createDir(parentDir);
		File.Copy(source, dest, overwrite);
	}
	public static long getFileSize(string file)
	{
		try
		{
			FileInfo fileInfo = new FileInfo(file);
			return fileInfo.Length;
		}
		catch
		{
			return 0;
		}
	}
	public static bool isDirExist(string dir)
	{
		return Directory.Exists(dir);
	}
	public static bool isFileExist(string fileName)
	{
		return File.Exists(fileName);
	}
	public static void createDir(string dir)
	{
		if(dir == "" || dir == "./" || dir == "../" || dir == "." || dir == "..")
		{
			return;
		}
		if (isDirExist(dir))
		{
			return;
		}
		// 如果有上一级目录,并且上一级目录不存在,则先创建上一级目录
		string parentDir = StringUtility.getFilePath(dir);
		if (parentDir != dir)
		{
			createDir(parentDir);
		}
		Directory.CreateDirectory(dir);
	}
	public static void findFiles(string path, ref List<string> fileList, List<string> pattern = null, bool recursive = true)
	{
		validPath(ref path);
		if (!isDirExist(path))
		{
			return;
		}
		DirectoryInfo folder = new DirectoryInfo(path);
		FileInfo[] fileInfoList = folder.GetFiles();
		int fileCount = fileInfoList.Length;
		int patternCount = pattern != null ? pattern.Count : 0;
		for (int i = 0; i < fileCount; ++i)
		{
			string fileName = fileInfoList[i].Name;
			// 如果需要过滤后缀名,则判断后缀
			if (patternCount > 0)
			{
				for (int j = 0; j < patternCount; ++j)
				{
					if (StringUtility.endWith(fileName, pattern[j], false))
					{
						fileList.Add(path + fileName);
					}
				}
			}
			// 不需要过滤,则直接放入列表
			else
			{
				fileList.Add(path + fileName);
			}
		}
		// 查找所有子目录
		if(recursive)
		{
			string[] dirs = Directory.GetDirectories(path);
			foreach(var item in dirs)
			{
				findFiles(item, ref fileList, pattern, recursive);
			}
		}
	}
	// 得到指定目录下的所有第一级子目录
	// path为相对于Assets的相对路径
	public static bool findDirectory(string path, ref List<string> dirList, bool recursive = true)
	{
		validPath(ref path);
		if (!isDirExist(path))
		{
			return false;
		}
		string[] dirs = Directory.GetDirectories(path);
		foreach (var item in dirs)
		{
			dirList.Add(item);
			if(recursive)
			{
				findDirectory(item, ref dirList, recursive);
			}
		}
		return true;
	}
	public static bool deleteFile(string path)
	{
		try
		{
			File.Delete(path);
			return true;
		}
		catch
		{
			return false;
		}
	}
	public static string generateFileMD5(string fileName, bool upperOrLower = true)
	{
		FileStream file = new FileStream(fileName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
		HashAlgorithm algorithm = MD5.Create();
		byte[] md5Bytes = algorithm.ComputeHash(file);
		file.Close();
		return BinaryUtility.bytesToHEXString(md5Bytes, false, upperOrLower);
	}
}