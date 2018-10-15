using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class SceneMap : GameBase
{
	public MapHeader mHeader;
	public MapTile[] mTileList;
	public SceneMap()
	{
		mHeader = new MapHeader();
	}
	public void readFile(string fileName)
	{
		byte[] fileBuffer = null;
		FileUtility.openFile(fileName, ref fileBuffer);
		if(fileBuffer == null)
		{
			return;
		}
		int offset = 0;
		mHeader.parseHeader(fileBuffer, ref offset);
		int tileCount = mHeader.mWidth * mHeader.mHeight;
		mTileList = new MapTile[tileCount];
		for(int i = 0; i < tileCount; ++i)
		{
			mTileList[i] = new MapTile(i);
			mTileList[i].parseTile(fileBuffer, ref offset);
		}
	}
}