#ifndef _TEXT_WINDOW_H_
#define _TEXT_WINDOW_H_

#include "Window.h"

class txFreeTypeFont;
class txTextureManager;
class txTexture;
struct TextRenderData
{
	GLfloat mWindowVertices[UI_VERTEX_COUNT * 3];				// 窗口渲染顶点坐标
	GLfloat mTexCoords[UI_TEXTURE_COORD_COUNT * 2];				// 纹理坐标
	GLfloat mWireframeVertices[UI_VERTEX_COUNT * 3];			// 边框的顶点坐标
	GLfloat mCoordinateVertices[UI_VERTEX_COUNT * 3];			// 坐标系顶点坐标
	VECTOR2 mTextureCoord;										// 纹理坐标
	VECTOR2 mTextureSize;										// 纹理大小
	txTexture* mTexture;
	int mTextLeft;		// 文字左边界所在的像素位置
	int mTextTop;		// 文字顶端所在的像素位置
	int mTextWidth;		// 文字的像素宽度,与图片中的大小不同,文字宽高是指在图片中文字区域所占的宽高
	int mTextHeight;	// 文字的像素高度
};

class TextWindow : public Window
{
public:
	TextWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~TextWindow();
	virtual void update(const float& elapsedTime);
	virtual void render();
	virtual void notifyWindowRectChanged(){ setTextDataDirty(true); }

	// 获得成员变量
	const txDim& getSpace()								{ return mSpace; }
	const std::string& getFontName()					{ return mFontName; }
	const bool& getRotateWithWindow()					{ return mRotateWithWindow; }
	const WINDOW_DOCKING_POSITION& getDockingPosition()	{ return mDockingPosition; }
	const int& getMaxLength()							{ return mMaxLength; }
	const bool& getTextDataDirty()						{ return mTextDataDirty; }
	static const bool& getShowTextRect()				{ return mShowTextRect; }
	virtual const std::string& getText()				{ return mText; }

	// 设置成员变量
	void setSpace(const txDim& space)									{ mSpace = space; setTextDataDirty(true); }
	void setRotateWithWindow(const bool& rotate)						{ mRotateWithWindow = rotate; setTextDataDirty(true); }
	void setDockingPosition(const WINDOW_DOCKING_POSITION& position)	{ mDockingPosition = position; setTextDataDirty(true); }
	void setMaxLength(const int& length)								{ mMaxLength = length; setTextDataDirty(true); }
	void setTextDataDirty(const bool& dirty)							{ mTextDataDirty = dirty; }
	static void setShowTextRect(const bool& show)						{ mShowTextRect = show; }

	void setFontName(const std::string& fontName, const bool& async);
	virtual void setText(const std::string& text, const bool& async);
	int getContentWidth();// 得到文字内容的宽
	virtual void finishAsycLoad();
protected:
	virtual void initProperty();
	txTexture* initFontTexture(const wchar_t& key, int& textLeft, int& textTop, int& textWidth, int& textHeight);
	virtual void notifyPixelSizeChanged() { setTextDataDirty(true); }
	// 刷新文字渲染数据
	void refreshTextData();
protected:
	txVector<TextRenderData*> mTextData;		// 文字渲染数据,字体如果在外部修改了,则文字渲染可能不会改变,因为没有去修改已经创建的文字纹理
	bool mTextDataDirty;						// 是否需要更新文字渲染数据
	int mMaxLength;								// 字符最大长度,是渲染个数的最大数量
	txDim mSpace;								// 间距,其中的相对坐标是相对于该窗口的最终高度的
	std::string mText;							// 文本内容
	std::wstring mUnicodeText;					// unicode编码的文本,其长度等于文字的渲染个数
	std::string mFontName;						// 字体名
	WINDOW_DOCKING_POSITION mDockingPosition;	// 文字停靠方式
	bool mRotateWithWindow;						// 文字是否随着窗口整体旋转,为真则文字窗口整体旋转,为假则每个文字单独旋转
	static bool mShowTextRect;					// 是否显示每个文字的边框
};

#endif