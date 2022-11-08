using System.Net;
using System.Net.Sockets;
using UnityEngine;

public class UDPClient
{
	protected Socket mSocket;
	protected IPEndPoint mServerAddress;
	protected EndPoint mRecvAddress;
	protected byte[] mSendBuffer = new byte[2048];
	protected byte[] mRecvBuffer = new byte[2048];
	public void connect(string serverIP, int serverPort)
	{
		mSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
		mServerAddress = new IPEndPoint(IPAddress.Parse(serverIP), serverPort);
		mRecvAddress = new IPEndPoint(IPAddress.Parse(serverIP), serverPort);
	}
	public void close()
	{
		if (mSocket != null)
		{
			mSocket.Close();
			mSocket.Dispose();
			mSocket = null;
		}
	}
	public void update()
	{
		if (mSocket != null && mSocket.Available > 0)
		{
			int recvCount = mSocket.ReceiveFrom(mRecvBuffer, ref mRecvAddress);
			if (recvCount > 0)
			{
				recvPacket(mRecvBuffer, recvCount);
			}
		}
	}
	public void sendPacket(UDPPacket packet)
	{
		int packetSize = 0;
		packet.write(mSendBuffer, ref packetSize);
		mSocket.SendTo(mSendBuffer, 0, packetSize, SocketFlags.None, mServerAddress);
	}
	protected void recvPacket(byte[] buffer, int recvCount)
	{
		UDPPacket packet = createPacket((PACKET_TPYE)buffer[0]);
		int index = 0;
		packet.read(buffer, ref index);
		if (index > recvCount)
		{
			Debug.LogError("接收数据错误");
			return;
		}
		packet.execute();
	}
	public static UDPPacket createPacket(PACKET_TPYE type)
	{
		UDPPacket packet = null;
		switch (type)
		{
			case PACKET_TPYE.CS_INIT: packet = new CSInit(); break;
			case PACKET_TPYE.CS_MOVE: packet = new CSPacketMove(); break;
			case PACKET_TPYE.SC_OTHER_MOVE: packet = new SCPacketOtherMove(); break;
		}
		if (packet != null)
		{
			packet.setType(type);
		}
		return packet;
	}
}
