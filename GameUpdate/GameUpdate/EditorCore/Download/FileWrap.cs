using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public enum WRITE_MODE
{
	WM_AUTO_WRITE,	// 接收到下载数据时就自动写入文件
	WM_WRITE_FINISH,// 只有在下载完毕时才写入文件
	WM_DONT_WRITE,	// 只保存在内存,不写入文件
}

public class FileWrap : EditorBase
{
	protected string mFileName;
	protected Serializer mFileData;
	protected long mTotalSize; // 文件总大小
	protected long mCurSize;   // 当前已下载大小,当文件太大时mFileData中的大小不等于文件大小,所以需要使用另外的变量来表示
	protected WRITE_MODE mWriteMode;	// 是否自动将下载数据写入文件,true则有数据就立即写入,false则有数据先保存在内存,下载完毕后再写入文件
	protected string mFullFilePath;	// 文件下载后的存放路径,默认与mFileName相同
	public FileWrap()
	{
		mFileData = null;
		mTotalSize = 0;
		mCurSize = 0;
		mWriteMode = WRITE_MODE.WM_AUTO_WRITE;
	}
	public void destroy()
	{
		mFileData = null;
		GC.Collect();
	}
	public void startWrite(string fileName = "", string fullPath = "")
	{
		mFileName = fileName;
		if(fullPath != "")
		{
			mFullFilePath = fullPath;
		}
		else
		{
			mFullFilePath = mFileName;
		}
		// 如果之前已经下载过了,则需要获取已经下载的大小
		if (FileUtility.isFileExist(mFullFilePath + CommonDefine.TEMP_FILE_EXTENSION))
		{
			mCurSize = FileUtility.getFileSize(mFullFilePath + CommonDefine.TEMP_FILE_EXTENSION);
		}
		mFileData = new Serializer();
	}
	// md5不为空表示需要检查文件md5,为空则不检查,返回0表示成功,1表示md5校验失败,2表示重命名失败,3表示写入文件失败
	public int finishWrite(string md5)
	{
		// 不自动写入的文件,在最后才写入文件
		if (mWriteMode == WRITE_MODE.WM_WRITE_FINISH)
		{
			bool ret = FileUtility.writeFile(mFullFilePath + CommonDefine.TEMP_FILE_EXTENSION, mFileData.getBuffer(), mFileData.getDataSize(), true);
			if(!ret)
			{
				return 3;
			}
		}
		// 将临时文件改名为正常文件名
		if (!FileUtility.renameFile(mFullFilePath + CommonDefine.TEMP_FILE_EXTENSION, mFullFilePath, true))
		{
			return 2;
		}
		if (md5 == null || md5 == "")
		{
			return 0;
		}
		string fileMD5 = FileUtility.generateFileMD5(mFullFilePath);
		if (fileMD5 != md5)
		{
			// 校验失败,删除文件,重新下载
			FileUtility.deleteFile(mFullFilePath);
			return 1;
		}
		return 0;
	}
	public bool writeFile(byte[] data, int length)
	{
		mCurSize += length;
		mFileData.writeBuffer(data, length);
		// 自动写入的文件在接收到数据就写入临时文件
		if(mWriteMode == WRITE_MODE.WM_AUTO_WRITE)
		{
			bool ret = FileUtility.writeFile(mFullFilePath + CommonDefine.TEMP_FILE_EXTENSION, data, length, true);
			if(!ret)
			{
				return false;
			}
		}
		// 如果缓冲区已满,则需要重新创建缓冲区
		if (mFileData.getDataSize() >= CommonDefine.BIG_FILE_SIZE)
		{	
			mFileData = null;
			mFileData = new Serializer();
			GC.Collect();
		}
		return true;
	}
	public string getFileName() { return mFileName; }
	public string getFullFilePath() { return mFullFilePath; }
	public long getTotalSize() { return mTotalSize; }
	public void setTotalSize(long size)
	{
		if (size <= 0)
		{
			return;
		}
		if(mTotalSize != 0 && mTotalSize != size)
		{
			logError("size error!", true);
		}
		mTotalSize = size;
	}
	public void setWriteMode(WRITE_MODE write)
	{
		mWriteMode = write;
	}
	public long getCurSize() { return mCurSize; }
	public byte[] getFileData()
	{
		if (mFileData == null)
		{
			return null;
		}
		return mFileData.getBuffer();
	}
}