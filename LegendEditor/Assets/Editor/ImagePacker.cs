using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEngine;

#if USE_NGUI
public class ImagePacker : GameBase
{
	[MenuItem("图集/创建图标图集", false, 2)]
	public static void BuildIcon()
	{
		DateTime time0 = DateTime.Now;
		SetupNGUIAtlas(CommonDefine.F_TEXTURE_PATH, CommonDefine.P_ATLAS_PATH, false);
		messageBox("创建图集结束! 耗时 : " + (DateTime.Now - time0), false);
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
	// 配置NGUI图集,srcFullPath是txt和png文件所在目录,绝对路径.atlasAssetPath是图集的目录,以Assets开头的路径
	private static void SetupNGUIAtlas(string srcFullPath, string atlasAssetPath = "Assets/Resources/Atlas/GameAtlas", bool recursive = true)
	{
		removeEndSlash(ref atlasAssetPath);
		try
		{
			AssetDatabase.Refresh();
			List<string> fileList = findFilesNonAlloc(srcFullPath, ".png", recursive);
			for (int i = 0; i < fileList.Count; ++i)
			{
				EditorUtility.DisplayProgressBar("设置NGUI图集", "正在进行" + getFileName(fileList[i]), i / (float)fileList.Count);
				string atlasName = getFileNameNoSuffix(fileList[i], true);
				makeAtlas(fileList[i], atlasAssetPath + atlasName);
			}
		}
		catch (Exception ex)
		{
			Debug.LogError("Error " + ex.ToString());
		}
		finally
		{
			EditorUtility.ClearProgressBar();
		}
	}
	[MenuItem("Assets/图集/创建NGUI图集")]
	private static void createAtlas()
	{
		string fullPath = projectPathToFullPath(AssetDatabase.GetAssetPath(Selection.activeObject));
		if(getFileSuffix(fullPath) != ".png")
		{
			EditorUtility.DisplayDialog("错误", "需要选中png文件", "确认");
			return;
		}
		makeAtlas(fullPath, CommonDefine.P_GAME_ATLAS_PATH + getFileNameNoSuffix(fullPath, true));
	}
	// fullFileName是图集大图的绝对路径,带后缀,assetNameNoSuffixWithPath是以Assets开头的带图集名的路径
	public static void makeAtlas(string fullFileName, string assetNameNoSuffixWithPath)
	{
		// 相对于项目的路径
		createDir(getFilePath(assetNameNoSuffixWithPath));
		string prefabFile = assetNameNoSuffixWithPath + ".prefab";
		if(isFileExist(projectPathToFullPath(prefabFile)))
		{
			if(!EditorUtility.DisplayDialog("覆盖", "图集:" + getFileNameNoSuffix(prefabFile) + "已存在,是否覆盖?", "是", "否"))
			{
				return;
			}
		}
		string matFile = assetNameNoSuffixWithPath + ".mat";
		string destPNGFile = assetNameNoSuffixWithPath + ".png";
		string txtFileFullPath = replaceSuffix(fullFileName, ".txt");
		if (!isFileExist(txtFileFullPath))
		{
			EditorUtility.DisplayDialog("错误", "找不到 : " + txtFileFullPath, "确认");
			return;
		}
		// 拷贝png文件
		copyFile(fullFileName, projectPathToFullPath(destPNGFile));
		AssetDatabase.Refresh();

		// 加载文件图集文件，如果不存则创建
		GameObject prefabAsset = AssetDatabase.LoadMainAssetAtPath(prefabFile) as GameObject;
		if (prefabAsset == null)
		{
			GameObject temp = new GameObject();
			prefabAsset = PrefabUtility.SaveAsPrefabAsset(temp, prefabFile);
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
		string dataFile = fullPathToProjectPath(txtFileFullPath);
		Texture2D pngAsset = GetAtlasTexture(destPNGFile);
		TextAsset dataAsset = AssetDatabase.LoadMainAssetAtPath(dataFile) as TextAsset;
		matAsset.SetTexture("_MainTex", pngAsset);
		uiAtlas.spriteMaterial = matAsset;
		NGUIJson.LoadSpriteData(uiAtlas, dataAsset);
		uiAtlas.MarkAsChanged();
		EditorUtility.SetDirty(uiAtlas);
		AssetDatabase.SaveAssets();
		AssetDatabase.Refresh();
	}
	// 获取NGUI图庥参数项的贴图
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
		settings.filterMode = FilterMode.Point;
		settings.mipmapEnabled = false;
		settings.aniso = 4;
		settings.alphaIsTransparency = true;
		ti.SetTextureSettings(settings);
		AssetDatabase.ImportAsset(path, ImportAssetOptions.ForceUpdate | ImportAssetOptions.ForceSynchronousImport);
		return AssetDatabase.LoadMainAssetAtPath(path) as Texture2D;
	}
}
#endif