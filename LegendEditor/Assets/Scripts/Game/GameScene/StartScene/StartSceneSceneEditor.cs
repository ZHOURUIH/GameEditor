using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class StartSceneSceneEditor : SceneProcedure
{
	public StartSceneSceneEditor(GameScene gameScene)
		:base(gameScene){}
	protected override void onInit(SceneProcedure lastProcedure, string intent)
	{
		LT.LOAD_UGUI_SHOW(LAYOUT.L_SCENE, 0);
		MapData mapData = mMapManager.createMap(88);
		Vector3 defaultPos = mSceneCamera.getDefaultPosition();
		int centerTileX = mapData.mHeader.mWidth / 2;
		int centerTileY = mapData.mHeader.mHeight / 2;
		Vector2 centerPixelPos = GameUtility.tilePosToPixelPos(new Vector2Int(centerTileX, centerTileY), mapData.mHeader.mHeight);
		mSceneCamera.setPosition(new Vector3(centerPixelPos.x, centerPixelPos.y, defaultPos.z));
		mScriptScene.showSceneMap(mapData, mSceneCamera.getPosition());
	}
	protected override void onUpdate(float elapsedTime)
	{
		;
	}
	protected override void onExit(SceneProcedure nextProcedure)
	{
		LT.UNLOAD_LAYOUT(LAYOUT.L_SCENE);
	}
	protected override void onKeyProcess(float elapsedTime)
	{
		;
	}
}