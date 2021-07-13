using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class GameUtility : GameBase
{
	private static Dictionary<int, TDMonster> mQueriedMonsterData = new Dictionary<int, TDMonster>();
	private static Dictionary<int, TDMonsterTemplate> mQueriedMonsterTemplateData = new Dictionary<int, TDMonsterTemplate>();
	private static Dictionary<int, TDNPC> mQueriedNPCData = new Dictionary<int, TDNPC>();
	private static Dictionary<string, TDImagePositionEffect> mQueriedImagePositionEffect = new Dictionary<string, TDImagePositionEffect>();
	private static Dictionary<string, List<Vector2>> mQueriedImagePositionEffectPosList = new Dictionary<string, List<Vector2>>();
	private static Dictionary<string, Dictionary<string, TDImagePositionAnimation>> mQueriedImagePositionAnimation = new Dictionary<string, Dictionary<string, TDImagePositionAnimation>>();
	private static Dictionary<string, Dictionary<string, List<Vector2>>> mQueriedImagePositionAnimationPosList = new Dictionary<string, Dictionary<string, List<Vector2>>>();
	private static Dictionary<string, Dictionary<string, TDImagePositionIcon>> mQueriedImagePositionIcon = new Dictionary<string, Dictionary<string, TDImagePositionIcon>>();
	private static Dictionary<string, TDSkillEffectDirection> mQueriedSkillEffectDirectionCount = new Dictionary<string, TDSkillEffectDirection>();
	private static Dictionary<int, TDMonsterSkill> mQueriedMonsterSkill = new Dictionary<int, TDMonsterSkill>();
	private static Dictionary<int, TDPlayerSkill> mQueriedPlayerSkill = new Dictionary<int, TDPlayerSkill>();
	private static Dictionary<int, TDSkillBullet> mQueriedSkillBullet = new Dictionary<int, TDSkillBullet>();
	private static Dictionary<uint, TDItemBox> mQueriedItemBox = new Dictionary<uint, TDItemBox>();
	private static Dictionary<uint, TDItemEquip> mQueriedItemEquip = new Dictionary<uint, TDItemEquip>();
	private static Dictionary<uint, TDItemConsumable> mQueriedItemConsumable = new Dictionary<uint, TDItemConsumable>();
	private static Dictionary<uint, TDItemMaterial> mQueriedItemMaterial = new Dictionary<uint, TDItemMaterial>();
	private static Dictionary<uint, TDItemSkillBook> mQueriedItemSkillBook = new Dictionary<uint, TDItemSkillBook>();
	private static Dictionary<int, List<int>> mObjectTileSetIndexList = new Dictionary<int, List<int>>();// key是FileIndex,value每个图集的终止图片下标
	private static Dictionary<int, TDAnimation> mAnimationList = new Dictionary<int, TDAnimation>();
	private static Dictionary<int, TDBuff> mBuffList = new Dictionary<int, TDBuff>();
	private static Dictionary<int, TDBuffDetail> mBuffDetailList = new Dictionary<int, TDBuffDetail>();
	private static Dictionary<int, TDMapEffectPosition> mMapEffectPositionList = new Dictionary<int, TDMapEffectPosition>();
	private static List<int> mMapTileSetIndexList = new List<int>();
	private static List<float> mDirectionAngle16 = new List<float>();
	private static List<float> mDirectionAngle32 = new List<float>();
	private static List<Vector3> mDirectionVector8 = new List<Vector3>();
	private static List<Vector3> mDirectionVector16 = new List<Vector3>();
	private static List<Vector3> mDirectionVector32 = new List<Vector3>();
	public static int getObjectTileSetIndex(int fileIndex, int textureIndex)
	{
		// key是FileIndex,value每个图集的终止图片下标
		if (mObjectTileSetIndexList.Count == 0)
		{
			Dictionary<int, SortedDictionary<int, TDMapTileIndex>> tempList = new Dictionary<int, SortedDictionary<int, TDMapTileIndex>>();
			List<TDMapTileIndex> allList;
			mSQLiteMapTileIndex.queryAll(out allList);
			int count = allList.Count;
			for (int i = 0; i < count; ++i)
			{
				int file = allList[i].mFileIndex;
				if (!tempList.ContainsKey(file))
				{
					tempList.Add(file, new SortedDictionary<int, TDMapTileIndex>());
				}
				tempList[file].Add(allList[i].mSetIndex, allList[i]);
			}
			foreach (var item in tempList)
			{
				mObjectTileSetIndexList.Add(item.Key, new List<int>());
				int endIndex = 0;
				foreach (var item0 in item.Value)
				{
					endIndex += item0.Value.mTextureCount;
					mObjectTileSetIndexList[item.Key].Add(endIndex - 1);
				}
			}
		}
		if (mObjectTileSetIndexList.ContainsKey(fileIndex))
		{
			var list = mObjectTileSetIndexList[fileIndex];
			int count = list.Count;
			for (int i = 0; i < count; ++i)
			{
				if (textureIndex <= list[i])
				{
					return i;
				}
			}
		}
		return -1;
	}
	public static int getMapTileSetIndex(int textureIndex)
	{
		if (mMapTileSetIndexList.Count == 0)
		{
			SortedDictionary<int, TDMapTileIndex> tempList = new SortedDictionary<int, TDMapTileIndex>();
			List<TDMapTileIndex> allList;
			mSQLiteMapTileIndex.queryAll(out allList);
			int count = allList.Count;
			for (int i = 0; i < count; ++i)
			{
				int file = allList[i].mFileIndex;
				// -1表示是大地砖的下标
				if (file == -1)
				{
					tempList.Add(allList[i].mSetIndex, allList[i]);
				}
			}
			int endIndex = 0;
			foreach (var item in tempList)
			{
				endIndex += item.Value.mTextureCount;
				mMapTileSetIndexList.Add(endIndex - 1);
			}
		}
		int count0 = mMapTileSetIndexList.Count;
		for (int i = 0; i < count0; ++i)
		{
			if (textureIndex <= mMapTileSetIndexList[i])
			{
				return i;
			}
		}
		return -1;
	}
	public static void dialogOK(string info){}
	public static TDBuff getBuffData(int id)
	{
		if (id <= 0)
		{
			return null;
		}
		if (mBuffList.ContainsKey(id))
		{
			return mBuffList[id];
		}
		TDBuff buff;
		mSQLiteBuff.query(id, out buff);
		if (buff != null)
		{
			mBuffList.Add(id, buff);
		}
		return buff;
	}
	public static TDBuffDetail getBuffDetailData(int id)
	{
		if (id <= 0)
		{
			return null;
		}
		if (mBuffDetailList.ContainsKey(id))
		{
			return mBuffDetailList[id];
		}
		TDBuffDetail buffDetail;
		mSQLiteBuffDetail.query(id, out buffDetail);
		if (buffDetail != null)
		{
			mBuffDetailList.Add(id, buffDetail);
		}
		return buffDetail;
	}
	public static TDSkillBullet getSkillBulletData(int id)
	{
		if (mQueriedSkillBullet.ContainsKey(id))
		{
			return mQueriedSkillBullet[id];
		}
		TDSkillBullet bullet;
		mSQLiteSkillBullet.query(id, out bullet);
		if (bullet != null)
		{
			mQueriedSkillBullet.Add(id, bullet);
		}
		return bullet;
	}
	public static TDMonsterSkill getMonsterSkillData(int id)
	{
		if (mQueriedMonsterSkill.ContainsKey(id))
		{
			return mQueriedMonsterSkill[id];
		}
		TDMonsterSkill skill;
		mSQLiteMonsterSkill.query(id, out skill);
		if (skill != null)
		{
			mQueriedMonsterSkill.Add(id, skill);
		}
		return skill;
	}
	public static TDPlayerSkill getPlayerSkillData(int id)
	{
		if (mQueriedPlayerSkill.ContainsKey(id))
		{
			return mQueriedPlayerSkill[id];
		}
		TDPlayerSkill skill;
		mSQLitePlayerSkill.query(id, out skill);
		if (skill != null)
		{
			mQueriedPlayerSkill.Add(id, skill);
		}
		return skill;
	}
	public static int getSkillMaxExp(int id, int level)
	{
		var skill = getPlayerSkillData(id);
		return level < skill.mMaxLevel ? skill.mLevelExp[level - 1] : 0;
	}
	public static int getSkillNeedLevel(int id, int level)
	{
		var skill = getPlayerSkillData(id);
		return level < skill.mMaxLevel ? skill.mPlayerLevel[level - 1] : 0;
	}
	public static int getSkillAttack(int id, int level)
	{
		var skill = getPlayerSkillData(id);
		if (level < skill.mMaxLevel && skill.mBullet.Count > 0)
		{
			var bullet = getSkillBulletData(skill.mBullet[0]);
			return bullet.mAttack + bullet.mAttackGrowth * (level - 1);
		}
		return 0;
	}
	public static float getSkillAttackPercent(int id, int level)
	{
		var skill = getPlayerSkillData(id);
		if (level < skill.mMaxLevel && skill.mBullet.Count > 0)
		{
			var bullet = getSkillBulletData(skill.mBullet[0]);
			return bullet.mAttackPercent + bullet.mAttackPercentGrowth * (level - 1);
		}
		return 0.0f;
	}
	public static int getSkillUseMP(int skill, int level)
	{
		TDPlayerSkill playerSkill = getPlayerSkillData(skill);
		if (level <= playerSkill.mMaxLevel)
		{
			return playerSkill.mUseMP + playerSkill.mUseMPGrowth * (level - 1);
		}
		return 99999;
	}
	public static TDMonster getMonsterData(int id)
	{
		if (mQueriedMonsterData.ContainsKey(id))
		{
			return mQueriedMonsterData[id];
		}
		TDMonster data;
		mSQLiteMonster.query(id, out data);
		if (data != null)
		{
			mQueriedMonsterData.Add(id, data);
		}
		return data;
	}
	public static TDMonsterTemplate getMonsterTemplateData(int id)
	{
		if (mQueriedMonsterTemplateData.ContainsKey(id))
		{
			return mQueriedMonsterTemplateData[id];
		}
		TDMonsterTemplate data;
		mSQLiteMonsterTemplate.query(id, out data);
		if (data != null)
		{
			mQueriedMonsterTemplateData.Add(id, data);
		}
		return data;
	}
	public static TDNPC getNPCData(int id)
	{
		if (mQueriedNPCData.ContainsKey(id))
		{
			return mQueriedNPCData[id];
		}
		TDNPC data;
		mSQLiteNPC.query(id, out data);
		if (data != null)
		{
			mQueriedNPCData.Add(id, data);
		}
		return data;
	}
	public static Vector2Int getMapEffectPosition(int id)
	{
		if (mMapEffectPositionList.ContainsKey(id))
		{
			return mMapEffectPositionList[id].mPosition;
		}
		TDMapEffectPosition data;
		mSQLiteMapEffectPosition.query(id, out data);
		if (data != null)
		{
			mMapEffectPositionList.Add(id, data);
			return data.mPosition;
		}
		return Vector2Int.zero;
	}
	public static TDImagePositionEffect getImagePositionEffect(string prefab)
	{
		if (mQueriedImagePositionEffect.ContainsKey(prefab))
		{
			return mQueriedImagePositionEffect[prefab];
		}
		TDImagePositionEffect data;
		mSQLiteImagePositionEffect.query(prefab, out data);
		if (data != null)
		{
			mQueriedImagePositionEffect.Add(prefab, data);
		}
		return data;
	}
	public static float getEffectSpeed(string prefab)
	{
		return getImagePositionEffect(prefab).mSpeed;
	}
	public static bool getEffectLoop(string prefab)
	{
		return getImagePositionEffect(prefab).mLoop != 0;
	}
	public static string getDirAnimationName(string animation, int dir, int dirCount)
	{
		if (dirCount == 0)
		{
			return animation;
		}
		return animation + "_dir" + dir;
	}
	public static string getWeaponAtlasName(int weaponShape)
	{
		string weaponAtlasName = EMPTY_STRING;
		if (weaponShape != 0)
		{
			string weaponAtlasFileName = intToString(weaponShape, 3);
			weaponAtlasName = "Weapon/" + weaponAtlasFileName + "/" + weaponAtlasFileName;
		}
		return weaponAtlasName;
	}
	public static string getClothAtlasName(int clothShape, bool male)
	{
		string clothAtalsFileName = clothShape + (male ? "_M" : "_F");
		return "Human/" + clothAtalsFileName + "/" + clothAtalsFileName;
	}
	// 获取动作长度,不考虑实时设置的播放速度,只考虑表格中动作设置的速度
	public static float getAnimationLength(int action)
	{
		TDAnimation animationData = getAnimationData(action);
		if (animationData == null)
		{
			return 0.0f;
		}
		return animationData.mFrameCount * AnimControl.speedToInterval(animationData.mAnimationSpeed);
	}
	public static TDAnimation getAnimationData(int id)
	{
		if (mAnimationList.ContainsKey(id))
		{
			return mAnimationList[id];
		}
		TDAnimation animationData;
		mSQLiteAnimation.query(id, out animationData);
		mAnimationList.Add(id, animationData);
		return animationData;
	}
	public static TDImagePositionAnimation getImagePositionAnimation(string atlas, string animation)
	{
		if (atlas.Length == 0 || animation.Length == 0)
		{
			return null;
		}
		if (!mQueriedImagePositionAnimation.ContainsKey(atlas))
		{
			mQueriedImagePositionAnimation.Add(atlas, new Dictionary<string, TDImagePositionAnimation>());
		}
		var tableList = mQueriedImagePositionAnimation[atlas];
		if (tableList.ContainsKey(animation))
		{
			return tableList[animation];
		}
		List<TDImagePositionAnimation> data;
		mSQLiteImagePositionAnimation.query(atlas, out data);
		int count = data.Count;
		for (int i = 0; i < count; ++i)
		{
			tableList.Add(data[i].mAnimation, data[i]);
		}
		if (!tableList.ContainsKey(animation))
		{
			logError("can not find atlas:" + atlas + ", animation:" + animation);
		}
		return tableList[animation];
	}
	public static List<Vector2> getEffectPositionList(string prefab)
	{
		if (prefab.Length == 0)
		{
			return null;
		}
		if (mQueriedImagePositionEffectPosList.ContainsKey(prefab))
		{
			return mQueriedImagePositionEffectPosList[prefab];
		}
		// 因为坐标系不同,所以Y坐标需要取反
		List<Vector2> posList = new List<Vector2>();
		TDImagePositionEffect animFrame = getImagePositionEffect(prefab);
		if (animFrame != null)
		{
			for (int j = 0; j < animFrame.mFrameCount; ++j)
			{
				posList.Add(new Vector2(animFrame.mPosX[j], -animFrame.mPosY[j]));
			}
		}
		mQueriedImagePositionEffectPosList.Add(prefab, posList);
		return posList;
	}
	public static List<Vector2> getAnimationPositionList(string atlas, string animation)
	{
		if (atlas.Length == 0 || animation.Length == 0)
		{
			return null;
		}
		if (!mQueriedImagePositionAnimationPosList.ContainsKey(atlas))
		{
			mQueriedImagePositionAnimationPosList.Add(atlas, new Dictionary<string, List<Vector2>>());
		}
		var tableList = mQueriedImagePositionAnimationPosList[atlas];
		if (tableList.ContainsKey(animation))
		{
			return tableList[animation];
		}
		// 因为坐标系不同,所以Y坐标需要取反
		List<Vector2> posList = new List<Vector2>();
		TDImagePositionAnimation animFrame = getImagePositionAnimation(atlas, animation);
		if (animFrame != null)
		{
			for (int j = 0; j < animFrame.mFrameCount; ++j)
			{
				posList.Add(new Vector2(animFrame.mPosX[j], -animFrame.mPosY[j]));
			}
		}
		mQueriedImagePositionAnimationPosList[atlas].Add(animation, posList);
		return posList;
	}
	public static int getSkillEffectDirection(string prefab)
	{
		if (mQueriedSkillEffectDirectionCount.ContainsKey(prefab))
		{
			return mQueriedSkillEffectDirectionCount[prefab].mDirectionCount;
		}
		TDSkillEffectDirection data;
		mSQLiteSkillEffectDirection.query(prefab, out data);
		if (data != null)
		{
			mQueriedSkillEffectDirectionCount.Add(prefab, data);
			return data.mDirectionCount;
		}
		return 0;
	}
	public static TDImagePositionIcon getImagePositionIcon(string atlas, string image)
	{
		if (!mQueriedImagePositionIcon.ContainsKey(atlas))
		{
			mQueriedImagePositionIcon.Add(atlas, new Dictionary<string, TDImagePositionIcon>());
		}
		var tableList = mQueriedImagePositionIcon[atlas];
		if (tableList.ContainsKey(image))
		{
			return tableList[image];
		}
		List<TDImagePositionIcon> data;
		mSQLiteImagePositionIcon.query(atlas, out data);
		int count = data.Count;
		for (int i = 0; i < count; ++i)
		{
			tableList.Add(data[i].mImage, data[i]);
		}
		if (!tableList.ContainsKey(image))
		{
			logError("can not find atlas:" + atlas + ", image:" + image);
		}
		return tableList[image];
	}
	public static TDItemBox getItemBoxData(uint objectID)
	{
		TDItemBox data = null;
		if (mQueriedItemBox.ContainsKey(objectID))
		{
			data = mQueriedItemBox[objectID];
		}
		else
		{
			mSQLiteItemBox.query((int)objectID, out data);
			if (data != null)
			{
				mQueriedItemBox.Add(objectID, data);
			}
		}
		return data;
	}
	public static TDItemEquip getItemEquipData(uint objectID)
	{
		TDItemEquip data = null;
		if (mQueriedItemEquip.ContainsKey(objectID))
		{
			data = mQueriedItemEquip[objectID];
		}
		else
		{
			mSQLiteItemEquip.query((int)objectID, out data);
			if (data != null)
			{
				mQueriedItemEquip.Add(objectID, data);
			}
		}
		return data;
	}
	public static TDItemConsumable getItemConsumableData(uint objectID)
	{
		TDItemConsumable data = null;
		if (mQueriedItemConsumable.ContainsKey(objectID))
		{
			data = mQueriedItemConsumable[objectID];
		}
		else
		{
			mSQLiteItemConsumable.query((int)objectID, out data);
			if (data != null)
			{
				mQueriedItemConsumable.Add(objectID, data);
			}
		}
		return data;
	}
	public static TDItemMaterial getItemMaterialData(uint objectID)
	{
		TDItemMaterial data = null;
		if (mQueriedItemMaterial.ContainsKey(objectID))
		{
			data = mQueriedItemMaterial[objectID];
		}
		else
		{
			mSQLiteItemMaterial.query((int)objectID, out data);
			if (data != null)
			{
				mQueriedItemMaterial.Add(objectID, data);
			}
		}
		return data;
	}
	public static TDItemSkillBook getItemSkillBookData(uint objectID)
	{
		TDItemSkillBook data = null;
		if (mQueriedItemSkillBook.ContainsKey(objectID))
		{
			data = mQueriedItemSkillBook[objectID];
		}
		else
		{
			mSQLiteItemSkillBook.query((int)objectID, out data);
			if (data != null)
			{
				mQueriedItemSkillBook.Add(objectID, data);
			}
		}
		return data;
	}
	public static void destroyEffect(GameEffect2D effect)
	{
		mEffect2DManager.destroyEffect(ref effect);
	}
	public static void destroyEffect(ref GameEffect2D effect)
	{
		mEffect2DManager.destroyEffect(ref effect);
	}
	public static GameEffect2D createEffect(string prefabName, bool active, txUIObject parent = null)
	{
		if (prefabName.Length == 0)
		{
			return null;
		}
		GameEffect2D effect = mEffect2DManager.createEffect(prefabName, getFileNameNoSuffix(prefabName, true) + "_" + makeID());
		if (parent != null)
		{
			effect.setParent(parent);
		}
		effect.move(Vector3.zero);
		effect.setVisible(active);
		return effect;
	}
	public static int vectorToDirection(Vector3 vec, int dirCount)
	{
		if (mDirectionAngle16.Count == 0)
		{
			initAngle();
		}
		// 没有其他方向的就返回0
		if (dirCount == 1)
		{
			return 0;
		}
		float angle = getAngleFromVector2ToVector2(Vector2.up, vec);
		adjustRadian360(ref angle);
		if (dirCount == 8)
		{
			// 找到位于哪两个角度之间
			if (angle <= mDirectionAngle16[1] || angle > mDirectionAngle16[mDirectionAngle16.Count - 1])
			{
				return 0;
			}
			else
			{
				for (int i = 0; i < dirCount; ++i)
				{
					if (i > 0 && angle > mDirectionAngle16[i * 2 - 1] && angle <= mDirectionAngle16[i * 2 + 1])
					{
						return i;
					}
				}
			}
		}
		else if (dirCount == 16)
		{
			// 找到位于哪两个角度之间
			if (angle <= mDirectionAngle32[1] || angle > mDirectionAngle32[mDirectionAngle32.Count - 1])
			{
				return 0;
			}
			else
			{
				for (int i = 0; i < dirCount; ++i)
				{
					if (i > 0 && angle > mDirectionAngle32[i * 2 - 1] && angle <= mDirectionAngle32[i * 2 + 1])
					{
						return i;
					}
				}
			}
		}
		logError("角度计算错误");
		return 0;
	}
	public static Vector3 direction8ToVector(int dir)
	{
		if (mDirectionVector8.Count == 0)
		{
			initAngle();
		}
		if (dir >= 0 && dir < mDirectionVector8.Count)
		{
			return mDirectionVector8[dir];
		}
		return Vector3.zero;
	}
	public static Vector3 mousePosToScenePos(Vector3 mousePos)
	{
		return replaceZ(mSceneCamera.getPosition() + mousePos - (Vector3)getScreenSize() * 0.5f, 0.0f);
	}
	public static Vector3 getMousePositionInScene()
	{
		return mousePosToScenePos(getMousePosition());
	}
	public static int tileIndexToTileX(int index, int mapHeight)
	{
		return index / mapHeight;
	}
	public static int tileIndexToTileY(int index, int mapHeight)
	{
		return index % mapHeight;
	}
	public static Vector2Int tileIndexToTilePos(int index, int mapHeight)
	{
		return new Vector2Int(index / mapHeight, index % mapHeight);
	}
	public static int tilePosToTileIndex(int x, int y, int mapHeight)
	{
		return x * mapHeight + y;
	}
	public static int pixelPosToTileX(Vector3 pixelPos, int mapHeight, int mapWidth)
	{
		int tileX = round(pixelPosToTilePos(pixelPos, mapHeight).x);
		if (tileX < 0 || tileX >= mapWidth)
		{
			return -1;
		}
		return tileX;
	}
	public static int pixelPosToTileY(Vector3 pixelPos, int mapHeight)
	{
		int tileY = round(pixelPosToTilePos(pixelPos, mapHeight).y);
		if (tileY < 0 || tileY >= mapHeight)
		{
			return -1;
		}
		return tileY;
	}
	public static int pixelPosToTileIndex(Vector3 pixelPos, int mapHeight, int mapWidth)
	{
		int tileX = pixelPosToTileX(pixelPos, mapHeight, mapWidth);
		int tileY = pixelPosToTileY(pixelPos, mapHeight);
		if (tileX < 0 || tileY < 0)
		{
			return -1;
		}
		return tilePosToTileIndex(tileX, tileY, mapHeight);
	}
	// 根据地砖左下角的像素坐标转换为地砖下标的x和y
	public static Vector2Int pixelPosToTilePos(Vector2 pixelPos, int mapHeight)
	{
		return new Vector2Int(floor(pixelPos.x * (1.0f / GameDefine.TILE_WIDTH)), floor((mapHeight * GameDefine.TILE_HEIGHT - pixelPos.y) * (1.0f / GameDefine.TILE_HEIGHT)));
	}
	// 计算出地砖坐标所对应的像素坐标,是地砖左下角的像素坐标
	public static Vector2 tilePosToPixelPos(Vector2Int tile, int mapHeight)
	{
		return new Vector2(GameDefine.TILE_WIDTH * tile.x, mapHeight * GameDefine.TILE_HEIGHT - GameDefine.TILE_HEIGHT * tile.y - GameDefine.TILE_HEIGHT);
	}
	// 计算出地砖坐标所对应的像素坐标,是地砖左下角的像素坐标
	public static Vector2 tilePosToPixelPos(int x, int y, int mapHeight)
	{
		return new Vector2(GameDefine.TILE_WIDTH * x, mapHeight * GameDefine.TILE_HEIGHT - GameDefine.TILE_HEIGHT * y - GameDefine.TILE_HEIGHT);
	}
	// 计算出地砖下所对应的像素坐标,是地砖左下角的像素坐标
	public static Vector2 tileIndexToPixelPos(int index, int mapHeight)
	{
		int x = index / mapHeight;
		int y = index % mapHeight;
		return tilePosToPixelPos(new Vector2Int(x, y), mapHeight);
	}
	// 以地砖左下角为原点的像素坐标所处的三角形下标
	public static TILE_TRIANGLE pixelPosToTriangleIndex(Vector2 pos)
	{
		if (pos.x < 0.0f || pos.x > GameDefine.TILE_WIDTH || pos.y < 0.0f || pos.y > GameDefine.TILE_HEIGHT)
		{
			return TILE_TRIANGLE.TT_MAX;
		}
		// 对角线斜率
		float k = (float)GameDefine.TILE_HEIGHT / GameDefine.TILE_WIDTH;
		// 位于左半部分
		if (pos.x <= GameDefine.TILE_WIDTH * 0.5f)
		{
			// 位于左下部分小矩形中
			if (pos.y <= GameDefine.TILE_HEIGHT * 0.5f)
			{
				// 相对于小矩形的右下角的相对坐标
				Vector2 relative = pos - new Vector2(GameDefine.TILE_WIDTH * 0.5f, 0.0f);
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(relative.y / relative.x) > k)
				{
					return TILE_TRIANGLE.TT_INNER_LEFT_BOTTOM;
				}
				else
				{
					return TILE_TRIANGLE.TT_LEFT_BOTTOM;
				}
			}
			// 位于左上部分
			else
			{
				// 相对于地砖中左上角小矩形的左下角的相对坐标
				Vector2 relative = pos - new Vector2(0.0f, GameDefine.TILE_HEIGHT * 0.5f);
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(relative.y / relative.x) > k)
				{
					return TILE_TRIANGLE.TT_LEFT_TOP;
				}
				else
				{
					return TILE_TRIANGLE.TT_INNER_LEFT_TOP;
				}
			}
		}
		// 位于右半部分
		else
		{
			// 位于右下部分
			if (pos.y <= GameDefine.TILE_HEIGHT * 0.5f)
			{
				// 相对于地砖中右下角小矩形的左下角的相对坐标
				Vector2 relative = pos - new Vector2(GameDefine.TILE_WIDTH * 0.5f, 0.0f);
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(relative.y / relative.x) > k)
				{
					return TILE_TRIANGLE.TT_INNER_RIGHT_BOTTOM;
				}
				else
				{
					return TILE_TRIANGLE.TT_RIGHT_BOTTOM;
				}
			}
			// 位于右上部分
			else
			{
				// 相对于地砖中右上角小矩形的左下角的相对坐标
				Vector2 relative = pos - new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f);
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(relative.y / relative.x) > k)
				{
					return TILE_TRIANGLE.TT_RIGHT_TOP;
				}
				else
				{
					return TILE_TRIANGLE.TT_INNER_RIGHT_TOP;
				}
			}
		}
	}
	// 获取指定位置上的三角形三个顶点的坐标,坐标是相对于所属地砖左下角
	public static void getTrianglePoints(TILE_TRIANGLE pos, ref Vector2 point0, ref Vector2 point1, ref Vector2 point2)
	{
		if (pos == TILE_TRIANGLE.TT_LEFT_TOP)
		{
			point0 = new Vector2(0.0f, GameDefine.TILE_HEIGHT);
			point1 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT);
			point2 = new Vector2(0.0f, GameDefine.TILE_HEIGHT * 0.5f);
		}
		else if (pos == TILE_TRIANGLE.TT_RIGHT_TOP)
		{
			point0 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT);
			point1 = new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT);
			point2 = new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f);
		}
		else if (pos == TILE_TRIANGLE.TT_RIGHT_BOTTOM)
		{
			point0 = new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f);
			point1 = new Vector2(GameDefine.TILE_WIDTH, 0.0f);
			point2 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, 0.0f);
		}
		else if (pos == TILE_TRIANGLE.TT_LEFT_BOTTOM)
		{
			point0 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, 0.0f);
			point1 = new Vector2(0.0f, 0.0f);
			point2 = new Vector2(0.0f, GameDefine.TILE_HEIGHT * 0.5f);
		}
		else if (pos == TILE_TRIANGLE.TT_INNER_LEFT_TOP)
		{
			point0 = new Vector2(0.0f, GameDefine.TILE_HEIGHT * 0.5f);
			point1 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT);
			point2 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT * 0.5f);
		}
		else if (pos == TILE_TRIANGLE.TT_INNER_RIGHT_TOP)
		{
			point0 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT * 0.5f);
			point1 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT);
			point2 = new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f);
		}
		else if (pos == TILE_TRIANGLE.TT_INNER_RIGHT_BOTTOM)
		{
			point0 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT * 0.5f);
			point1 = new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f);
			point2 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, 0.0f);
		}
		else if (pos == TILE_TRIANGLE.TT_INNER_LEFT_BOTTOM)
		{
			point0 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT * 0.5f);
			point1 = new Vector2(GameDefine.TILE_WIDTH * 0.5f, 0.0f);
			point2 = new Vector2(0.0f, GameDefine.TILE_HEIGHT * 0.5f);
		}
		else
		{
			logError("三角形位置错误");
		}
	}
	// 像素坐标转换为地图坐标系中的坐标,地图坐标系是以地图左下角为原点,右斜上方为Y轴正方向,右斜下方为X轴正方向
	public static Vector2 pixelPosToMapPos(Vector3 pixelPos)
	{
		Vector2 pixelPos2 = (Vector2)pixelPos;
		// 地图坐标系Y轴起点,就是地图渲染的左下角,而左下角就是世界坐标系的原点
		Line2 axisY = new Line2(Vector2.zero, new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT));
		Line2 axisX = new Line2(Vector2.zero, new Vector2(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT));
		// 经过世界坐标并且平行于地图坐标系Y轴的直线
		Vector2 otherPoint;
		generateParallel(axisY, pixelPos2, out otherPoint);
		Line2 line = new Line2(pixelPos2, otherPoint);
		// 该直线与地图坐标系X轴的交点
		Vector2 intersect;
		intersectLine2(line, axisX, out intersect);
		// 交点到世界坐标的距离就是在地图坐标系中的Y坐标
		float unit = getLength(new Vector2(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT)) * 0.5f;
		float mapPosY = getLength(intersect - pixelPos2) / unit;
		// 交点到地图坐标系原点的距离就是在地图坐标系中的X坐标
		float mapPosX = getLength(intersect) / unit;
		// 点在Y轴的右边则X大于0,在Y轴左边则X小于0
		mapPosX *= getAngleSignFromVectorToVector2(axisY.mEnd - axisY.mStart, pixelPos2);
		return new Vector2(mapPosX, mapPosY);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------
	protected static void initAngle()
	{
		if (mDirectionVector8.Count == 0)
		{
			mDirectionVector8.Add(normalize(new Vector3(0.0f, GameDefine.TILE_HEIGHT)));
			mDirectionVector8.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT)));
			mDirectionVector8.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, 0.0f)));
			mDirectionVector8.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT)));
			mDirectionVector8.Add(normalize(new Vector3(0.0f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector8.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT)));
			mDirectionVector8.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, 0.0f)));
			mDirectionVector8.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT)));
		}
		if (mDirectionVector16.Count == 0)
		{
			mDirectionVector16.Add(normalize(new Vector3(0.0f, GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector16.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, 0.0f)));
			mDirectionVector16.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector16.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.5f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(0.0f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.5f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector16.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, 0.0f)));
			mDirectionVector16.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector16.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT)));
			mDirectionVector16.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT)));
		}
		if (mDirectionVector32.Count == 0)
		{
			mDirectionVector32.Add(normalize(new Vector3(0.0f, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.25f, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.75f, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.75f)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.25f)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, 0.0f)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.25f)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.75f)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.75f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.5f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(GameDefine.TILE_WIDTH * 0.25f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(0.0f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.25f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.5f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.75f, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.75f)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, -GameDefine.TILE_HEIGHT * 0.25f)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, 0.0f)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.25f)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.5f)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT * 0.75f)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.75f, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.5f, GameDefine.TILE_HEIGHT)));
			mDirectionVector32.Add(normalize(new Vector3(-GameDefine.TILE_WIDTH * 0.25f, GameDefine.TILE_HEIGHT)));
		}
		if (mDirectionAngle16.Count == 0)
		{
			foreach (var item in mDirectionVector16)
			{
				float angle = getAngleFromVector2ToVector2(Vector2.up, item);
				adjustRadian360(ref angle);
				mDirectionAngle16.Add(angle);
			}
		}
		if (mDirectionAngle32.Count == 0)
		{
			foreach (var item in mDirectionVector32)
			{
				float angle = getAngleFromVector2ToVector2(Vector2.up, item);
				adjustRadian360(ref angle);
				mDirectionAngle32.Add(angle);
			}
		}
	}
}