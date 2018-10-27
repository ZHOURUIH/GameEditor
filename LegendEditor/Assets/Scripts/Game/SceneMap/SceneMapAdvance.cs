using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class SceneMapAdvance : GameBase
{
	public MapHeader mHeader;
	public MapTileAdvance[] mTileList;
	public SceneMapAdvance()
	{
		mHeader = new MapHeader();
	}
	public void readFile(string fileName)
	{
		byte[] fileBuffer = null;
		openFile(fileName, ref fileBuffer);
		if(fileBuffer == null)
		{
			return;
		}
		int offset = 0;
		mHeader.parseHeader(fileBuffer, ref offset);
		int tileCount = mHeader.mWidth * mHeader.mHeight;
		mTileList = new MapTileAdvance[tileCount];
		for(int i = 0; i < tileCount; ++i)
		{
			mTileList[i] = new MapTileAdvance(i);
			// 读文件时只是保存数据,暂时不解析所有地砖
			mTileList[i].preParseTile(fileBuffer, ref offset);
		}
	}
}