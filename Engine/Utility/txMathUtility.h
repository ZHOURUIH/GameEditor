#ifndef _TX_MATH_UTILITY_H_
#define _TX_MATH_UTILITY_H_

#include "txEngineDefine.h"
#include "txComplex.h"
#include "txDim.h"
#include "txPoint.h"

class txMath
{
public:
	static const float MATH_PI;
	static const float MIN_DELTA;
	static const VECTOR3 AXIS_X;
	static const VECTOR3 AXIS_Y;
	static const VECTOR3 AXIS_Z;
	static const VECTOR3 VEC3_ZERO;
	static const VECTOR2 VEC2_ZERO;
	static const VECTOR3 VEC3_ONE;
	static const VECTOR2 VEC2_ONE;
	static const VECTOR3 COLOUR_BLACK;
	static const VECTOR3 COLOUR_WHITE;
	static const VECTOR3 COLOUR_RED;
	static const VECTOR3 COLOUR_GREEN;
	static const VECTOR3 COLOUR_BLUE;
	static const VECTOR3 COLOUR_YELLOW;
	static const VECTOR3 COLOUR_PURPLE;
	static const VECTOR3 COLOUR_ORANGE;
	static const VECTOR3 COLOUR_PINK;
	static const MATRIX3 MAT3_IDENTITY;
	static const MATRIX4 MAT4_IDENTITY;
	static const QUATERNION QUAT_IDENTITY;

	static MATRIX4 scale(const MATRIX4& matrix, const VECTOR3& vec3){return glm::scale(matrix, vec3);}
	static MATRIX4 translate(const MATRIX4& matrix, const VECTOR3& vec3){return glm::translate(matrix, vec3);}
	static QUATERNION rotate(const QUATERNION& q, const float& radian, const VECTOR3& axis){return glm::rotate(q, radian, axis);}
	static float dot(const VECTOR3& vec1, const VECTOR3& vec2){return glm::dot(vec1, vec2);}
	static VECTOR3 cross(const VECTOR3& vec1, const VECTOR3& vec2){return glm::cross(vec1, vec2);}
	static MATRIX4 lookAt(const VECTOR3& pos, const VECTOR3& lookPoint, const VECTOR3& up){return glm::lookAt(pos, lookPoint, up);}
	static float getYaw(const QUATERNION& q){return glm::yaw(q);}
	static float getPitch(const QUATERNION& q){return glm::pitch(q);}
	static float getRoll(const QUATERNION& q){return glm::roll(q);}
	static MATRIX4 ortho(const float& left, const float& right, const float& bottom, const float& top, const float& zNear, const float& zFar)
	{
		return glm::ortho(left, right, bottom, top, zNear, zFar);
	}
	static MATRIX4 perspective(const float& fovy, const float& aspect, const float& zNear, const float& zFar){return glm::perspective(fovy, aspect, zNear, zFar);}
	static QUATERNION slerp(const QUATERNION& q1, const QUATERNION& q2, const float& a){return glm::slerp(q1, q2, a);}
	static MATRIX4 inverse(const MATRIX4& mat){return glm::inverse(mat);}
	static MATRIX3 inverse(const MATRIX3& mat){return glm::inverse(mat);}
	static QUATERNION inverse(const QUATERNION& q){return glm::inverse(q);}
	static const float* toPtr(const MATRIX3& value){return glm::value_ptr(value);}
	static const float* toPtr(const MATRIX4& value){return glm::value_ptr(value);}
	static const float* toPtr(const VECTOR4& value){return glm::value_ptr(value);}
	static const float* toPtr(const VECTOR3& value){return glm::value_ptr(value);}
	static const float* toPtr(const VECTOR2& value){return glm::value_ptr(value);}
	static const float* toPtr(const QUATERNION& value){return glm::value_ptr(value);}
	static bool intersectRayTriangle(const VECTOR3& orig, const VECTOR3& dir,const VECTOR3& v0, const VECTOR3& v1, const VECTOR3& v2, VECTOR3& baryPosition)
	{
		return glm::intersectRayTriangle(orig, dir, v0, v1, v2, baryPosition);
	}
	static VECTOR3 getDirectionFromEulerAngle(const VECTOR3& eulerAngle);
	static void getYawPitchFromDirection(const VECTOR3& dir, float& fYaw, float& fPitch);
	static MATRIX3 getRotationMatrixFromDirection(const VECTOR3& dir);
	static QUATERNION getQuaternionFromDirection(const VECTOR3& dir);
	static void checkInt(float& value, const float& precision = MIN_DELTA); // 判断传入的参数是否已经接近于整数,如果接近于整数,则设置为整数
	static bool isFloatZero(const float& value, const float& precision = MIN_DELTA){return value >= -precision && value <= precision;}
	static bool isFloatEqual(const float& value1, const float& value2, const float& precision = MIN_DELTA){return isFloatZero(value1 - value2, precision);}
	static bool isVector3Zero(const VECTOR3& v1){return isFloatZero(v1.x) && isFloatZero(v1.y) && isFloatZero(v1.z);}
	static bool isVector3Equal(const VECTOR3& v1, const VECTOR3& v2){return isFloatZero(v1.x - v2.x) && isFloatZero(v1.y - v2.y) && isFloatZero(v1.z - v2.z);}
	static bool isVector2Zero(const VECTOR2& v1){return isFloatZero(v1.x) && isFloatZero(v1.y);}
	static bool isVector2Equal(const VECTOR2& v1, const VECTOR2& v2){return isFloatZero(v1.x - v2.x) && isFloatZero(v1.y - v2.y);}
	static bool isMatrix3Equal(const MATRIX3& m1, const MATRIX3& m2);
	static bool isMatrix4Equal(const MATRIX4& m1, const MATRIX4& m2);
	// 得到比value大的第一个pow的n次方的数
	static int getGreaterPowerValue(const int& value, const int& pow);
	// 得到数轴上浮点数右边的第一个整数,向上取整
	static int getForwardInt(const float& value);
	template<typename T>
	static void clamp(T& value, const T& minValue, const T& maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
		else if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	static void clampMin(T& value, const T& minValue)
	{
		if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	static void clampMax(T& value, const T& maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
	}
	static float randomFloat(const float& minFloat, const float& maxFloat)
	{
		float percent = (rand() % (1000 + 1)) / 1000.0f;
		return percent * (maxFloat - minFloat) + minFloat;
	}
	static int randomInt(const int& minInt, const int& maxInt)
	{
		if (minInt >= maxInt)
		{
			return minInt;
		}
		return rand() % (maxInt - minInt + 1) + minInt;
	}
	static float angleToRadian(const float& angle)
	{
		return angle * 3.14159f / 180.0f;
	}
	static float radianToAngle(const float& radian)
	{
		return radian * 180.0f / 3.14159f;
	}
	static void clampValue(float& value, const float& min, const float& max, const float& cycle)
	{
		while (value < min)
		{
			value += cycle;
		}
		while (value > max)
		{
			value -= cycle;
		}
	}
	static void clampAngle(float& angle, const float& min, const float& max, const float& pi)
	{
		clampValue(angle, min, max, pi * 2.0f);
	}
	static void adjustRadian180(float& radianAngle)
	{
		clampAngle(radianAngle, -MATH_PI, MATH_PI, MATH_PI);
	}
	static void adjustAngle180(float& radianAngle)
	{
		clampAngle(radianAngle, -180.0f, 180.0f, 180.0f);
	}
	static void adjustRadian180(VECTOR3& radianAngle)
	{
		adjustRadian180(radianAngle.x);
		adjustRadian180(radianAngle.y);
		adjustRadian180(radianAngle.z);
	}
	static void adjustAngle180(VECTOR3& radianAngle)
	{
		adjustAngle180(radianAngle.x);
		adjustAngle180(radianAngle.y);
		adjustAngle180(radianAngle.z);
	}
	static void adjustRadian360(float& radianAngle)
	{
		clampAngle(radianAngle, 0.0f, MATH_PI * 2.0f, MATH_PI);
	}
	static void adjustAngle360(float& radianAngle)
	{
		clampAngle(radianAngle, 0.0f, 360.0f, 180.0f);
	}
	static void adjustAngle360(VECTOR3& radianAngle)
	{
		adjustAngle360(radianAngle.x);
		adjustAngle360(radianAngle.y);
		adjustAngle360(radianAngle.z);
	}
	static void resetRight(MATRIX3& rotate); // 调整旋转使right平行于X-Z平面
	// 给定一段圆弧,以及圆弧圆心角的百分比,计算对应的圆弧上的一个点以及该点的切线方向
	static void getPosOnArc(const VECTOR3& circleCenter, const VECTOR3& startArcPos, const VECTOR3& endArcPos, float anglePercent, VECTOR3& pos, VECTOR3& tangencyDir);
	static float getAngleFromVectorToVector(const VECTOR2& from, const VECTOR2& to);	// 计算从from到to的顺时针角度,范围是-180到180
	static float getAngleBetweenVector(const VECTOR3& vec1, const VECTOR3& vec2);		// 得到两个向量的夹角,范围是0到180
	// 计算点到线的距离
	static float getDistanceToLine(const VECTOR3& point, const VECTOR3& start, const VECTOR3& end)
	{
		return getLength(point - getProjectPoint(point, start, end));
	}
	// 计算点在线上的投影
	static VECTOR3 getProjectPoint(const VECTOR3& point, const VECTOR3& start, const VECTOR3& end)
	{
		// 计算出点到线一段的向量在线上的投影
		VECTOR3 projectOnLine = getProjection(point - start, end - start);
		// 点到线垂线的交点
		return start + projectOnLine;
	}
	// 点在线上的投影是否在线段范围内
	static bool isPointProjectOnLine(const VECTOR3& point, const VECTOR3& start, const VECTOR3& end)
	{
		float dis0 = getLength(point - start);
		float dis1 = getLength(point - end);
		VECTOR3 point0 = start;
		VECTOR3 point1 = end;
		if (dis0 > dis1)
		{
			point0 = end;
			point1 = start;
		}
		return getAngleBetweenVector(point - point0, point1 - point0) <= MATH_PI / 2.0f;
	}
	// 计算一个向量在另一个向量上的投影
	static VECTOR3 getProjection(const VECTOR3& v1, const VECTOR3& v2)
	{
		return normalize(v2) * getLength(v1) * std::cos(getAngleBetweenVector(v1, v2));
	}
	// 根据入射角和法线得到反射角
	static VECTOR3 getReflection(VECTOR3 inRay, VECTOR3 normal)
	{
		inRay = normalize(inRay);
		normal = normalize(normal);
		return inRay - 2 * (dot(inRay, normal)) * normal;
	}
	static bool isInRange(const int& value, const int& range0, const int& range1)
	{
		return value >= getMin(range0, range1) && value <= getMax(range0, range1);
	}
	static bool isInRange(const float& value, const float& range0, const float& range1)
	{
		return value >= getMin(range0, range1) && value <= getMax(range0, range1);
	}
	static bool isInRange(const VECTOR3& value, const VECTOR3& point0, const VECTOR3& point1, const bool& ignoreY = true)
	{
		return isInRange(value.x, point0.x, point1.x) && isInRange(value.z, point0.z, point1.z) &&
			(ignoreY || isInRange(value.y, point0.y, point1.y));
	}
	template<typename T>
	static T getMin(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
	template<typename T>
	static T getMax(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	template<typename T>
	static float lerp(const T& start, const T& end, const float& t)
	{
		clamp(t, 0.0f, 1.0f);
		return start + (end - start) * t;
	}
	static float inverseLerp(const float& a, const float& b, const float& value)
	{
		return (value - a) / (b - a);
	}
	static float inverseLerp(const VECTOR2& a, const VECTOR2& b, const VECTOR2& value)
	{
		return getLength(value - a) / getLength(b - a);
	}
	static float inverseLerp(const VECTOR3& a, const VECTOR3& b, const VECTOR3& value)
	{
		return getLength(value - a) / getLength(b - a);
	}
	static bool isVectorZero(const VECTOR2& vec)
	{
		return isFloatZero(vec.x) && isFloatZero(vec.y);
	}
	static bool isVectorZero(const VECTOR3& vec)
	{
		return isFloatZero(vec.x) && isFloatZero(vec.y) && isFloatZero(vec.z);
	}
	// 使用一个4阶矩阵去变换一个三维向量
	static VECTOR3 transformVector3(const VECTOR3& vec, const MATRIX4& transMat4, const bool& divideW = false)
	{
		if (divideW)
		{
			VECTOR4 ret = transMat4 * VECTOR4(vec, 1.0f);
			return VECTOR3(ret.x / ret.w, ret.y / ret.w, ret.z / ret.w);
		}
		else
		{
			return VECTOR3(transMat4 * VECTOR4(vec, 1.0f));
		}
	}
	// 使用一个3阶矩阵去变换一个三维向量
	static VECTOR3 rotateVector3(const VECTOR3& vec, const MATRIX3& transMat3){return transMat3 * vec;}
	// 使用一个四元数去旋转一个三维向量
	static VECTOR3 rotateVector3(const VECTOR3& vec, const QUATERNION& transQuat){return vec * transQuat;}
	// 求向量水平顺时针旋转一定角度后的向量,角度范围是-MATH_PI 到 MATH_PI
	static VECTOR3 rotateVector3(const VECTOR3& vec, const float& angle)
	{
		VECTOR3 temp = vec;
		temp.y = 0.0f;
		float tempLength = txMath::getLength(temp);
		float questAngle = getAngleFromVector(temp);
		questAngle += angle;
		adjustAngle180(questAngle);
		temp = getVectorFromAngle(questAngle) * tempLength;
		temp.y = vec.y;
		return temp;
	}
	static VECTOR3 normalize(const VECTOR3& vec3)
	{
		float length = txMath::getLength(vec3);
		if (isFloatZero(length))
		{
			return VEC3_ZERO;
		}
		if (isFloatEqual(length, 1.0f))
		{
			return vec3;
		}
		return VECTOR3(vec3.x / length, vec3.y / length, vec3.z / length);
	}
	static VECTOR2 normalize(const VECTOR2& vec2)
	{
		float length = txMath::getLength(vec2);
		if (0.0f == length)
		{
			return VEC2_ZERO;
		}
		if (isFloatEqual(length, 1.0f))
		{
			return vec2;
		}
		return VECTOR2(vec2.x / length, vec2.y / length);
	}
	static QUATERNION normalize(const QUATERNION& q){return glm::normalize(q);}
	static float getLength(const VECTOR3& vec){return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);}
	static float getLength(const VECTOR2& vec){return sqrt(vec.x * vec.x + vec.y * vec.y);}
	static bool vectorGreater(const VECTOR3& vec1, const VECTOR3& vec2){return vec1.z > vec2.z && vec1.y > vec2.y && vec1.x > vec2.x;}
	static bool vectorLess(const VECTOR3& vec1, const VECTOR3& vec2){return vec1.z < vec2.z && vec1.y < vec2.y && vec1.x < vec2.x;}
	static bool vectorGreater(const VECTOR2& vec1, const VECTOR2& vec2){return vec1.x > vec2.x && vec1.y > vec2.y;}
	static bool vectorLess(const VECTOR2& vec1, const VECTOR2& vec2){return vec1.x < vec2.x && vec1.y < vec2.y;}
	// 求从z轴到指定向量的水平方向上的顺时针角度,角度范围是-MATH_PI 到 MATH_PI
	static float getAngleFromVector(const VECTOR3& vec)
	{
		VECTOR3 projection(vec.x, 0.0f, vec.z);
		projection = txMath::normalize(projection);
		float angle = std::acos(projection.z);
		if (projection.x > 0.0f)
		{
			angle = -angle;
		}
		adjustAngle180(angle);
		return angle;
	}
	// 求Z轴顺时针旋转一定角度后的向量,角度范围是-MATH_PI 到 MATH_PI
	static VECTOR3 getVectorFromAngle(const float& angle)
	{
		VECTOR3 temp;
		temp.x = -std::sin(angle);
		temp.y = 0.0f;
		temp.z = std::cos(angle);
		return temp;
	}
	// 设置一个向量的长度,不改变方向
	static void setLength(VECTOR3& vec, const float& length){vec = normalize(vec) * length;}
	// 设置一个向量的长度,不改变方向
	static void setLength(VECTOR2& vec, const float& length){vec = normalize(vec) * length;}
	// 从矩阵中获得位移
	static VECTOR3 getMatrixPosition(const MATRIX4& matrix4){return VECTOR3(matrix4[3].x, matrix4[3].y, matrix4[3].z);}
	static void setMatrixPosition(MATRIX4& matrix4, const VECTOR3& position)
	{
		matrix4[3].x = position.x;
		matrix4[3].y = position.y;
		matrix4[3].z = position.z;
	}
	// 从矩阵中获得旋转
	static MATRIX3 getMatrixRotation(const MATRIX4& matrix4);
	// 设置矩阵的旋转,但是不改变缩放和位移
	static void setMatrixRotation(MATRIX4& matrix4, const MATRIX3& rotation);
	// 从矩阵中获得缩放
	static VECTOR3 getMatrixScale(const MATRIX4& matrix4)
	{
		VECTOR3 vec1(matrix4[0].x, matrix4[0].y, matrix4[0].z);
		VECTOR3 vec2(matrix4[1].x, matrix4[1].y, matrix4[1].z);
		VECTOR3 vec3(matrix4[2].x, matrix4[2].y, matrix4[2].z);
		float x = txMath::getLength(vec1);
		float y = txMath::getLength(vec2);
		float z = txMath::getLength(vec3);
		return VECTOR3(x, y, z);
	}
	// 设置矩阵中的缩放,但是不改变旋转和位移
	static void setMatrixScale(MATRIX4& matrix4, const VECTOR3& scale);
	// 从一个旋转矩阵中获得航向角
	static float getMatrix3Yaw(const MATRIX3& rot){return matrix3ToEulerAngle(rot).x;}
	// 从一个旋转矩阵中获得俯仰角
	static float getMatrix3Pitch(const MATRIX3& rot){return matrix3ToEulerAngle(rot).y;}
	// 从一个旋转矩阵中获得滚动角
	static float getMatrix3Roll(const MATRIX3& rot){return matrix3ToEulerAngle(rot).z;}
	static MATRIX3 getYawMatrix3(const float& angle)
	{
		float cosY = std::cos(angle);
		float sinY = std::sin(angle);
		MATRIX3 rot;
		rot[0][0] = cosY;
		rot[0][1] = 0.0f;
		rot[0][2] = sinY;
		rot[1][0] = 0.0f;
		rot[1][1] = 1.0f;
		rot[1][2] = 0.0f;
		rot[2][0] = -sinY;
		rot[2][1] = 0.0f;
		rot[2][2] = cosY;
		return rot;
	}
	static MATRIX3 getPitchMatrix3(const float& angle)
	{
		float cosZ = std::cos(angle);
		float sinZ = std::sin(angle);
		MATRIX3 rot;
		rot[0][0] = 1.0f;
		rot[0][1] = 0.0f;
		rot[0][2] = 0.0f;
		rot[1][0] = 0.0f;
		rot[1][1] = cosZ;
		rot[1][2] = -sinZ;
		rot[2][0] = 0.0f;
		rot[2][1] = sinZ;
		rot[2][2] = cosZ;
		return rot;
	}
	static MATRIX3 getRollMatrix3(const float& angle)
	{
		float cosX = std::cos(angle);
		float sinX = std::sin(angle);
		MATRIX3 rot;
		rot[0][0] = cosX;
		rot[0][1] = -sinX;
		rot[0][2] = 0.0f;
		rot[1][0] = sinX;
		rot[1][1] = cosX;
		rot[1][2] = 0.0f;
		rot[2][0] = 0.0f;
		rot[2][1] = 0.0f;
		rot[2][2] = 1.0f;
		return rot;
	}
	// 得到一个俯仰角旋转四元数
	static QUATERNION getYawQuaternion(const float& radian){return rotate(QUATERNION(), radian, AXIS_Y);}
	// 得到一个俯仰角旋转四元数
	static QUATERNION getPitchQuaternion(const float& radian){return rotate(QUATERNION(), radian, AXIS_X);}
	// 得到一个俯仰角旋转四元数
	static QUATERNION getRollQuaternion(const float& radian){return rotate(QUATERNION(), radian, AXIS_Z);}
	// 物体1去碰撞物体2,计算新的速度
	static void collideSpeed(const VECTOR3& pos1, VECTOR3 v1, const float& m1, const VECTOR3& pos2, VECTOR3 v2, const float& m2, VECTOR3& newV1, VECTOR3& newV2);
	// 欧拉角与旋转矩阵的互相转换
	// 根据欧拉角获得旋转矩阵,yaw,pitch,roll分量可能与其他的有所不同,但是计算结果是正确的
	static MATRIX3 eulerAngleToMatrix3(const VECTOR3& angle);
	// 旋转矩阵转换为欧拉角,由于不同的旋转方式也能达到同样的旋转结果,所以欧拉角计算出的矩阵,再转回欧拉角时可能值会不一样
	//static VECTOR3 matrix3ToEulerAngle(MATRIX3& rot);
	static VECTOR3 matrix3ToEulerAngle(const MATRIX3& rot);
	// 欧拉角与四元数的互相转换
	static QUATERNION eulerAngleToQuat(const VECTOR3& eulerAngle)
	{
		QUATERNION q = QUAT_IDENTITY;
		q = rotate(q, eulerAngle.x, AXIS_Y);
		q = rotate(q, eulerAngle.y, AXIS_X);
		q = rotate(q, eulerAngle.z, AXIS_Z);
		return q;
	}
	// 该函数未测试
	static QUATERNION eulerAngleToQuaternion(const VECTOR3& eulerAngle)
	{
		float cosHalfX = std::cos(eulerAngle.x / 2.0f);
		float sinHalfX = std::sin(eulerAngle.x / 2.0f);
		float cosHalfY = std::cos(eulerAngle.y / 2.0f);
		float sinHalfY = std::sin(eulerAngle.y / 2.0f);
		float cosHalfZ = std::cos(eulerAngle.z / 2.0f);
		float sinHalfZ = std::sin(eulerAngle.z / 2.0f);
		QUATERNION q;
		q.w = cosHalfX * cosHalfY * cosHalfZ + sinHalfX * sinHalfY * sinHalfZ;
		q.x = cosHalfX * sinHalfY * cosHalfZ + sinHalfX * cosHalfY * sinHalfZ;
		q.y = sinHalfX * cosHalfY * cosHalfZ - cosHalfX * sinHalfY * sinHalfZ;
		q.z = cosHalfX * cosHalfY * sinHalfZ - sinHalfX * sinHalfY * cosHalfZ;
		return q;
	}
	static VECTOR3 quaternionToEulerAngle(const QUATERNION& q)
	{
		matrix3ToEulerAngle(quatToMatrix3(q));
	}
	// 旋转矩阵与四元数的互相转换
	static MATRIX3 quatToMatrix3(const QUATERNION& q){return glm::mat3_cast(q);}
	// 该函数未测试
	static QUATERNION matrix3ToQuat(const MATRIX3& rot);
	// 将音频信号从时域转换到频域,pcmData和frequencyList的长度都为dataCount,dataCount应该是2的n次方,不足的补0
	static void getFrequencyZone(const short* pcmData, const int& dataCount, short* frequencyList);
	static void fft(txComplex* x, const int& count);
	static void ifft(txComplex* x, const int& count);
	static int txMath::pcm_db_count(const short* ptr, const int& size);	// 根据一段音频采样信号,计算音量大小
	// 将一个复合坐标转换为像素坐标
	static VECTOR2 pointToVector2(const txPoint& point, const VECTOR2& parentSize)
	{
		VECTOR2 position;
		position.x = parentSize.x * point.x.mRelative + point.x.mAbsolute;
		position.y = parentSize.y * point.y.mRelative + point.y.mAbsolute;
		return position;
	}
	static txPoint vector2DirectToPoint(const VECTOR2& pos){return txPoint(txDim(0.0f, (int)pos.x), txDim(0.0f, (int)pos.y));}
	// 将point转化为相对坐标
	static txPoint pointToRelotivePoint(const txPoint& point, const VECTOR2& parentSize);
	// 将point转换为绝对坐标
	static txPoint pointToAbsolutePoint(const txPoint& point, const VECTOR2& parentSize);
	// 将表达式str中的keyword替换为replaceValue,然后计算str的值,返回值表示str中是否有被替换的值,str只能是算术表达式
	static bool replaceKeywordAndCalculate(std::string& str, const std::string& keyword, const int& replaceValue, const bool& floatOrInt);
	// 将表达式str中的所有\\()包含的部分中的keyword替换为keyValue,并且计算包含的表达式,返回值表示str中是否有被替换的部分,str可以是任意表达式
	static bool replaceStringKeyword(std::string& str, const std::string& keyword, const int& keyValue, const bool& floatOrInt);
	static float powerFloat(const float& f, int p);
	static float calculateFloat(std::string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(std::string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	// 秒数转换为分数和秒数
	static void secondsToMinutesSeconds(int seconds, int& outMin, int& outSec);
	static void secondsToHoursMinutesSeconds(int seconds, int& outHour, int& outMin, int& outSec);
	template<typename T>
	static void swap(T& value0, T& value1)
	{
		T temp = value0;
		value0 = value1;
		value1 = temp;
	}
	template<typename T>
	static T getSign(const T& value)
	{
		if (value > (T)0)
		{
			return (T)1;
		}
		else if (value < (T)0)
		{
			return (T)-1;
		}
		else
		{
			return (T)0;
		}
	}
	float HueToRGB(float v1, float v2, float vH);
	VECTOR3 RGBtoHSL(const VECTOR3& rgb);
	VECTOR3 HSLtoRGB(const VECTOR3& hsl);
};

#endif