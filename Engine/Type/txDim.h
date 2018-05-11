#ifndef _TX_DIM_H_
#define _TX_DIM_H_

class txDim
{
public:
	txDim()
	:
	mRelative(0.0f),
	mAbsolute(0)
	{}
	txDim(const float& relative, const int& absolute)
	{
		mRelative = relative;
		mAbsolute = absolute;
	}
	virtual ~txDim(){}
	txDim operator+(const txDim& d) const
	{
		return txDim(mRelative + d.mRelative, mAbsolute + d.mAbsolute);
	}
	txDim operator-(const txDim& d) const
	{
		return txDim(mRelative - d.mRelative, mAbsolute - d.mAbsolute);
	}
	txDim operator*(const float& d) const
	{
		return txDim(mRelative * d, (int)(mAbsolute * d));
	}
	txDim operator/(const float& d) const
	{
		return txDim(mRelative / d, (int)(mAbsolute / d));
	}
	const txDim& operator+=(const txDim& d)
	{
		mRelative += d.mRelative;
		mAbsolute += d.mAbsolute;
		return *this;
	}
	const txDim& operator-=(const txDim& d)
	{
		mRelative -= d.mRelative;
		mAbsolute -= d.mAbsolute;
		return *this;
	}
	const txDim& operator*=(const float& d)
	{
		mRelative *= d;
		mAbsolute = (int)(mAbsolute * d);
		return *this;
	}
	const txDim& operator/=(const float& d)
	{
		mRelative /= d;
		mAbsolute = (int)(mAbsolute / d);
		return *this;
	}
public:
    float mRelative;	//相对坐标
    int mAbsolute;		//绝对坐标
	static txDim ZERO;
	static txDim IDENTITY;
};

#endif