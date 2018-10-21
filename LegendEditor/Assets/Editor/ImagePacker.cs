using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEngine;

public class ImagePacker
{
	/// <summary>
	/// 自动构建图集菜单命令
	/// </summary>
	[MenuItem("图集/创建地砖图集")]
	public static void BuildAtlasTiles()
	{
		DateTime time0 = DateTime.Now;
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_PATH,
					   CommonDefine.P_ATLAS_PATH + "Map");
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_PATH + "Tiles",
					   CommonDefine.P_ATLAS_PATH + "Map/Tiles");
		UnityUtility.messageBox("创建图集结束! 耗时 : " + (DateTime.Now - time0), false);
	}
	[MenuItem("图集/创建地图物体图集")]
	public static void BuildAtlasObjects()
	{
		DateTime time0 = DateTime.Now;
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_GAME_TEXTURE_PATH + "MapAtlasTexture/Objects1", CommonDefine.P_ATLAS_PATH + "Map/Objects1");
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_GAME_TEXTURE_PATH + "MapAtlasTexture/Objects2", CommonDefine.P_ATLAS_PATH + "Map/Objects2");
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_GAME_TEXTURE_PATH + "MapAtlasTexture/Objects3", CommonDefine.P_ATLAS_PATH + "Map/Objects3");
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_GAME_TEXTURE_PATH + "MapAtlasTexture/Objects4", CommonDefine.P_ATLAS_PATH + "Map/Objects4");
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_GAME_TEXTURE_PATH + "MapAtlasTexture/Objects5", CommonDefine.P_ATLAS_PATH + "Map/Objects5");
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_GAME_TEXTURE_PATH + "MapAtlasTexture/Objects6", CommonDefine.P_ATLAS_PATH + "Map/Objects6");
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_GAME_TEXTURE_PATH + "MapAtlasTexture/Objects7", CommonDefine.P_ATLAS_PATH + "Map/Objects7");
		UnityUtility.messageBox("创建图集结束! 耗时 : " + (DateTime.Now - time0), false);
	}
	// 绝对路径转换为相对于项目的路径
	static string fullPathToProjectPath(string fullPath)
	{
		return CommonDefine.P_ASSETS_PATH + fullPath.Substring(CommonDefine.F_ASSETS_PATH.Length);
	}
	static string projectPathToFullPath(string assetPath)
	{
		return CommonDefine.F_ASSETS_PATH.Substring(0, CommonDefine.F_ASSETS_PATH.Length - CommonDefine.ASSETS.Length - 1) + assetPath;
	}
	/// <summary>
	/// 配置NGUI图集,srcFullPath是txt和png文件所在目录,绝对路径.atlasAssetPath是图集的目录,以Assets开头的路径
	/// </summary>
	private static void SetupNGUIAtlas(string srcFullPath, string atlasAssetPath)
	{
		try
		{
			AssetDatabase.Refresh();
			List<string> fileList = new List<string>();
			FileUtility.findFiles(srcFullPath, ref fileList, ".png", false);
			for (int i = 0; i < fileList.Count; ++i)
			{
				EditorUtility.DisplayProgressBar("设置NGUI图集", "正在进行" + StringUtility.getFileName(fileList[i]), i / (float)fileList.Count);

				FileUtility.createDir(atlasAssetPath);
				string assetNameNoSuffix = atlasAssetPath + "/" + StringUtility.getFileNameNoSuffix(fileList[i], true);
				string prefabFile = assetNameNoSuffix + ".prefab";
				string matFile = assetNameNoSuffix + ".mat";
				string destPNGFile = assetNameNoSuffix + ".png";
				// 拷贝png文件
				FileUtility.copyFile(fileList[i], projectPathToFullPath(destPNGFile));
				AssetDatabase.Refresh();

				// 加载文件图集文件，如果不存则创建
				GameObject prefabAsset = AssetDatabase.LoadMainAssetAtPath(prefabFile) as GameObject;
				if (prefabAsset == null)
				{
					GameObject temp = new GameObject();
					prefabAsset = PrefabUtility.CreatePrefab(prefabFile, temp);
					GameObject.DestroyImmediate(temp);
				}
				UIAtlas uiAtlas = prefabAsset.GetComponent<UIAtlas>();
				if (uiAtlas == null)
				{
					uiAtlas = prefabAsset.AddComponent<UIAtlas>();
				}
				// 创建材质文件
				Material matAsset = AssetDatabase.LoadMainAssetAtPath(matFile) as Material;
				if (matAsset == null)
				{
					matAsset = new Material(Shader.Find("NGUIDefault"));
					AssetDatabase.CreateAsset(matAsset, matFile);
				}

				// 配置图集参数
				string srcPNGFile = fullPathToProjectPath(fileList[i]);
				string dataFile = StringUtility.getFileNameNoSuffix(srcPNGFile) + ".txt";
				Texture2D pngAsset = GetAtlasTexture(destPNGFile);
				TextAsset dataAsset = AssetDatabase.LoadMainAssetAtPath(dataFile) as TextAsset;
				matAsset.SetTexture("_MainTex", pngAsset);
				uiAtlas.spriteMaterial = matAsset;
				NGUIJson.LoadSpriteData(uiAtlas, dataAsset);
				uiAtlas.MarkAsChanged();
				EditorUtility.SetDirty(uiAtlas);
			}
		}
		catch (System.Exception ex)
		{
			Debug.LogError("Error " + ex.ToString());
		}
		finally
		{
			EditorUtility.ClearProgressBar();
		}

		AssetDatabase.Refresh();
	}

	/// <summary>
	/// 获取NGUI图庥参数项的贴图
	/// </summary>
	/// <returns>The atlas texture.</returns>
	/// <param name="path">Path.</param>
	public static Texture2D GetAtlasTexture(string path)
	{
		TextureImporter ti = AssetImporter.GetAtPath(path) as TextureImporter;
		if (ti == null)
		{
			return null;
		}	
		TextureImporterSettings settings = new TextureImporterSettings();
		ti.ReadTextureSettings(settings);
		settings.readable = false;
		settings.wrapMode = TextureWrapMode.Clamp;
		settings.npotScale = TextureImporterNPOTScale.None;
		settings.mipmapEnabled = false;
		settings.aniso = 4;
		settings.alphaIsTransparency = true;
		ti.SetTextureSettings(settings);
		AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate | ImportAssetOptions.ForceSynchronousImport);
		return AssetDatabase.LoadMainAssetAtPath(path) as Texture2D;
	}
}