using System;
using System.Text;

public class GameFileInfo : EditorBase
{
	public string mFileName;        // 文件完整路径
	public string mFullPath;		// 如果是本地文件,则是文件的绝对路径
	public string mMD5;				// 文件MD5
	public int mFileSize;           // 文件大小
	public void copyFrom(GameFileInfo info)
	{
		mFileName = info.mFileName;
		mFileSize = info.mFileSize;
		mMD5 = info.mMD5;
	}
	public static GameFileInfo createInfo(string infoString)
	{
		string[] list = split(infoString, "\t");
		if(list == null || list.Length != 3)
		{
			return null;
		}
		GameFileInfo info = new GameFileInfo();
		info.mFileName = list[0];
		info.mFileSize = stringToInt(list[1]);
		info.mMD5 = list[2];
		return info;
	}
	public void toString(StringBuilder builder)
	{
		builder.Append(mFileName);
		builder.Append('\t');
		builder.Append(mFileSize);
		builder.Append('\t');
		builder.Append(mMD5);
	}
}