#include "txFreeTypeFont.h"
#include "Utility.h"
#include "txEngineDefine.h"
#include "txEngineRoot.h"
#include "txEngineLog.h"

txFreeTypeFont::~txFreeTypeFont()
{
	destroy();
}

void txFreeTypeFont::init()
{
	FT_Error error = FT_Init_FreeType(&mLibrary);
	if (error) 
	{
		ENGINE_ERROR("error : an error occurred during library initialization");
		return;
	}

	refreshFontList();
}

void txFreeTypeFont::refreshFontList()
{
	// 卸载全部已加载的字体
	if (mFontList.size() > 0)
	{
		auto iterFont = mFontList.begin();
		auto iterFontEnd = mFontList.end();
		FOR_STL(mFontList, ; iterFont != iterFontEnd; ++iterFont)
		{
			FT_Done_Face(iterFont->second);
		}
		END_FOR_STL(mFontList);
		mFontList.clear();
	}
	mFontFileNameList.clear();

	// 查找字体文件
	FT_Error error;
	txVector<std::string> fontFileList;
	txFileUtility::findFiles(P_FONT_PATH, fontFileList, ".ttf");
	int fileCount = fontFileList.size();
	FOR_STL(fontFileList, int i = 0; i < fileCount; ++i)
	{
		// 加载字体
		FT_Face face;
		error = FT_New_Face(mLibrary, fontFileList[i].c_str(), 0, &face);
		if (error == FT_Err_Unknown_File_Format)
		{
			ENGINE_ERROR("error : the font file could be opened and read, its font format is unsupported");
		}
		else if (error)
		{
			ENGINE_ERROR("error : the font file could not be opened or read, or that it is broken");
		}
		else
		{
			std::string fileNameNoSuffix = txStringUtility::getFileNameNoSuffix(fontFileList[i]);
			mFontList.insert(fileNameNoSuffix, face);
			mFontFileNameList.push_back(fileNameNoSuffix);
		}
	}
	END_FOR_STL(fontFileList);
}

void txFreeTypeFont::destroy()
{
	auto iterFont = mFontList.begin();
	auto iterFontEnd = mFontList.end();
	FOR_STL(mFontList, ; iterFont != iterFontEnd; ++iterFont)
	{
		FT_Done_Face(iterFont->second);
	}
	END_FOR_STL(mFontList);
	mFontList.clear();
	FT_Done_FreeType(mLibrary);
}

bool txFreeTypeFont::getFontBitmapData(const std::string& fontName, const wchar_t& text, unsigned char* buffer, const int& width, const int& height, int& textLeft, int& textTop, int& textWidth, int& textHeight)
{
	if (buffer == NULL || width <= 0 || height <= 0)
	{
		return false;
	}
	auto iterFont = mFontList.find(fontName);
	if (iterFont == mFontList.end())
	{
		ENGINE_ERROR("error : can not find font %s!", fontName.c_str());
		return false;
	}

	FT_Face face = iterFont->second;
 
	//FreeType 2 uses size objects to model all information related to a given character size for a given face.
	FT_Error error = FT_Set_Pixel_Sizes(face, width, height);
	//FT_Error error = FT_Set_Char_Size(face, width * height, 0, 72, 0);
	if (error)
	{
		ENGINE_ERROR("error : an error occurs when trying to set the pixel size to a value");
		return false;
	}

	//5. Loading a Glyph Image  
	//Converting a Character Code Into a Glyph Index  
	FT_UInt charIndex = FT_Get_Char_Index(face, text);
	//Once you have a glyph index, you can load the corresponding glyph image  
	error = FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);
	if (error)
	{
		ENGINE_ERROR("error : an error occurs when trying to load the corresponding glgyh image");
		return false;
	}

	//6. Simple Text Rendering  
	error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
	if (error)
	{
		ENGINE_ERROR("error : an error occurs when trying to render glyph");
		return false;
	}

	FT_Glyph glyph;
	error = FT_Get_Glyph(face->glyph, &glyph);
	if (error)
	{
		ENGINE_ERROR("error : get glyph error");
		return false;
	}

	//convert glyph to bitmap with 256 gray   
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;
	textLeft = bitmap_glyph->left;
	textTop = bitmap_glyph->top;
	textWidth = txMath::getMin((int)bitmap.width, width);
	textHeight = txMath::getMin((int)bitmap.rows, height);
	for (int i = 0; i < textHeight; ++i)
	{
		for (int j = 0; j < textWidth; ++j)
		{
			buffer[i * width * 4 + j * 4 + 0] = bitmap.buffer[i * bitmap.width + j];
			buffer[i * width * 4 + j * 4 + 1] = bitmap.buffer[i * bitmap.width + j];
			buffer[i * width * 4 + j * 4 + 2] = bitmap.buffer[i * bitmap.width + j];
			buffer[i * width * 4 + j * 4 + 3] = bitmap.buffer[i * bitmap.width + j];
		}
	}
	FT_Done_Glyph(glyph);
	return true;
}
