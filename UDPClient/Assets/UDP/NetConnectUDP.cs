using System;
using System.Net;
using System.Net.Sockets;
using System.Collections.Generic;
using UnityEngine;

// 当前程序作为客户端时使用,表示一个与UDP服务器的连接
public abstract class NetConnectUDP : NetConnect
{
	protected DoubleBuffer<UDPPacket> mReceiveBuffer;// 在主线程中执行的消息列表
	protected DoubleBuffer<byte[]> mOutputBuffer;           // 使用双缓冲提高发送消息的效率
	protected Queue<string> mReceivePacketHistory;          // 接收过的包的缓冲列表
	protected StreamBuffer mInputBuffer;                    // 接收消息的缓冲区
	protected ThreadLock mOutputBufferLock;                 // mOutputBuffer的锁
	protected ThreadLock mSocketLock;                       // mSocket的锁
	protected IPEndPoint mSendEndPoint;                     // 发送的目标地址
	protected EndPoint mRecvEndPoint;						// 接收时的地址
	protected MyThread mReceiveThread;                      // 接收线程
	protected MyThread mSendThread;                         // 发送线程
	protected Socket mSocket;                               // 套接字实例
	protected byte[] mRecvBuff;                             // 从Socket接收时使用的缓冲区
	protected int mMinPacketSize;                           // 解析一个消息包所需的最少字节数
	public NetConnectUDP()
	{
		mReceiveBuffer = new DoubleBuffer<UDPPacket>();
		mOutputBuffer = new DoubleBuffer<byte[]>();
		mReceiveThread = new MyThread("SocketReceiveUDP");
		mSendThread = new MyThread("SocketSendUDP");
		mRecvBuff = new byte[2048];
		mInputBuffer = new StreamBuffer();
		mOutputBufferLock = new ThreadLock();
		mSocketLock = new ThreadLock();
		mReceivePacketHistory = new Queue<string>();
		mMinPacketSize = 1;
	}
	public virtual void init(string targetIP, int targetPort)
	{
		mSendThread.start(sendSocket);
		mReceiveThread.start(receiveSocket, 0, 1);
		mInputBuffer.init(2048);
		if (!string.IsNullOrEmpty(targetIP) && targetPort != 0)
		{
			mSendEndPoint = new IPEndPoint(IPAddress.Parse(targetIP), targetPort);
			mRecvEndPoint = new IPEndPoint(IPAddress.Parse(targetIP), targetPort);
		}
		mSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
	}
	public virtual void update(float elapsedTime)
	{
		// 解析所有已经收到的消息包
		var readList = mReceiveBuffer.get();
		if (readList != null)
		{
			try
			{
				int count = readList.Count;
				for (int i = 0; i < count; ++i)
				{
					readList[i].execute();
				}
			}
			catch (Exception e)
			{
				Debug.LogError("socket packet error");
			}
			readList.Clear();
		}
		mReceiveBuffer.endGet();
	}
	public override void destroy()
	{
		base.destroy();
		clearSocket();
		mSendThread.destroy();
		mReceiveThread.destroy();
	}
	public abstract void sendPacket(UDPPacket packet);
	public void clearSocket()
	{
		mSocketLock.waitForUnlock();
		try
		{
			if (mSocket != null)
			{
				mSocket.Close();
				mSocket.Dispose();
				mSocket = null;
			}
		}
		catch (Exception e)
		{
			mSocket = null;
		}
		mSocketLock.unlock();
	}
	public void clearBuffer()
	{
		// 将消息列表中残留的消息清空,双缓冲中的读写列表都要清空
		mOutputBufferLock.waitForUnlock();
		mOutputBuffer.clear();
		mOutputBufferLock.unlock();
		mInputBuffer.clear();
	}
	//------------------------------------------------------------------------------------------------------------------------------
	// 发送Socket消息
	protected bool sendSocket()
	{
		if (mSocket == null)
		{
			return true;
		}
		// 获取输出数据的读缓冲区,手动拼接到大的缓冲区中
		mOutputBufferLock.waitForUnlock();
		var readList = mOutputBuffer.get();
		if (readList != null)
		{
			try
			{
				int count = readList.Count;
				for (int i = 0; i < count; ++i)
				{
					byte[] item = readList[i];
					if (item == null)
					{
						continue;
					}
					// dataLength表示item的有效数据长度,包含dataLength本身占的4个字节
					if (mSocket == null)
					{
						return true;
					}
					mSocket.SendTo(item, 0, item.Length, SocketFlags.None, mSendEndPoint);
				}
			}
			catch (SocketException) { }
			readList.Clear();
		}
		mOutputBuffer.endGet();
		mOutputBufferLock.unlock();
		return true;
	}
	// 接收Socket消息
	protected bool receiveSocket()
	{
		if (mSocket == null)
		{
			return true;
		}
		try
		{
			// 在Receive之前先判断SocketBuffer中有没有数据可以读,因为如果不判断直接调用的话,可能会出现即使SocketBuffer中有数据,
			// Receive仍然获取不到的问题,具体原因未知,且出现几率也比较小,但是仍然可能会出现.所以先判断再Receive就不会出现这个问题
			while (mSocket == null || mSocket.Available == 0)
			{
				return true;
			}
			int nRecv = mSocket.ReceiveFrom(mRecvBuff, ref mRecvEndPoint);
			if (nRecv <= 0)
			{
				// 服务器异常
				return true;
			}
			mInputBuffer.addData(mRecvBuff, nRecv);
			// 解析接收到的数据
			while (parseInputBuffer()) { }
		}
		catch(SocketException){ }
		return true;
	}
	protected abstract UDPPacket preParsePacket(byte[] buffer, int size, ref int index);
	protected bool parseInputBuffer()
	{
		// 可能还没有接收完全,等待下次接收
		if (mInputBuffer.getDataLength() < mMinPacketSize)
		{
			return false;
		}
		int index = 0;
		mReceiveBuffer.add(preParsePacket(mInputBuffer.getData(), mInputBuffer.getDataLength(), ref index));
		mInputBuffer.removeData(0, index);
		return true;
	}
}