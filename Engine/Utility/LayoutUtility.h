#ifndef _LAYOUT_UTILITY_H_
#define _LAYOUT_UTILITY_H_

#include "txRect.h"
#include "txEngineBase.h"

class txTexture;
class LayoutManager;
class Window;
class LayoutUtility : public txEngineBase
{
public:
	LayoutUtility();
	void init(){}
	static WINDOW_TYPE getWindowTypeFromTypeName(const std::string& typeName)
	{
		auto iter = mTypeNameToWindowTypeMap.find(typeName);
		if (iter != mTypeNameToWindowTypeMap.end())
		{
			return iter->second;
		}
		return WT_MAX;
	}
	static const std::string& getTypeNameFromWindowType(const WINDOW_TYPE& type)
	{
		auto iter = mWindowTypeToTypeNameMap.find(type);
		if (iter != mWindowTypeToTypeNameMap.end())
		{
			return iter->second;
		}
		return EMPTY_STRING;
	}
	static VECTOR2 getWindowPosition(Window* pWindow);
	static VECTOR2 getRelativeWindowPosition(Window* pWindow); // 得到相对于父窗口的像素坐标
	static VECTOR2 getWindowSize(Window* pWindow);
	static txRect getWindowRect(Window* pWindow);
	static VECTOR3 getPickupRayDirection(const VECTOR2& screenPoint);	// 通过屏幕坐标计算鼠标拣选的射线方向
	static const int& getScreenWidth();
	static const int& getScreenHeight();
	static VECTOR2 getScreenSize();
	// 将屏幕像素坐标转换为父窗口中的相对像素坐标,parentPosition时父窗口的屏幕坐标
	static VECTOR2 screenToLocalPosition(const VECTOR2& screenPixel, const VECTOR2& parentPosition) { return screenPixel - parentPosition; }
	// 得到窗口在屏幕中的像素坐标
	static VECTOR2 getWindowPositionInScreen(Window* window);
	// 计算窗口的渲染顶点数据
	// isAbsoluteCenter为真则表示transformCenter中所有分量都是绝对值,以整个屏幕中心为原点的坐标系,不会乘以窗口宽高计算最终的变换中心
	// 为假则表示transformCenter的x,y是相对值,z是绝对值,会乘以窗口宽高并加上窗口位置计算最终的变换中心
	// 一般情况下,当一个窗口中需要渲染多次时,该值设置为true,一个窗口只需要渲染一次时,该值设置为false
	// directVertices为真则表示是计算直接渲染到屏幕上的顶点坐标,为假则表示是计算在实际三维空间中的一个平面的顶点坐标
	static void generateVertices(GLfloat* vertices, const VECTOR2& windowPosition, const VECTOR2& windowSize, const MATRIX4& transform,
		const VECTOR3& transformCenter, const bool& isAbsoluteCenter, const float& fovy, const bool& directVertices);
	// 计算窗口的纹理坐标
	static void generateTexCoord(txTexture* tex, GLfloat* texcoords, SYMMETRY_MODE& symmetryMode, const VECTOR2& texCoord, const VECTOR2& texSize);
	// 计算线框和坐标系顶点坐标
	static void generateWireframeCoordinate(GLfloat* wireframeVertices, GLfloat* coordinateVertices, GLfloat* vertices);
	// 计算UI平面的Z轴距离,screenHeight实际是视口的高度,fovy是构造投影矩阵时的fovy
	static float generateUIClipZ(const int& screenHeight, const float& fovy)
	{
		return screenHeight / 2.0f / std::tan(fovy / 2.0f);
	}
protected:
	static txMap<WINDOW_TYPE, std::string> mWindowTypeToTypeNameMap;
	static txMap<std::string, WINDOW_TYPE> mTypeNameToWindowTypeMap;
};

#endif