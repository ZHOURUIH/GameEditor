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
	[MenuItem("图集/根据txt和png创建NGUI图集")]
	public static void BuildAtlas()
	{
		DateTime time0 = DateTime.Now;
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_PATH + "GameTexture/MapAtlasTexture/Objects1",
					   CommonDefine.P_RESOURCE_PATH + CommonDefine.R_ATLAS_PATH + "Map/Objects1");
		UnityUtility.messageBox("创建图集结束! 耗时 : " + (DateTime.Now - time0), false);
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

				// 创建NGUI的3个图集文件
				string atlasName = StringUtility.getFileNameNoSuffix(fileList[i], true);
				string prefabFile = atlasAssetPath + "/" + atlasName + ".prefab";
				string matFile = atlasAssetPath + "/" + atlasName + ".mat";

				// 加载文件图集文件，如果不存则创建
				GameObject prefabAsset = AssetDatabase.LoadMainAssetAtPath(prefabFile) as GameObject;
				if (prefabAsset == null)
				{
					GameObject temp = new GameObject();
					prefabAsset = PrefabUtility.CreatePrefab(prefabFile, temp);
					GameObject.DestroyImmediate(temp);
				}

				Material matAsset = AssetDatabase.LoadMainAssetAtPath(matFile) as Material;
				if (matAsset == null)
				{
					matAsset = new Material(Shader.Find("NGUIDefault"));
					AssetDatabase.CreateAsset(matAsset, matFile);
				}

				UIAtlas uiAtlas = prefabAsset.GetComponent<UIAtlas>();
				if (uiAtlas == null)
				{
					uiAtlas = prefabAsset.AddComponent<UIAtlas>();
				}

				// 配置图集参数
				string pngFileName = CommonDefine.P_ASSETS_PATH + fileList[i].Substring(CommonDefine.F_ASSETS_PATH.Length);
				string dataFile = StringUtility.getFileNameNoSuffix(pngFileName) + ".txt";
				Texture2D pngAsset = GetAtlasTexture(pngFileName);
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