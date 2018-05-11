#ifndef _NUMBER_WINDOW_H_
#define _NUMBER_WINDOW_H_

#include "Window.h"

class txTexture;
class txTextureManager;
// 类似于将每个数字都当作一个独立的窗口来存储
struct NumberRenderData
{
	GLfloat mWindowVertices[UI_VERTEX_COUNT * 3];				// 窗口渲染顶点坐标
	GLfloat mTexCoords[UI_TEXTURE_COORD_COUNT * 2];				// 纹理坐标
	GLfloat mWireframeVertices[UI_VERTEX_COUNT * 3];			// 边框的顶点坐标
	GLfloat mCoordinateVertices[UI_VERTEX_COUNT * 3];			// 坐标系顶点坐标
	VECTOR2 mTextureCoord;										// 纹理坐标
	VECTOR2 mTextureSize;										// 纹理大小
};
class NumberWindow : public Window
{
public:
	NumberWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~NumberWindow();
	virtual void update(const float& elapsedTime);
	virtual void render();
	void setNumber(const std::string& number, const bool& refreshNow = false);
	void setNumberStyle(const std::string& style, const bool& async);
	static void refreshNumberStyleList();
	virtual void finishAsycLoad();
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
	virtual void notifyWindowRectChanged(){ setNumberDataDirty(true); }
	virtual void setMatrixScale(const VECTOR2& scale, const bool& refreshNow = false)
	{
		Window::setMatrixScale(scale, refreshNow);
		setNumberDataDirty(true);
	}
	virtual void setMatrixRotation(const MATRIX3& rotation, const bool& refreshNow = false)
	{
		Window::setMatrixRotation(rotation, refreshNow);
		setNumberDataDirty(true);
	}
	virtual void setMatrixTranslation(const VECTOR3& trans, const bool& refreshNow = false)
	{
		Window::setMatrixTranslation(trans, refreshNow);
		setNumberDataDirty(true);
	}

	// 设置成员变量
	void setNumberInterval(const txDim& interval)					{ mInterval = interval; setNumberDataDirty(true); }
	void setDockingPosition(const WINDOW_DOCKING_POSITION& position){ mDockingPosition = position; setNumberDataDirty(true); }
	void setRotateWithWindow(const bool& rotate)					{ mRotateWithWindow = rotate; setNumberDataDirty(true); }
	void setMaxNumberCount(const int& count)						{ mMaxCount = count; setNumberDataDirty(true); }
	void setNumberDataDirty(const bool& dirty)						{ mNumberDataDirty = dirty; }
	static void setShowNumberRect(const bool& show)					{ mShowNumberRect = show; }

	// 获得成员变量
	const std::string& getNumber()									{ return mNumber; }
	const std::string& getNumberStyle()								{ return mNumberStyle; }
	const VECTOR2& getNumberTextureSize()							{ return mNumberTextureSize; }
	const VECTOR2& getDotTextureSize()								{ return mDotTextureSize; }
	const txDim& getNumberInterval()								{ return mInterval; }
	const int& getMaxNumberCount()									{ return mMaxCount; }
	const WINDOW_DOCKING_POSITION& getDockingPosition()				{ return mDockingPosition; }
	const bool& getRotateWithWindow()								{ return mRotateWithWindow; }
	static const txSet<std::string>& getNumberStyleList()			{ return mNumberStyleList; }
	static const bool& getShowNumberRect()							{ return mShowNumberRect; }
protected:
	virtual void initProperty();
	// 刷新渲染数据,当数字最大位数改变,或者数字内容改变,窗口大小,位置改变,数字风格改变,数字停靠位置改变,跟随旋转改变,纹理大小改变
	void refreshNumberData();
protected:
	txVector<NumberRenderData*> mNumberData;		// 数字的渲染数据,列表长度等于最大数字个数,包含小数点
	std::string mNumber;							// 表示数字的字符串
	txTexture* mNumberTexture[10];					// 所有数字的纹理
	txTexture* mDotTexture;							// 小数点纹理
	std::string mNumberStyle;						// 数字风格,数字风格决定渲染数字时使用什么纹理
	int mMaxCount;									// 最大数字位数,小数点也计算在内
	txDim mInterval;								// 数字渲染间距
	bool mRotateWithWindow;							// 数字是否随着窗口整体旋转,为真则数字窗口整体旋转,为假则每个数字单独旋转
	bool mNumberDataDirty;							// 是否需要刷新数字渲染数据
	WINDOW_DOCKING_POSITION mDockingPosition;		// 数字的停靠位置
	VECTOR2 mNumberTextureSize;						// 数字纹理大小
	VECTOR2 mDotTextureSize;						// 小数点纹理大小
	static bool mShowNumberRect;					// 是否显示每个数字的线框
	static txSet<std::string> mNumberStyleList;		// 数字风格列表
};

#endif