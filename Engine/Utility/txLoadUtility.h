#ifndef _TX_LOAD_UTILITY_H_
#define _TX_LOAD_UTILITY_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class txMesh;
class txSkeleton;
class txAnimation;
struct SkinMeshPair;
class txEntity;
class txTexture;
class txLoadUtility : public txEngineBase
{
public:
	// 加载静态模型
	static bool loadMeshFromFile(const std::string& fileName, txMesh* pMesh, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	// 加载动态模型
	static bool loadSkeletonMeshFromFile(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	// json格式的场景文件转换为二进制的场景文件,fullFileName为代完整路径的文件名,可以是绝对路径,也可以是media下的相对路径
	// 返回的也是带完整路径的文件名,transMeshFile为是否连模型文件也一起转换为二进制的
	static bool jsonSceneFileToBinarySceneFile(const std::string& fullFileName, std::string& newFileName, const bool& transMeshFile);
	// json格式的模型文件转换为二进制的模型文件,fullFileName为代完整路径的文件名,可以是绝对路径,也可以是media下的相对路径
	// 返回的也是带完整路径的文件名
	static bool jsonMeshFileToBinaryMeshFile(const std::string& fullFileName, std::string& newFileName);
	static bool jsonSkeletonMeshFileToBinarySkeletonMeshFile(const std::string& fullFileName, std::string& newFileName);
	static bool jsonSkeletonFileToBinarySkeletonFile(const std::string& fullFileName, std::string& newFileName);
	static bool jsonAnimationFileToBinaryAnimationFile(const std::string& fullFileName, std::string& newFileName);
protected:
	static bool loadJsonMesh(const std::string& fileName, txMesh* pMesh, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadBinaryMesh(const std::string& fileName, txMesh* pMesh, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadJsonSkeletonMesh(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadBinarySkeletonMesh(const std::string& fileName, txVector<SkinMeshPair>& skinMeshPairList, txEntity* pEntity, const bool& loadTexture = true, txVector<txTexture*>* textureList = NULL);
	static bool loadJsonSkeleton(const std::string& fileName, txSkeleton* pSkeleton);
	static bool loadBinarySkeleton(const std::string& fileName, txSkeleton* pSkeleton);
	static bool loadJsonAnimation(const std::string& fileName, txAnimation* pAnimation);
	static bool loadBinaryAnimation(const std::string& fileName, txAnimation* pAnimation);
};

#endif