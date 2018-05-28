#ifndef _EC_EDITOR_H_
#define _EC_EDITOR_H_

#include "txMovableObject.h"

#include "EditorCoreBase.h"

class txComponent;
class ECScene;
class ECEditor : public txMovableObject, public EditorCoreBase
{
public:
	ECEditor(const std::string& name)
		:
		txMovableObject(name),
		mScene(NULL)
	{}
	virtual ~ECEditor(){ destroy(); }
	virtual void initComponents(){}
	virtual void setComponentProperty(txComponent* component, txProperty* prop, const std::string& propertyValue);
	virtual void destroyComponent(const std::string& name);
	void destroy(){ mScene = NULL; }
	void setScene(ECScene* scene) { mScene = scene; }
	ECScene* getScene() { return mScene; }
	bool rename(const std::string& newName);
	void saveEditor(std::string& stream, const int& tableCount);
protected:
	static void saveComponent(txComponent* component, std::string& stream, const int& tableCount);
protected:
	ECScene* mScene;	// 编辑体所属的场景
};

#endif