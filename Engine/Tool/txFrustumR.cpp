#include "txFrustumR.h"

#define ANG2RAD 3.14159265358979323846/360.0

#define m(col, row)  m[row * 4 + col]

void txFrustumR::setFrustum(float *m)
{
	pl[NEARP].setCoefficients(m(2, 0) + m(3, 0), m(2, 1) + m(3, 1), m(2, 2) + m(3, 2), m(2, 3) + m(3, 3));
	pl[FARP].setCoefficients(-m(2, 0) + m(3, 0), -m(2, 1) + m(3, 1), -m(2, 2) + m(3, 2), -m(2, 3) + m(3, 3));
	pl[BOTTOM].setCoefficients(m(1, 0) + m(3, 0), m(1, 1) + m(3, 1), m(1, 2) + m(3, 2), m(1, 3) + m(3, 3));
	pl[TOP].setCoefficients(-m(1, 0) + m(3, 0), -m(1, 1) + m(3, 1), -m(1, 2) + m(3, 2), -m(1, 3) + m(3, 3));
	pl[LEFT].setCoefficients(m(0, 0) + m(3, 0), m(0, 1) + m(3, 1), m(0, 2) + m(3, 2), m(0, 3) + m(3, 3));
	pl[RIGHT].setCoefficients(-m(0, 0) + m(3, 0), -m(0, 1) + m(3, 1), -m(0, 2) + m(3, 2), -m(0, 3) + m(3, 3));
}
#undef m

void txFrustumR::setCamInternals(const float& angle, const float& ratio, const float& nearD, const float& farD)
{
	// store the information
	this->ratio = ratio;
	this->angle = angle * (float)(ANG2RAD);
	this->nearD = nearD;
	this->farD = farD;

	// compute width and height of the near and far plane sections
	tang = tan(this->angle);

	nh = nearD * tang;
	nw = nh * ratio;

	fh = farD * tang;
	fw = fh * ratio;
}

void txFrustumR::setCamDef(const VECTOR3& p, const VECTOR3& xDir, const VECTOR3& yDir, const VECTOR3& zDir)
{
	VECTOR3 dir, nc, fc;

	camPos = p;

	// compute the Z axis of camera
	Z = zDir;
	Z = txMath::normalize(Z);

	// X axis of camera of given "up" vector and Z axis
	X = xDir;
	X = txMath::normalize(X);

	// the real "up" vector is the cross product of Z and X
	Y = yDir;
	Y = txMath::normalize(Y);

	// compute the center of the near and far planes
	nc = p - Z * nearD;
	fc = p - Z * farD;

	// compute the 8 corners of the frustum
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;

	// compute the six planes
	// the function set3Points asssumes that the points
	// are given in counter clockwise order
	pl[TOP].set3Points(ntr,ntl,ftl);
	pl[BOTTOM].set3Points(nbl,nbr,fbr);
	pl[LEFT].set3Points(ntl,nbl,fbl);
	pl[RIGHT].set3Points(nbr,ntr,fbr);

	pl[NEARP].setNormalAndPoint(-Z,nc);
	pl[FARP].setNormalAndPoint(Z,fc);

	VECTOR3 aux, normal;

	aux = (nc + Y*nh) - p;
	normal = aux * X;
	pl[TOP].setNormalAndPoint(normal,nc+Y*nh);

	aux = (nc - Y*nh) - p;
	normal = X * aux;
	pl[BOTTOM].setNormalAndPoint(normal,nc-Y*nh);

	aux = (nc - X*nw) - p;
	normal = aux * Y;
	pl[LEFT].setNormalAndPoint(normal,nc-X*nw);

	aux = (nc + X*nw) - p;
	normal = Y * aux;
	pl[RIGHT].setNormalAndPoint(normal,nc+X*nw);
}

void txFrustumR::setCamDef(const VECTOR3& p, const VECTOR3& l, const VECTOR3& u)
{
	VECTOR3 dir, nc, fc;

	camPos = p;

	// compute the Z axis of camera
	Z = p - l;
	Z = txMath::normalize(Z);

	// X axis of camera of given "up" vector and Z axis
	X = u * Z;
	X = txMath::normalize(X);

	// the real "up" vector is the cross product of Z and X
	Y = Z * X;

	// compute the center of the near and far planes
	nc = p - Z * nearD;
	fc = p - Z * farD;

	// compute the 8 corners of the frustum
	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;

	// compute the six planes
	// the function set3Points asssumes that the points
	// are given in counter clockwise order
	pl[TOP].set3Points(ntr,ntl,ftl);
	pl[BOTTOM].set3Points(nbl,nbr,fbr);
	pl[LEFT].set3Points(ntl,nbl,fbl);
	pl[RIGHT].set3Points(nbr,ntr,fbr);
//	pl[NEARP].set3Points(ntl,ntr,nbr);
//	pl[FARP].set3Points(ftr,ftl,fbl);

	pl[NEARP].setNormalAndPoint(-Z,nc);
	pl[FARP].setNormalAndPoint(Z,fc);

	VECTOR3 aux,normal;

	aux = (nc + Y*nh) - p;
	normal = aux * X;
	pl[TOP].setNormalAndPoint(normal,nc+Y*nh);

	aux = (nc - Y*nh) - p;
	normal = X * aux;
	pl[BOTTOM].setNormalAndPoint(normal,nc-Y*nh);

	aux = (nc - X*nw) - p;
	normal = aux * Y;
	pl[LEFT].setNormalAndPoint(normal,nc-X*nw);

	aux = (nc + X*nw) - p;
	normal = Y * aux;
	pl[RIGHT].setNormalAndPoint(normal,nc+X*nw);
}

int txFrustumR::pointInFrustum(const VECTOR3& p)
{
	float pcz, pcx, pcy, aux;

	// compute vector from camera position to p
	VECTOR3 v = p - camPos;

	// compute and test the Z coordinate
	pcz = txMath::dot(v, -Z);
	if (pcz > farD || pcz < nearD)
	{
		return OUTSIDE;
	}
	// compute and test the Y coordinate
	pcy = txMath::dot(v, Y);
	aux = pcz * tang;
	if (pcy > aux || pcy < -aux)
	{
		return OUTSIDE;
	}
	// compute and test the X coordinate
	pcx = txMath::dot(v, X);
	aux = aux * ratio;
	if (pcx > aux || pcx < -aux)
	{
		return OUTSIDE;
	}
	return INSIDE;
}

int txFrustumR::sphereInFrustum(const VECTOR3& p, const float& radius)
{
	float d1, d2;
	float az, ax, ay, zz1, zz2;
	int result = INSIDE;

	VECTOR3 v = p - camPos;

	az = txMath::dot(v, -Z);
	if (az > farD + radius || az < nearD - radius)
	{
		return(OUTSIDE);
	}
	ax = txMath::dot(v, X);
	zz1 = az * tang * ratio;
	d1 = sphereFactorX * radius;
	if (ax > zz1 + d1 || ax < -zz1 - d1)
	{
		return(OUTSIDE);
	}

	ay = txMath::dot(v, Y);
	zz2 = az * tang;
	d2 = sphereFactorY * radius;
	if (ay > zz2 + d2 || ay < -zz2 - d2)
	{
		return(OUTSIDE);
	}

	if (az > farD - radius || az < nearD + radius)
	{
		result = INTERSECT;
	}
	if (ay > zz2 - d2 || ay < -zz2 + d2)
	{
		result = INTERSECT;
	}
	if (ax > zz1 - d1 || ax < -zz1 + d1)
	{
		result = INTERSECT;
	}
	return(result);
}

int txFrustumR::boxInFrustum(const txAABox& b) 
{
	int result = INSIDE;
	//for each plane do ...
	for(int i = 0; i < 6; ++i) 
	{
		// is the positive vertex outside?
		if (pl[i].distance(b.getVertexP(pl[i].mNormal)) < 0)
		{
			return OUTSIDE;
		}
		// is the negative vertex outside?
		else if (pl[i].distance(b.getVertexN(pl[i].mNormal)) < 0)
		{
			result = INTERSECT;
		}
	}
	return result;
}