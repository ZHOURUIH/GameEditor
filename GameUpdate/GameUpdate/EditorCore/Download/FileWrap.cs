using System;
using System.Collections.Generic;

public class FileWrap : EditorBase
{
	protected Serializer mFileData;
	protected string mFullFilePath;     // 文件下载后的存放路径,默认与mFileName相同
	protected string mFileName;
	protected long mTotalSize;          // 文件总大小
	protected long mCurSize;            // 当前已下载大小,当文件太大时mFileData中的大小不等于文件大小,所以需要使用另外的变量来表示
	protected WRITE_MODE mWriteMode;    // 是否自动将下载数据写入文件,true则有数据就立即写入,false则有数据先保存在内存,下载完毕后再写入文件
	public FileWrap()
	{
		mWriteMode = WRITE_MODE.AUTO_WRITE;
	}
	public void destroy()
	{
		mFileData = null;
		GC.Collect();
	}
	public void startWrite(string fileName = "", string fullPath = null)
	{
		mFileName = fileName;
		if (fullPath != null)
		{
			mFullFilePath = fullPath;
		}
		else
		{
			mFullFilePath = mFileName;
		}
		// 如果之前已经下载过了,则需要获取已经下载的大小
		if (isFileExist(mFullFilePath + GameDefine.TEMP_FILE_EXTENSION))
		{
			mCurSize = getFileSize(mFullFilePath + GameDefine.TEMP_FILE_EXTENSION);
		}
		mFileData = new Serializer();
	}
	// md5不为空表示需要检查文件md5,为空则不检查
	public WRITE_RESULT finishWrite(string md5)
	{
		// 不自动写入的文件,在最后才写入文件
		if (mWriteMode == WRITE_MODE.WRITE_FINISH &&
			!writeFile(mFullFilePath + GameDefine.TEMP_FILE_EXTENSION, mFileData.getBuffer(), mFileData.getDataSize(), true))
		{
			return WRITE_RESULT.WRITE_ERROR;
		}
		// 将临时文件改名为正常文件名
		if (!renameFile(mFullFilePath + GameDefine.TEMP_FILE_EXTENSION, mFullFilePath, true))
		{
			return WRITE_RESULT.RENAME_ERROR;
		}
		if (!string.IsNullOrEmpty(md5) && generateFileMD5(mFullFilePath) != md5)
		{
			// 校验失败,删除文件,重新下载
			deleteFile(mFullFilePath);
			return WRITE_RESULT.MD5_ERROR;
		}
		return WRITE_RESULT.SUCCESS;
	}
	public bool writeFile(byte[] data, int length)
	{
		mCurSize += length;
		mFileData.writeBuffer(data, length);
		// 自动写入的文件在接收到数据就写入临时文件
		if (mWriteMode == WRITE_MODE.AUTO_WRITE && !writeFile(mFullFilePath + GameDefine.TEMP_FILE_EXTENSION, data, length, true))
		{
			return false;
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
		if (mTotalSize != 0 && mTotalSize != size)
		{
			logError("size error!", true);
		}
		mTotalSize = size;
	}
	public void setWriteMode(WRITE_MODE write) { mWriteMode = write; }
	public long getCurSize() { return mCurSize; }
	public byte[] getFileData() { return mFileData?.getBuffer(); }
}