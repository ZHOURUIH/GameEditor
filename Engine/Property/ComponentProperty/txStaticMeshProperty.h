#ifndef _TX_STATIC_MESH_PROPERTY_H_
#define _TX_STATIC_MESH_PROPERTY_H_

#include "txComponentProperty.h"

DEFINE_BASE_PROPERTY(txComponentStaticMesh)

// ¾²Ì¬Ä£ÐÍ
class txStaticMeshPropertyStaticMesh : public txComponentStaticMeshProperty<txStringProperty, std::string>
{
public:
	txStaticMeshPropertyStaticMesh();
	virtual void setRealValue(const std::string& value, txComponentStaticMesh* component);
	virtual std::string getRealValue(txComponentStaticMesh* component);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

#endif