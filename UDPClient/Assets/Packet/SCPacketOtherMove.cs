using UnityEngine;
using static BinaryUtility;

public class SCPacketOtherMove : UDPPacket
{
	public int mPlayerID;
	public float mPosX;
	public float mPosY;
	public override void read(byte[] buffer, ref int index)
	{
		base.read(buffer, ref index);
		mPlayerID = readInt(buffer, ref index, out _);
		mPosX = readInt(buffer, ref index, out _);
		mPosY = readInt(buffer, ref index, out _);
	}
	public override void execute()
	{
		GameFramework.get().setPlayerPosition(mPlayerID, new Vector3(mPosX, mPosY));
	}
}