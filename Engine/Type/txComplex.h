#ifndef _TX_COMPLEX_H_
#define _TX_COMPLEX_H_

// ¸´Êý
class txComplex
{
public:
	txComplex()
		:
		mReal(0.0f),
		mImg(0.0f)
	{}
	txComplex(float realPart, float imgPart)
		:
		mReal(realPart),
		mImg(imgPart)
	{}
	txComplex& operator= (const txComplex& that)
	{
		mReal = that.mReal;
		mImg = that.mImg;
		return *this;
	}
	txComplex& operator+= (const txComplex& that)
	{
		mReal += that.mReal;
		mImg += that.mImg;
		return *this;
	}
	txComplex& operator-= (const txComplex& that)
	{
		mReal -= that.mReal;
		mImg -= that.mImg;
		return *this;
	}
	txComplex operator+(const txComplex& that)
	{
		txComplex temp(that.mReal + mReal, that.mImg + mImg);
		return temp;
	}
	txComplex operator-(const txComplex& that)
	{
		txComplex temp(that.mReal - mReal, that.mImg - mImg);
		return temp;
	}
public:
	float mReal;
	float mImg;
};

#endif