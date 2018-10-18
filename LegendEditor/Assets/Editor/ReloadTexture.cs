using UnityEngine;
using UnityEditor;
using System.Collections;
using System.Collections.Generic;
using System;

public class ReloadTexture
{
	[MenuItem("Reload/Texture")]
	static public void reloadDitherTexture()
	{
		DateTime start = DateTime.Now;
		string[] pathList = new string[] { "Objects3", "Objects4", "Objects5", "Objects6", "Objects7", "Tiles", "SmTiles" };
		foreach (var item in pathList)
		{
			reloadPath(CommonDefine.F_RESOURCES_PATH + "Texture/GameTexture/MapTexture/" + item);
		}
		UnityUtility.messageBox("重新导入结束,耗时 : " + (DateTime.Now - start), false);
	}
	//----------------------------------------------------------------------------------------------------------------------------
	static protected void reloadPath(string path)
	{
		List<string> files = new List<string>();
		FileUtility.findFiles(path, ref files, ".png");
		foreach(var item in files)
		{
			string curPath = CommonDefine.ASSETS + "/" + item.Substring(CommonDefine.F_ASSETS_PATH.Length);
			reloadTexture(curPath);
		}
	}
	static protected void reloadTexture(string name)
	{
		// 重新导入图片,触发TextureModifier中的处理函数
		AssetDatabase.ImportAsset(name);
	}
}

class TextureModifier : AssetPostprocessor
{
	//void OnPreprocessTexture()
	//{
	//	var importer = (assetImporter as TextureImporter);
	//	importer.textureType = TextureImporterType.Default;
	//	importer.npotScale = TextureImporterNPOTScale.None;
	//	importer.isReadable = false;
	//	importer.mipmapEnabled = false;
	//	importer.alphaIsTransparency = true;
	//	importer.wrapMode = TextureWrapMode.Clamp;
	//}
}
