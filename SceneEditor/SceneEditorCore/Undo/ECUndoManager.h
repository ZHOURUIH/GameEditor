#ifndef _EC_UNDO_MANAGER_H_
#define _EC_UNDO_MANAGER_H_

#include "txEngineDefine.h"
#include "EditorCoreBase.h"

class ECUndo;
class ECUndoManager : public EditorCoreBase
{
public:
	ECUndoManager()
		:
		mUndoing(false),
		mRedoing(false)
	{}
	virtual ~ECUndoManager(){ destroy(); }
	void init(){}
	void destroy();
	void addUndo(ECUndo* undoCollection);
	void undo();
	void redo();
	bool canUndo(){return mUndoBuffer.size() > 0;}
	bool canRedo(){return mRedoBuffer.size() > 0;}
	void clearRedo();
	void clearUndo();
	void checkUndoRedoEnable();
	template<typename T>
	T* createUndo()
	{
		T* undoCollection = TRACE_NEW(T, undoCollection);
		return undoCollection;
	}
protected:
	// beforeEnable是改变之前的可撤销状态,nowEnable是改版之后的可撤销状态,force是无论状态有没有改变都强制发送事件
	void undoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force = false);
	void redoChanged(const bool& beforeEnable, const bool& nowEnable, const bool& force = false);
protected:
	txVector<ECUndo*> mUndoBuffer;
	txVector<ECUndo*> mRedoBuffer;
	bool mUndoing;	// 是否正在撤销,如果撤销,则添加撤销操作时实际是添加重做操作
	bool mRedoing;	// 是否正在重做,如果不是重做,则在添加撤销操作时需要将重做列表清空
};

#endif