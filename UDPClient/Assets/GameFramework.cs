using System;
using System.Collections.Generic;
using UnityEngine;

public class GameFramework : MonoBehaviour
{
	protected int mMyselfID;
	protected Dictionary<int, GameObject> mPlayerList = new Dictionary<int, GameObject>();
	protected GameObject mMyselfPlayer;
	protected UDPClient mUDPClient;
	protected NetConnectUDPFrame mUDPConnect;
	bool mUseMultiThread = false;
	protected CommandSystem mCommandSystem;
	protected static GameFramework mInstance;
	public GameObject mPlayerPrefab;
	protected DateTime lastTime;
	void Start()
    {
		mCommandSystem = new CommandSystem();
		Application.targetFrameRate = 60;
		mInstance = this;
		mMyselfID = UnityEngine.Random.Range(10, 100000);
		mMyselfPlayer = createPlayer(mMyselfID);
		mPlayerList.Add(mMyselfID, mMyselfPlayer);
		if (mUseMultiThread)
		{
			mUDPConnect = new NetConnectUDPFrame();
			mUDPConnect.init("192.168.1.5", 11112);
			mUDPConnect.sendPacket(UDPClient.createPacket(PACKET_TPYE.CS_INIT));
		}
		else
		{
			mUDPClient = new UDPClient();
			mUDPClient.connect("192.168.1.5", 11112);
			mUDPClient.sendPacket(UDPClient.createPacket(PACKET_TPYE.CS_INIT));
		}
	}
	void OnDestroy()
	{
		if (mUseMultiThread)
		{
			mUDPConnect.destroy();
		}
		else
		{
			mUDPClient.close();
		}
	}
	void Update()
    {
		Vector3 direction = Vector3.zero;
        if (Input.GetKey(KeyCode.LeftArrow))
		{
			direction += Vector3.left;
		}
		if (Input.GetKey(KeyCode.RightArrow))
		{
			direction += Vector3.right;
		}
		if (Input.GetKey(KeyCode.UpArrow))
		{
			direction += Vector3.up;
		}
		if (Input.GetKey(KeyCode.DownArrow))
		{
			direction += Vector3.down;
		}
		if (direction.magnitude > 0.0f)
		{
			Vector3 pos = mMyselfPlayer.transform.localPosition + Vector3.Normalize(direction) * Time.deltaTime * 500;
			mMyselfPlayer.transform.localPosition = pos;
			// 通知服务器位置改变
			var packet = UDPClient.createPacket(PACKET_TPYE.CS_MOVE) as CSPacketMove;
			packet.mPlayerID = mMyselfID;
			packet.mPosX = pos.x;
			packet.mPosY = pos.y;
			if (mUseMultiThread)
			{
				mUDPConnect.sendPacket(packet);
			}
			else
			{
				mUDPClient.sendPacket(packet);
			}
		}
		if (mUseMultiThread)
		{
			mUDPConnect.update(Time.deltaTime);
		}	
		else
		{
			mUDPClient.update();
		}
		mCommandSystem.update(Time.deltaTime);
	}
	public static GameFramework get() { return mInstance; }
	public void setPlayerPosition(int playerID, Vector3 pos)
	{
		if (!mPlayerList.TryGetValue(playerID, out GameObject player))
		{
			player = createPlayer(playerID);
			mPlayerList.Add(playerID, player);
		}
		player.transform.localPosition = pos;
		Debug.Log("时间差:" + (DateTime.Now - lastTime).TotalMilliseconds);
		lastTime = DateTime.Now;
	}
	//------------------------------------------------------------------------------------------------------------------------------
	protected GameObject createPlayer(int playerID)
	{
		GameObject player = Instantiate(mPlayerPrefab);
		player.SetActive(true);
		player.transform.parent = mPlayerPrefab.transform.parent;
		player.name = playerID.ToString();
		return player;
	}
}