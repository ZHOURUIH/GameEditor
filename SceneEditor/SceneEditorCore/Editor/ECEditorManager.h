#ifndef _EC_EDITOR_MANAGER_H_
#define _EC_EDITOR_MANAGER_H_

#include "Utility.h"
#include "txCommandReceiver.h"

#include "CommonDefine.h"

class ECEditor;
class txNode;
class ECEditorManager : public txCommandReceiver
{
public:
	ECEditorManager()
		:
		txCommandReceiver("EditorManager")
	{}
	virtual ~ECEditorManager(){ destroy(); }
	virtual void init(){}
	virtual void update(float elapsedTime);
	void destroy();
	ECEditor* createEditor(const std::string& name);
	void destroyEditor(const std::string& name);
	void destroyEditor(ECEditor* editor);
	ECEditor* getEditor(const std::string& name);
	ECEditor* getEditor(txNode* node);
	static std::string createUniqueName(const std::string& name)
	{
		if (name == EMPTY_STRING)
		{
			return "editor_" + txStringUtility::intToString(mCreatedCount);
		}
		else
		{
			return name + "_" + txStringUtility::intToString(mCreatedCount);
		}
	}
	virtual bool notifyEditorRenamed(const std::string& oldName, ECEditor* editor);
protected:
	txMap<txNode*, ECEditor*> mEditorNodeList;		// 根据场景节点查找到编辑体的列表
	txMap<std::string, ECEditor*> mEditorNameList;	// 编辑体列表
	static int mCreatedCount;
};

#endif