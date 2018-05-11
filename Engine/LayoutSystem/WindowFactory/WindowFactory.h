#ifndef _WINDOW_FACTORY_H_
#define _WINDOW_FACTORY_H_

#include "Utility.h"

#include "Window.h"

class WindowFactoryBase
{
public:
	WindowFactoryBase(WINDOW_TYPE type)
		:
		mType(type),
		mCurCount(0),
		mNameCount(0),
		mPublicType(true)
	{
		mTypeName = LayoutUtility::getTypeNameFromWindowType(mType);
	}
	virtual ~WindowFactoryBase()
	{
		if (mCurCount != 0)
		{
			ENGINE_ERROR("error : there is window left! count : %d, window type : %s", mCurCount, mTypeName.c_str());
		}
	}
	virtual Window* createWindow(const std::string& name) = 0;

	virtual std::string createUniqueName()
	{
		return mTypeName + "_" + txStringUtility::intToString(mNameCount);
	}

	void destroyWindow(Window*& window)
	{
		if (window == NULL)
		{
			ENGINE_ERROR("error : can not destroy window, window : NULL %s ", mTypeName.c_str());
			return;
		}
		if (window->getType() != mType)
		{
			ENGINE_ERROR("error : destroy wrong window, window type : %s, factory type : %s ", window->getTypeName().c_str(), mTypeName.c_str());
		}
		TRACE_DELETE(window);
		--mCurCount;
	}
	template<typename T>
	static WindowFactoryBase* createFactory(WINDOW_TYPE type)
	{
		T* factory = TRACE_NEW(T, factory, type);
		return factory;
	}

	const WINDOW_TYPE& getType() { return mType; }
	bool isPublicType() { return mPublicType; }
	void setPublicType(bool isPublic) { mPublicType = isPublic; }
protected:
	WINDOW_TYPE mType;
	std::string mTypeName;
	int mCurCount;
	int mNameCount;		// 命名计数
	bool mPublicType;	// 是否是外部可直接访问的类型,除了预设类型不能直接访问,其他的都可以直接访问
};

template<typename T>
class WindowFactory : public WindowFactoryBase
{
public:
	WindowFactory(WINDOW_TYPE type)
		:
		WindowFactoryBase(type)
	{}
	virtual Window* createWindow(const std::string& name)
	{
		T* window = TRACE_NEW(T, window, name, mType);
		++mCurCount;
		++mNameCount;
		return window;
	}
};

#endif