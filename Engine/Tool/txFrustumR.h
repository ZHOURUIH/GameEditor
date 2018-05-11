#ifndef _TX_FRUSTUM_R_
#define _TX_FRUSTUM_R_

#include "txPlane.h"
#include "txAABox.h"

class txFrustumR
{
private:
	enum 
	{
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

public:
	enum {OUTSIDE, INTERSECT, INSIDE};
	txPlane pl[6];
	VECTOR3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr,X,Y,Z,camPos;
	float nearD, farD, ratio, angle;
	float sphereFactorX, sphereFactorY;
	float tang;
	float nw,nh,fw,fh;
public:
	txFrustumR(){}
	virtual ~txFrustumR(){}

	void setFrustum(float* m);
	void setCamInternals(const float& angle, const float& radius, const float& nearD, const float& farD);
	void setCamDef(const VECTOR3& p, const VECTOR3& l, const VECTOR3& u);
	void setCamDef(const VECTOR3& p, const VECTOR3& xDir, const VECTOR3& yDir, const VECTOR3& zDir);
	int pointInFrustum(const VECTOR3& p);
	int sphereInFrustum(const VECTOR3& p, const float& radius);
	int boxInFrustum(const txAABox&);
};


#endif
