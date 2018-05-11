#include "txComponentHeader.h"
#include "txStaticMeshProperty.h"
#include "txEntity.h"

void txComponentStaticMesh::initProperty()
{
	txComponentMesh::initProperty();
	addProperty<txStaticMeshPropertyStaticMesh>();

	// 为下拉列表添加数据
	txStaticMeshPropertyStaticMesh* staticMeshProperty = getProperty<txStaticMeshPropertyStaticMesh>();
	if (staticMeshProperty->getChoices() != NULL && staticMeshProperty->getChoices()->size() == 0)
	{
		txVector<std::string> fileList;
		txFileUtility::findFiles(P_MODEL_STATIC_PATH, fileList, ".mesh");
		txVector<std::string> fileOnlyList;
		int fileCount = fileList.size();
		FOR_STL(fileList, int i = 0; i < fileCount; ++i)
		{
			fileOnlyList.push_back(txStringUtility::getFileName(fileList[i]));
		}
		END_FOR_STL(fileList);
		staticMeshProperty->setChoices(fileOnlyList);
	}
}