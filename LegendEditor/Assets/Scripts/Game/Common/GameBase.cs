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
	public static MonsterManager mMonsterManager;
	// SQLiteTable
	public static SQLiteCloth mSQLiteCloth;
	public static SQLiteClothFrame mSQLiteClothFrame;
	public static SQLiteWeapon mSQLiteWeapon;
	public static SQLiteWeaponFrame mSQLiteWeaponFrame;
	public static SQLiteMonster mSQLiteMonster;
	public static SQLiteMonsterFrame mSQLiteMonsterFrame;
	public static SQLiteEffect mSQLiteEffect;
	public static SQLiteEffectFrame mSQLiteEffectFrame;
	// 以下是用于快速访问的布局脚本
	//public static ScriptObjectEditor mScriptObjectEditor;
	public static ScriptSceneEditor mScriptSceneEditor;
	public static ScriptSceneAdvanceEditor mScriptSceneAdvanceEditor;
	public static ScriptEditorSelect mScriptEditorSelect;
	public static ScriptEffectTextureEditor mScriptEffectTextureEditor;
	public static ScriptEffectSpriteEditor mScriptEffectSpriteEditor;
	public static ScriptMonsterSpriteEditor mScriptMonsterSpriteEditor;
	public static ScriptHumanSpriteEditor mScriptHumanSpriteEditor;
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
			mMonsterManager = mGame.getSystem<MonsterManager>();
		}
	}
}
