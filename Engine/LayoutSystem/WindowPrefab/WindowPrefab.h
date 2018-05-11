#ifndef _WINDOW_PREFAB_H_
#define _WINDOW_PREFAB_H_

#include "Window.h"

class WindowPrefab : public Window
{
public:
	WindowPrefab(const std::string& name, WINDOW_TYPE type)
		:
		Window(name, type),
		mFileName(name + PREFAB_SUFFIX)
	{}
	virtual ~WindowPrefab(){}
	void savePrefab(std::string& stream);
	const std::string& getFileName() { return mFileName; }
	void setFileName(const std::string& fileName) { mFileName = fileName; }
protected:
	std::string mFileName;
};

#endif