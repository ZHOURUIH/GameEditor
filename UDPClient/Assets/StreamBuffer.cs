﻿using System;
using static BinaryUtility;

// 自定义缓冲区,用于持续写入数据
public class StreamBuffer
{
	protected byte[] mBuffer;		// 缓冲区
	protected int mBufferSize;		// 缓冲区大小
	protected int mDataLength;		// 当前数据大小
	public StreamBuffer(){}
	public StreamBuffer(int bufferSize)
	{
		init(bufferSize);
	}
	public void init(int bufferSize)
	{
		resizeBuffer(bufferSize);
	}
	public byte[] getData(){return mBuffer;}
	public int getDataLength(){return mDataLength;}
	public void merge(StreamBuffer stream)
	{
		addData(stream.getData(), stream.getDataLength());
	}
	public void addData(byte[] data, int count)
	{
		// 缓冲区足够放下数据时才处理
		if (count <= mBufferSize - mDataLength)
		{
			memcpy(mBuffer, data, mDataLength, 0, count);
			mDataLength += count;
		}
	}
	public bool addData(byte[] data, int offer, int count)
	{
		// 缓冲区足够放下数据时才处理
		if (count > mBufferSize - mDataLength)
		{
			return false;
		}
		memcpy(mBuffer, data, mDataLength, offer, count);
		mDataLength += count;
		return true;
	}
	public void removeData(int start, int count)
	{
		if (mDataLength >= start + count)
		{
			memmove(ref mBuffer, start, start + count, mDataLength - start - count);
			mDataLength -= count;
		}
	}
	public void clear()
	{
		mDataLength = 0;
	}
	//------------------------------------------------------------------------------------------------------------------------------
	protected void resizeBuffer(int size)
	{
		if (mBufferSize >= size)
		{
			return;
		}
		mBufferSize = size;
		if (mBuffer != null)
		{
			// 创建新的缓冲区,将原来的数据拷贝到新缓冲区中,销毁原缓冲区,指向新缓冲区
			byte[] newBuffer = new byte[mBufferSize];
			if (mDataLength > 0)
			{
				memcpy(newBuffer, mBuffer, 0, 0, mDataLength);
			}
			mBuffer = newBuffer;
		}
		else
		{
			mBuffer = new byte[mBufferSize];
		}
	}
}