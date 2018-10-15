using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class MapHeader : GameBase
{
	public static int TITLE_LENGTH = 17;
	public static int UPDATE_TIME_LENGTH = 8;
	public static int RESERVED_LENGTH = 23;
	public short mWidth;
	public short mHeight;
	public byte[] mTitle;
	public byte[] mUpdateTime;
	public byte[] mReserved;
	public MapHeader()
	{
		mTitle =new byte[TITLE_LENGTH];
		mUpdateTime = new byte[UPDATE_TIME_LENGTH];
		mReserved = new byte[RESERVED_LENGTH];
	}
	public void parseHeader(byte[] buffer, ref int offset)
	{
		Serializer serializer = new Serializer(buffer);
		serializer.setIndex(offset);
		serializer.read(ref mWidth);
		serializer.read(ref mHeight);
		serializer.readBuffer(mTitle, TITLE_LENGTH);
		serializer.readBuffer(mUpdateTime, UPDATE_TIME_LENGTH);
		serializer.readBuffer(mReserved, RESERVED_LENGTH);
		offset = serializer.getIndex();
	}
	public int getHeaderLength()
	{
		return sizeof(short) + sizeof(short) + TITLE_LENGTH + UPDATE_TIME_LENGTH + RESERVED_LENGTH;
	}
}