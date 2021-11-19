using System.Collections.Generic;
using System.IO;
using System.Xml;

public class GameUtility : EditorBase
{
	public static Dictionary<string, GameFileInfo> getFileList(string path, bool removePrefix)
	{
		var paramList = new Dictionary<string, string>();
		var fileList = new List<GameFileInfo>();
		string marker = null;
		string str;
		do
		{
			paramList.Clear();
			if (!isEmpty(marker))
			{
				paramList.Add("marker", marker);
			}
			paramList.Add("prefix", path);
			str = HttpDownloadManager.get(GameDefine.REMOTE_URL, paramList, null);
			if (str == null)
			{
				return null;
			}
		}
		while (!parseFileList(str, fileList, out marker));
		var fileMap = new Dictionary<string, GameFileInfo>();
		for(int i = 0; i < fileList.Count; ++i)
		{
			if (removePrefix)
			{
				fileList[i].mFileName = fileList[i].mFileName.Substring(path.Length);
			}
			fileMap.Add(fileList[i].mFileName, fileList[i]);
		}
		return fileMap;
	}
	//------------------------------------------------------------------------------------------------------------------------------
	// 返回值表示是否已经获取了全部的文件信息,如果没有获取全,nextMarker则会返回下一次获取所需的标记
	protected static bool parseFileList(string str, List<GameFileInfo> fileList, out string nextMarker)
	{
		bool fetchFinish = false;
		nextMarker = null;
		var strReader = new StringReader(str);
		XmlReader reader = XmlReader.Create(strReader);
		while (reader.Read())
		{
			if (reader.NodeType != XmlNodeType.Element)
			{
				continue;
			}
			if (reader.Name == "Contents")
			{
				GameFileInfo info = new GameFileInfo();
				while (reader.Read())
				{
					if (reader.NodeType != XmlNodeType.Element)
					{
						continue;
					}
					string name = reader.Name;
					reader.Read();
					if (name == "Key")
					{
						info.mFileName = reader.Value;
						// 以/结尾的是目录,不需要放入列表
						if (reader.Value[reader.Value.Length - 1] == '/')
						{
							break;
						}
					}
					else if (name == "ETag")
					{
						info.mMD5 = removeAll(reader.Value, '\"');
					}
					else if (name == "Size")
					{
						info.mFileSize = stringToInt(reader.Value);
						// 完成一个文件信息的解析
						fileList.Add(info);
						break;
					}
				}
			}
			else if(reader.Name == "IsTruncated")
			{
				reader.Read();
				fetchFinish = !stringToBool(reader.Value);
			}
			else if(reader.Name == "NextMarker")
			{
				reader.Read();
				nextMarker = reader.Value;
			}
		}
		reader.Close();
		strReader.Close();
		return fetchFinish;
	}
}