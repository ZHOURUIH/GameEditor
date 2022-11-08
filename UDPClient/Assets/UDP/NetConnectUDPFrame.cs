using System;
using static MathUtility;
using static BinaryUtility;
using UnityEngine;

// Frame层默认的UDP连接封装类,应用层可根据实际需求仿照此类封装自己的UDP连接类
public class NetConnectUDPFrame : NetConnectUDP
{
	protected byte[] mBodyBuffer;
	public NetConnectUDPFrame()
	{
		mBodyBuffer = new byte[2048 * 4];
	}
	public override void sendPacket(UDPPacket packet)
	{
		if (mSocket == null)
		{
			return;
		}
		
		int realPacketSize = 0;
		packet.write(mBodyBuffer, ref realPacketSize);
		byte[] packetData = new byte[realPacketSize];
		memcpy(packetData, mBodyBuffer, 0, 0, realPacketSize);
		// 添加到写缓冲中
		mOutputBuffer.add(packetData);
	}
	//------------------------------------------------------------------------------------------------------------------------------
	protected override UDPPacket preParsePacket(byte[] buffer, int size, ref int index)
	{
		UDPPacket packet = UDPClient.createPacket((PACKET_TPYE)buffer[0]);
		packet.read(buffer, ref index);
		return packet;
	}
}