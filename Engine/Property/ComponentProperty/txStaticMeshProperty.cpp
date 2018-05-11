#include "txStaticMeshProperty.h"

//---------------------------------------------------------------------------------------------------------------------------------------------
txVector<std::string> txStaticMeshPropertyStaticMesh::mChoices;
txStaticMeshPropertyStaticMesh::txStaticMeshPropertyStaticMesh()
{
	mPropertyName = "StaticMesh";
	mDescription = "¾²Ì¬Ä£ÐÍ";
}
void txStaticMeshPropertyStaticMesh::setRealValue(const std::string& value, txComponentStaticMesh* component)
{
	component->initEntityByFileName(P_MODEL_STATIC_PATH + value, false, true);
}
std::string txStaticMeshPropertyStaticMesh::getRealValue(txComponentStaticMesh* component)
{
	return component->getModelFileName();
}