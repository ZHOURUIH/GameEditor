#ifndef _TX_FREE_TYPE_FONT_H_
#define _TX_FREE_TYPE_FONT_H_

#include "freetype/ft2build.h" 
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"

#include "txEngineDefine.h"

class txFreeTypeFont
{
public:
	txFreeTypeFont() {}
	virtual ~txFreeTypeFont();
	virtual void init();
	virtual void update(float elapsedTime){}
	virtual void destroy();
	void refreshFontList();
	const txVector<std::string>& getFontFileNameList() { return mFontFileNameList; }
	bool getFontBitmapData(const std::string& fontName, const wchar_t& text, unsigned char* buffer, const int& width, const int& height, int& textLeft, int& textTop, int& textWidth, int& textHeight);
protected:
	FT_Library mLibrary;
	txMap<std::string, FT_Face> mFontList;
	txVector<std::string> mFontFileNameList;
};

#endif
