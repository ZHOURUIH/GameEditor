#include "ECEditorManager.h"
#include "ECEditor.h"
#include "EditorCoreRoot.h"

int ECEditorManager::mCreatedCount = 0;

void ECEditorManager::update(float elapsedTime)
{
	// 根据编辑体类型列表来更新编辑体
	auto iter = mEditorNameList.begin();
	auto iterEnd = mEditorNameList.end();
	FOR_STL(mEditorNameList, ; iter != iterEnd; ++iter)
	{
		iter->second->update(elapsedTime);
	}
	END_FOR_STL(mEditorNameList);
}

void ECEditorManager::destroy()
{
	// 销毁所有编辑体
	auto iter = mEditorNameList.begin();
	auto iterEnd = mEditorNameList.end();
	FOR_STL(mEditorNameList, ; iter != iterEnd; ++iter)
	{
		TRACE_DELETE(iter->second);
	}
	END_FOR_STL(mEditorNameList);
	mEditorNameList.clear();
	mEditorNodeList.clear();
}

ECEditor* ECEditorManager::createEditor(const std::string& name)
{
	std::string editorName = name;
	if (editorName == EMPTY_STRING)
	{
		editorName = createUniqueName(editorName);
	}
	if (mEditorNameList.find(editorName) != mEditorNameList.end())
	{
		return NULL;
	}
	ECEditor* editor = TRACE_NEW(ECEditor, editor, editorName);
	editor->init();
	// 放入编辑体列表
	mEditorNameList.insert(editor->getName(), editor);
	// 放入节点列表中
	mEditorNodeList.insert(editor->getNode(), editor);
	++mCreatedCount;
	return editor;
}

void ECEditorManager::destroyEditor(const std::string& name)
{
	auto iter = mEditorNameList.find(name);
	if (iter == mEditorNameList.end())
	{
		return;
	}
	else
	{
		auto iterNode = mEditorNodeList.find(iter->second->getNode());
		if (iterNode != mEditorNodeList.end())
		{
			mEditorNodeList.erase(iterNode);
		}
		TRACE_DELETE(iter->second);
		mEditorNameList.erase(iter);
	}
}

void ECEditorManager::destroyEditor(ECEditor* editor)
{
	destroyEditor(editor->getName());
}

ECEditor* ECEditorManager::getEditor(const std::string& name)
{
	auto iter = mEditorNameList.find(name);
	if (iter != mEditorNameList.end())
	{
		return iter->second;
	}
	return NULL;
}

ECEditor* ECEditorManager::getEditor(txNode* node)
{
	auto iter = mEditorNodeList.find(node);
	if (iter != mEditorNodeList.end())
	{
		return iter->second;
	}
	return NULL;
}

bool ECEditorManager::notifyEditorRenamed(const std::string& oldName, ECEditor* editor)
{
	// 查找是否有该名字的编辑体
	auto iterOld = mEditorNameList.find(oldName);
	if (iterOld == mEditorNameList.end())
	{
		return false;
	}
	// 查找新的名字是否有重名
	auto iterNew = mEditorNameList.find(editor->getName());
	if (iterNew != mEditorNameList.end())
	{
		return false;
	}
	mEditorNameList.erase(iterOld);
	mEditorNameList.insert(editor->getName(), editor);
	return true;
}