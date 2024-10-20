﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using UnityEngine;
using UnityEditor;

class AssetBuildBundleInfo
{
	public string fileName;
	public string assetName;
	public string bundleName;
	public List<string> dependencies;

	public void AddDependence(string dep)
	{
		if (dependencies == null)
		{
			dependencies = new List<string>();
		}
		dependencies.Add(dep);
	}
}

public class AssetBundlePack : GameBase
{
	// 清理时需要保留的目录和目录的meta
	protected static string[] mKeepFolder = new string[] { "Config", "GameDataFile", "GamePlugin", "DataBase", "Video", "DataTemplate", "HelperExe", "CustomSound" };
	// Resources下的目录,带相对路径,且如果前缀符合,也会认为是不打包的目录
	protected static string[] mUnPackFolder = new string[] { "Scene" };
	protected const string mAssetMenuRoot = "AssetBundle/";
	private static string RES_SRC_PATH = "Assets/Resources/";
	// 打包输出目录
	private static string RES_OUTPUT_PATH = "Assets/StreamingAssets";
	// AssetBundle打包后缀
	private static string ASSET_BUNDLE_SUFFIX = CommonDefine.ASSET_BUNDLE_SUFFIX;
	// xml文件生成器
	private static XMLDocument doc;
	// bundleName <-> List<AssetBuildBundleInfo>
	private static Dictionary<string, List<AssetBuildBundleInfo>> bundleMap = new Dictionary<string, List<AssetBuildBundleInfo>>();
	// 文件名 <-> AssetBuildBundleInfo
	private static Dictionary<string, AssetBuildBundleInfo> fileMap = new Dictionary<string, AssetBuildBundleInfo>();
	[MenuItem(mAssetMenuRoot + "pack/Android")]
	public static void packAssetBundleAndroid()
	{
		packAssetBundle(BuildTarget.Android);
	}
	[MenuItem(mAssetMenuRoot + "pack/Windows")]
	public static void packAssetBundleWindows()
	{
		packAssetBundle(BuildTarget.StandaloneWindows);
	}
	[MenuItem(mAssetMenuRoot + "pack/IOS")]
	public static void packAssetBundleiOS()
	{
		packAssetBundle(BuildTarget.iOS);
	}
	[MenuItem(mAssetMenuRoot + "pack/Lunix")]
	public static void packAssetBundleLinux()
	{
		packAssetBundle(BuildTarget.StandaloneLinux);
	}
	public static void packAssetBundle(BuildTarget target)
	{
		DateTime time0 = DateTime.Now;
		// 清理输出目录
		CreateOrClearOutPath();
		// 清理之前设置过的bundleName
		ClearAssetBundleName();
		// 设置bunderName
		bundleMap.Clear();
		List<string> resList = new List<string>();
		GetAllSubResDirs(RES_SRC_PATH, resList);
		foreach (string dir in resList)
		{
			setAssetBundleName(dir);
		}
		// 打包
		BuildPipeline.BuildAssetBundles(RES_OUTPUT_PATH, BuildAssetBundleOptions.ChunkBasedCompression, target);
		AssetDatabase.Refresh();

		// 构建依赖关系
		AssetBundle assetBundle = AssetBundle.LoadFromFile(CommonDefine.F_STREAMING_ASSETS_PATH + "StreamingAssets");
		AssetBundleManifest mainfest = assetBundle.LoadAsset<AssetBundleManifest>("AssetBundleManifest");
		string[] assetBundleNameList = mainfest.GetAllAssetBundles();
		foreach (string bundle in assetBundleNameList)
		{
			string bundleName = bundle;
			string[] deps = mainfest.GetAllDependencies(bundleName);
			rightToLeft(ref bundleName);
			foreach (string dep in deps)
			{
				string depName = dep;
				rightToLeft(ref depName);
				if (bundleMap.ContainsKey(dep))
				{
					List<AssetBuildBundleInfo> infoList = bundleMap[bundleName];
					foreach (AssetBuildBundleInfo info in infoList)
					{
						info.AddDependence(depName);
					}
				}
			}
		}

		// 生成XML
		doc = new XMLDocument();
		doc.startObject("files");
		foreach (KeyValuePair<string, AssetBuildBundleInfo> pair in fileMap)
		{
			AssetBuildBundleInfo info = pair.Value;

			doc.startObject("file");
			doc.createElement("bundleName", info.bundleName);
			doc.createElement("fileName", info.fileName);
			doc.createElement("assetName", info.assetName);

			if (info.dependencies != null)
			{
				doc.startObject("deps");
				foreach (string dep in info.dependencies)
				{
					doc.createElement("dep", dep);
				}
				doc.endObject("deps");
			}
			doc.endObject("file");
		}
		doc.endObject("files");

		FileStream fs = new FileStream(Path.Combine(RES_OUTPUT_PATH, "StreamingAssets.xml"), FileMode.Create);
		byte[] data = System.Text.Encoding.UTF8.GetBytes(doc.ToString());
		fs.Write(data, 0, data.Length);
		fs.Flush();
		fs.Close();
		UnityUtility.messageBox("资源打包结束! 耗时 : " + (DateTime.Now - time0), false);
	}
	protected static void setAssetBundleName(string fullPath)
	{
		string[] files = Directory.GetFiles(fullPath);
		if (files == null || files.Length == 0)
		{
			return;
		}
		string pathUnderResources = fullPath.Substring(RES_SRC_PATH.Length);
		int unpackCount = mUnPackFolder.Length;
		for (int i = 0; i < unpackCount; ++i)
		{
			// 如果该文件夹是不打包的文件夹,则直接返回
			if (startWith(pathUnderResources, mUnPackFolder[i], false))
			{
				return;
			}
		}
		Debug.Log("Set AssetBundleName Start......");
		string dirBundleName = pathUnderResources.Replace("/", "@") + ASSET_BUNDLE_SUFFIX;
		foreach (string file in files)
		{
			// .asset文件和.meta不打包
			if (file.EndsWith(".meta") || file.EndsWith(".asset"))
			{
				continue;
			}
			AssetImporter importer = AssetImporter.GetAtPath(file);
			if (importer != null)
			{
				string ext = Path.GetExtension(file);
				string bundleName = dirBundleName;
				// prefab和unity(但是一般情况下unity场景文件不打包)单个文件打包
				if (ext != null && (ext.Equals(".prefab") || ext.Equals(".unity")))
				{
					bundleName = file.Substring(RES_SRC_PATH.Length);
					bundleName = bundleName.Replace("/", "@");
					if (null != ext)
					{
						bundleName = bundleName.Replace(ext, ASSET_BUNDLE_SUFFIX);
					}
					else
					{
						bundleName += ASSET_BUNDLE_SUFFIX;
					}
				}
				rightToLeft(ref bundleName);
				bundleName = bundleName.ToLower();
				Debug.LogFormat("Set AssetName Succ, File:{0}, AssetName:{1}", file, bundleName);
				importer.assetBundleName = bundleName;
				EditorUtility.UnloadUnusedAssetsImmediate();

				string fileName = file;
				fileName = fileName.ToLower();
				rightToLeft(ref fileName);
				// 存储bundleInfo
				AssetBuildBundleInfo info = new AssetBuildBundleInfo();
				info.assetName = fileName;
				info.bundleName = bundleName;
				if (ext != null)
				{
					int index = fileName.IndexOf(ext.ToLower());
					info.fileName = fileName.Substring(0, index);
				}
				else
				{
					info.fileName = fileName;
				}
				fileMap.Add(fileName, info);

				List<AssetBuildBundleInfo> infoList = null;
				bundleMap.TryGetValue(info.bundleName, out infoList);
				if (null == infoList)
				{
					infoList = new List<AssetBuildBundleInfo>();
					bundleMap.Add(info.bundleName, infoList);
				}
				infoList.Add(info);
			}
			else
			{
				Debug.LogFormat("Set AssetName Fail, File:{0}, Msg:Importer is null", file);
			}
		}
		Debug.Log("Set AssetBundleName End......");
	}
	// 递归获取所有子目录文件夹
	protected static void GetAllSubResDirs(string fullPath, List<string> dirList)
	{
		if ((dirList == null) || (string.IsNullOrEmpty(fullPath)))
		{
			return;
		}

		string[] dirs = Directory.GetDirectories(fullPath);
		if (dirs != null && dirs.Length > 0)
		{
			for (int i = 0; i < dirs.Length; ++i)
			{
				GetAllSubResDirs(dirs[i], dirList);
			}
		}
		dirList.Add(fullPath);
	}
	// 创建和清理输出目录
	protected static void CreateOrClearOutPath()
	{
		if (!Directory.Exists(RES_OUTPUT_PATH))
		{
			Directory.CreateDirectory(RES_OUTPUT_PATH);
		}
		else
		{
			// 查找目录下的所有第一级子目录
			string[] dirList = Directory.GetDirectories(RES_OUTPUT_PATH);
			int dirCount = dirList.Length;
			for (int i = 0; i < dirCount; ++i)
			{
				// 只删除不需要保留的目录
				if (!isKeepFolderOrMeta(getFolderName(dirList[i])))
				{
					Directory.Delete(dirList[i], true);
				}
			}
			// 查找目录下的所有第一级文件
			string[] files = Directory.GetFiles(RES_OUTPUT_PATH);
			int fileCount = files.Length;
			for (int i = 0; i < fileCount; ++i)
			{
				if (!isKeepFolderOrMeta(getFileName(files[i])))
				{
					File.Delete(files[i]);
				}
			}
		}
	}
	protected static bool isKeepFolderOrMeta(string name)
	{
		int count = mKeepFolder.Length;
		for (int i = 0; i < count; ++i)
		{
			if (mKeepFolder[i] == name || mKeepFolder[i] + ".meta" == name)
			{
				return true;
			}
		}
		return false;
	}
	// 清理之前设置的bundleName
	protected static void ClearAssetBundleName()
	{
		string[] bundleNames = AssetDatabase.GetAllAssetBundleNames();
		int length = bundleNames.Length;
		for (int i = 0; i < length; ++i)
		{
			AssetDatabase.RemoveAssetBundleName(bundleNames[i], true);
			UnityUtility.logInfo("remove asset bundle name : " + bundleNames[i]);
		}
	}
}