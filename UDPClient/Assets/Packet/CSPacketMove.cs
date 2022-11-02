using UnityEngine;
using static BinaryUtility;

public class CSPacketMove : UDPPacket
{
	public int mPlayerID;
	public float mPosX;
	public float mPosY;
	public override void write(byte[] buffer, ref int index)
	{
		base.write(buffer, ref index);
		writeInt(buffer, ref index, mPlayerID);
		writeFloat(buffer, ref index, mPosX);
		writeFloat(buffer, ref index, mPosY);
	}
}
