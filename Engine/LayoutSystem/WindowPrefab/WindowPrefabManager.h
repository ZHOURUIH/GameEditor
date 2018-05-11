#ifndef _WINDOW_PREFAB_MANAGER_H_
#define _WINDOW_PREFAB_MANAGER_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

class WindowPrefab;
class LayoutManager;
class Window;
class WindowPrefabManager : txEngineBase
{
public:
	WindowPrefabManager(){}
	virtual ~WindowPrefabManager(){ destroy(); }
	void init();
	void destroy();
	void loadAllPrefab();
	void loadPrefab(const std::string& fileName);
	// 创建一个空的预设
	WindowPrefab* createPrefab(const std::string& name, const bool& sendEvent = true);
	// 根据一个窗口创建一个预设,会将该窗口中的所有子窗口拷贝到预设中
	WindowPrefab* createPrefab(Window* window, const std::string& name);
	Window* createWindow(const std::string& typeName, const std::string& name);
	void destroyPrefab(const std::string& name);
	void destroyPrefab(WindowPrefab* prefab);
	WindowPrefab* getPrefab(const std::string& name);
	txMap<std::string, WindowPrefab*>& getPrefabList() { return mPrefabList; }
protected:
	void addChildToList(Window* window);
	void removeChildFromList(Window* window);
protected:
	txMap<std::string, WindowPrefab*> mPrefabList;
	txMap<std::string, Window*> mWindowList;	// 所有的预设中的子窗口,不包括预设,也不包括预设子窗口中的内部子窗口
};

#endif