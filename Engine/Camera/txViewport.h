#ifndef _TX_VIEWPORT_H_
#define _TX_VIEWPORT_H_

class txCamera;
class txViewport
{
public:
	txViewport(txCamera* camera);
	virtual ~txViewport(){}
	// 设置视口
	void setViewport(const int& startX, const int& startY, const int& width, const int& height);
	// 应用视口
	void applyViewport();
protected:
	int mWidth;
	int mHeight;
	int mStartX;
	int mStartY;
	txCamera* mCamera;
};

#endif