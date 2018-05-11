using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NetFwTypeLib;
using System.Windows.Forms;
using System.IO;

public class EditorCore
{
	protected Dictionary<string, string> mProgramPathList;
	public EditorCore()
	{
		mProgramPathList = new Dictionary<string, string>();
	}
	public void init()
	{
		// 查找../../../目录下的所有exe
		string path = StringUtility.getFilePath(Application.ExecutablePath);
		path = StringUtility.getFilePath(path);
		path = StringUtility.getFilePath(path);
		path = StringUtility.getFilePath(path);
		string[] files = Directory.GetFiles(path);
		int count = files.Length;
		for (int i = 0; i < count; ++i)
		{
			string fileName = StringUtility.getFileName(files[i]);
			if (fileName.EndsWith(".exe"))
			{
				mProgramPathList.Add(fileName, files[i]);
			}
		}
	}
	public void destroy()
	{
		;
	}
	public void update(float elapsedTime)
	{
		;
	}
	public Dictionary<string, string> getProgramPathList() { return mProgramPathList; }
	public string getProgramPath(string file)
	{
		if(!mProgramPathList.ContainsKey(file))
		{
			return "";
		}
		return mProgramPathList[file];
	}
}