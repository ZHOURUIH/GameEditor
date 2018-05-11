using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NetFwTypeLib;
using System.IO;
using System.Diagnostics;

public class EditorCore
{
	protected Dictionary<string, string> mProgramPathList;
	public EditorCore()
	{
		mProgramPathList = new Dictionary<string, string>();
	}
	public void init(string path)
	{
		string thisProgramName = Process.GetCurrentProcess().MainModule.FileName;
		thisProgramName = StringUtility.getFileName(thisProgramName);
		string[] files = Directory.GetFiles(path);
		int count = files.Length;
		for (int i = 0; i < count; ++i)
		{
			string fileName = StringUtility.getFileName(files[i]);
			if (fileName.EndsWith(".exe") && fileName != thisProgramName)
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