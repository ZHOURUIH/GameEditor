using UnityEngine;
using System.Collections;

// 管理类初始化完成调用
// 这个父类的添加是方便代码的书写
public class GameBase : FrameBase
{
	public static Game mGame;
	public static GameConfig mGameConfig;
	public static HeadTextureManager mHeadTextureManager;
	public static LogSystem mLogSystem;
	public static DataCenter mDataCenter;
	public static SQLite mSQLite;
	public static MonsterManager mMonsterManager;
	// 以下是用于快速访问的布局脚本
	public static ScriptScene mScriptScene;
	public override void notifyConstructDone()
	{
		base.notifyConstructDone();
		if (mGame == null)
		{
			mGame = Game.instance as Game;
			mGameConfig = mGame.getSystem<GameConfig>();
			mHeadTextureManager = mGame.getSystem<HeadTextureManager>();
			mLogSystem = mGame.getSystem<LogSystem>();
			mDataCenter = mGame.getSystem<DataCenter>();
			mSQLite = mGame.getSystem<SQLite>();
			mMonsterManager = mGame.getSystem<MonsterManager>();
		}
	}
}
