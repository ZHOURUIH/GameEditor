#pragma once

#include "FrameDefine.h"

class Undo;
class UndoManager
{
public:
	UndoManager() = default;
	virtual ~UndoManager(){ destroy(); }
	void destroy();
	void undo();
	void redo();
	bool canUndo() const {return mUndoBuffer.size() > 0;}
	bool canRedo() const {return mRedoBuffer.size() > 0;}
	void setUndoChangeCallback(Action callback) { mUndoChangeCallback = callback; }
	void clearRedo();
	void clearUndo();
	void checkUndoRedoEnable();
	template<typename T, typename TypeCheck = IsSubClassOf<Undo, T>::mType>
	T* addUndo() 
	{
		T* undo = new T();
		addUndo(undo);
		return undo;
	}
protected:
	void addUndo(Undo* undoCollection);
	// beforeEnable�Ǹı�֮ǰ�Ŀɳ���״̬,nowEnable�Ǹı�֮��Ŀɳ���״̬,force������״̬��û�иı䶼ǿ�Ʒ����¼�
	void undoChanged(bool beforeEnable, bool nowEnable, bool force = false);
	void redoChanged(bool beforeEnable, bool nowEnable, bool force = false);
protected:
	Vector<Undo*> mUndoBuffer;
	Vector<Undo*> mRedoBuffer;
	bool mUndoing = false;	// �Ƿ����ڳ���,�������,����ӳ�������ʱʵ���������������
	bool mRedoing = false;	// �Ƿ���������,�����������,������ӳ�������ʱ��Ҫ�������б����
	Action mUndoChangeCallback;
};