using UnityEngine;
using System.Collections;

// 管理类初始化完成调用
// 这个父类的添加是方便代码的书写
public class GameBase : FrameBase
{
	public static Game mGame;
	public static GameConfig mGameConfig;
	public static LogSystem mLogSystem;
	public static DataCenter mDataCenter;
	public static Effect2DManager mEffect2DManager;
	public static SceneCamera mSceneCamera;
	public static MapManager mMapManager;
	// SQLiteTable
	public static SQLiteSound mSQLiteSound;
	public static SQLiteMonster mSQLiteMonster;
	public static SQLiteSceneMap mSQLiteSceneMap;
	public static SQLiteLog mSQLiteLog;
	public static SQLiteNPC mSQLiteNPC;
	public static SQLiteImagePositionAnimation mSQLiteImagePositionAnimation;
	public static SQLiteImagePositionIcon mSQLiteImagePositionIcon;
	public static SQLiteImagePositionEffect mSQLiteImagePositionEffect;
	public static SQLiteMonsterSkill mSQLiteMonsterSkill;
	public static SQLitePlayerSkill mSQLitePlayerSkill;
	public static SQLiteSkillBullet mSQLiteSkillBullet;
	public static SQLiteSkillEffectDirection mSQLiteSkillEffectDirection;
	public static SQLiteItemBox mSQLiteItemBox;
	public static SQLiteItemEquip mSQLiteItemEquip;
	public static SQLiteItemConsumable mSQLiteItemConsumable;
	public static SQLiteItemMaterial mSQLiteItemMaterial;
	public static SQLiteItemSkillBook mSQLiteItemSkillBook;
	public static SQLiteBuff mSQLiteBuff;
	public static SQLiteBuffDetail mSQLiteBuffDetail;
	public static SQLiteMonsterTemplate mSQLiteMonsterTemplate;
	public static SQLiteMapTileIndex mSQLiteMapTileIndex;
	public static SQLiteAnimation mSQLiteAnimation;
	public static SQLiteSceneMapTransfer mSQLiteSceneMapTransfer;
	public static SQLiteMapEffectPosition mSQLiteMapEffectPosition;
	// 以下是用于快速访问的布局脚本
	public static ScriptScene mScriptScene;
	public override void notifyConstructDone()
	{
		base.notifyConstructDone();
		if (mGame == null)
		{
			mGame = mGameFramework as Game;
			mGame.getSystem(out mGameConfig);
			mGame.getSystem(out mLogSystem);
			mGame.getSystem(out mDataCenter);
			mGame.getSystem(out mEffect2DManager);
			mGame.getSystem(out mSceneCamera);
			mGame.getSystem(out mMapManager);
		}
	}
}
