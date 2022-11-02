using UnityEngine;
using static BinaryUtility;

public class UDPPacket
{
	protected PACKET_TPYE mType;
	public void setType(PACKET_TPYE type) { mType = type; }
	public PACKET_TPYE getType() { return mType; }
	public virtual void read(byte[] buffer, ref int index)
	{
		mType = (PACKET_TPYE)readByte(buffer, ref index, out _);
	}
	public virtual void write(byte[] buffer, ref int index)
	{
		writeByte(buffer, ref index, (byte)mType);
	}
	public virtual void execute() { }
}
