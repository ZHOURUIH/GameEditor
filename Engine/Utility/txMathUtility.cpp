#include "txMathUtility.h"
#include "txComplex.h"
#include "Utility.h"

const float txMath::MATH_PI = 3.1415926f;
const float txMath::MIN_DELTA = 0.00001f;
const VECTOR3 txMath::AXIS_X = VECTOR3(1.0f, 0.0f, 0.0f);
const VECTOR3 txMath::AXIS_Y = VECTOR3(0.0f, 1.0f, 0.0f);
const VECTOR3 txMath::AXIS_Z = VECTOR3(0.0f, 0.0f, 1.0f);
const VECTOR3 txMath::VEC3_ZERO = VECTOR3(0.0f, 0.0f, 0.0f);
const VECTOR2 txMath::VEC2_ZERO = VECTOR2(0.0f, 0.0f);
const VECTOR3 txMath::VEC3_ONE = VECTOR3(1.0f, 1.0f, 1.0f);
const VECTOR2 txMath::VEC2_ONE = VECTOR2(1.0f, 1.0f);
const VECTOR3 txMath::COLOUR_BLACK = VECTOR3(0.0f, 0.0f, 0.0f);
const VECTOR3 txMath::COLOUR_WHITE = VECTOR3(255.0f, 255.0f, 255.0f);
const VECTOR3 txMath::COLOUR_RED = VECTOR3(255.0f, 0.0f, 0.0f);
const VECTOR3 txMath::COLOUR_GREEN = VECTOR3(0.0f, 255.0f, 0.0f);
const VECTOR3 txMath::COLOUR_BLUE = VECTOR3(0.0f, 0.0f, 255.0f);
const VECTOR3 txMath::COLOUR_YELLOW = VECTOR3(255.0f, 255.0f, 0.0f);
const VECTOR3 txMath::COLOUR_PURPLE = VECTOR3(255.0f, 0.0f, 255.0f);
const VECTOR3 txMath::COLOUR_ORANGE = VECTOR3(255.0f, 127.0f, 0.0f);
const VECTOR3 txMath::COLOUR_PINK = VECTOR3(255.0f, 127.0f, 255.0f);
const MATRIX3 txMath::MAT3_IDENTITY = MATRIX3(1.0f);
const MATRIX4 txMath::MAT4_IDENTITY = MATRIX4(1.0f);
const QUATERNION txMath::QUAT_IDENTITY = QUATERNION(1.0f, 0.0f, 0.0f, 0.0f);

VECTOR3 txMath::getDirectionFromEulerAngle(const VECTOR3& eulerAngle)
{
	// 如果pitch为90°或者-90°,则直接返回向量,此时无论航向角为多少,向量都是竖直向上或者竖直向下
	if (isFloatZero(eulerAngle.y - MATH_PI / 2.0f))
	{
		return AXIS_Y;
	}
	else if (isFloatZero(eulerAngle.y + MATH_PI / 2.0f))
	{
		return -AXIS_Y;
	}
	else
	{
		VECTOR3 dir;
		dir.z = cos(eulerAngle.x);
		dir.x = -sin(eulerAngle.x);
		dir.y = tan(eulerAngle.y);
		dir = txMath::normalize(dir);
		return dir;
	}
}

void txMath::getYawPitchFromDirection(const VECTOR3& dir, float& fYaw, float& fPitch)
{
	VECTOR3 curDir = txMath::normalize(dir);
	// 首先计算俯仰角,俯仰角是向量与X-Z平面的夹角,在上面为正,在下面为负
	fPitch = asin(curDir.y);

	// 再计算航向角,航向角是向量与在X-Z平面上的投影与Z轴正方向的夹角,从上往下看是顺时针为正,逆时针为负
	VECTOR3 projectionXZ(curDir.x, 0.0f, curDir.z);
	float len = txMath::getLength(projectionXZ);
	// 如果投影的长度为0,则表示俯仰角为90°或者-90°,航向角为0
	if (isFloatZero(len))
	{
		fYaw = 0.0f;
	}
	else
	{
		projectionXZ = txMath::normalize(projectionXZ);
		fYaw = std::acos(txMath::dot(projectionXZ, AXIS_Z));
		// 判断航向角的正负,如果x为正,则航向角为负,如果x为,则航向角为正
		if (curDir.x > 0.0f)
		{
			fYaw = -fYaw;
		}
	}
}

MATRIX3 txMath::getRotationMatrixFromDirection(const VECTOR3& dir)
{
	float fYaw = 0.0f;
	float fPitch = 0.0f;
	getYawPitchFromDirection(dir, fYaw, fPitch);
	return eulerAngleToMatrix3(VECTOR3(fYaw, fPitch, 0.0f));
}

QUATERNION txMath::getQuaternionFromDirection(const VECTOR3& dir)
{
	float fYaw = 0.0f;
	float fPitch = 0.0f;
	getYawPitchFromDirection(dir, fYaw, fPitch);
	return getYawQuaternion(fYaw) * getPitchQuaternion(fPitch);
}

void txMath::checkInt(float& value, const float& precision)
{
	// 先判断是否为0
	if (isFloatZero(value, precision))
	{
		value = 0.0f;
		return;
	}
	int intValue = (int)value;
	// 大于0
	if (value > 0.0f)
	{
		// 如果原值减去整数值小于0.5f,则表示原值可能接近于整数值
		if (value - (float)intValue < 0.5f)
		{
			if (isFloatZero(value - intValue, precision))
			{
				value = (float)intValue;
			}
		}
		// 如果原值减去整数值大于0.5f, 则表示原值可能接近于整数值+1
		else
		{
			if (isFloatZero(value - (intValue + 1), precision))
			{
				value = (float)(intValue + 1);
			}
		}
	}
	// 小于0
	else if (value < 0.0f)
	{
		// 如果原值减去整数值的结果的绝对值小于0.5f,则表示原值可能接近于整数值
		if (std::abs(value - (float)intValue) < 0.5f)
		{
			if (isFloatZero(value - intValue, precision))
			{
				value = (float)intValue;
			}
		}
		else
		{
			// 如果原值减去整数值的结果的绝对值大于0.5f, 则表示原值可能接近于整数值-1
			if (isFloatZero(value - (intValue - 1), precision))
			{
				value = (float)(intValue - 1);
			}
		}
	}
}

bool txMath::isMatrix3Equal(const MATRIX3& m1, const MATRIX3& m2)
{
	for (char i = 0; i < 3; ++i)
	{
		for (char j = 0; j < 3; ++j)
		{
			if (!txMath::isFloatZero(m1[i][j] - m2[i][j]))
			{
				return false;
			}
		}
	}
	return true;
}

bool txMath::isMatrix4Equal(const MATRIX4& m1, const MATRIX4& m2)
{
	for (char i = 0; i < 4; ++i)
	{
		for (char j = 0; j < 4; ++j)
		{
			if (!txMath::isFloatZero(m1[i][j] - m2[i][j]))
			{
				return false;
			}
		}
	}
	return true;
}

int txMath::getGreaterPowerValue(const int& value, const int& pow)
{
	int powValue = 1;
	for (int i = 0; i < 100; ++i)
	{
		if (powValue >= value)
		{
			break;
		}
		powValue *= pow;
	}
	return powValue;
}

int txMath::getForwardInt(const float& value)
{
	if (value >= 0.0f)
	{
		int intValue = (int)(value);
		if (value - intValue > 0.0f)
		{
			return intValue + 1;
		}
		else
		{
			return (int)value;
		}
	}
	else
	{
		return (int)value;
	}
}

void txMath::resetRight(MATRIX3& rotate)
{
	VECTOR3 right = rotateVector3(txMath::AXIS_X, rotate);
	if (!isFloatZero(right.y))
	{
		VECTOR3 eulerAngle = matrix3ToEulerAngle(rotate);
		eulerAngle.z = 0.0f;
		rotate = eulerAngleToMatrix3(eulerAngle);
	}
}

// 给定一段圆弧,以及圆弧圆心角的百分比,计算对应的圆弧上的一个点以及该点的切线方向
void txMath::getPosOnArc(const VECTOR3& circleCenter, const VECTOR3& startArcPos, const VECTOR3& endArcPos, float anglePercent, VECTOR3& pos, VECTOR3& tangencyDir)
{
	VECTOR3 relativeStart = startArcPos - circleCenter;
	VECTOR3 relativeEnd = endArcPos - circleCenter;
	float radius = getLength(relativeStart);
	clamp(anglePercent, 0.0f, 1.0f);
	// 首先判断从起始半径线段到终止半径线段的角度的正负
	float angleBetween = getAngleFromVectorToVector(VECTOR2(relativeStart.x, relativeStart.z), VECTOR2(relativeEnd.x, relativeEnd.z));
	if (isFloatZero(angleBetween))
	{
		pos = normalize(relativeStart) * radius;
		tangencyDir = normalize(rotateVector3(-pos, MATH_PI / 2.0f));
	}
	// 根据夹角的正负,判断应该顺时针还是逆时针旋转起始半径线段
	else
	{
		pos = normalize(rotateVector3(relativeStart, anglePercent * angleBetween)) * radius;
		// 计算切线,如果顺时针计算出的切线与从起始点到终止点所成的角度大于90度,则使切线反向
		tangencyDir = normalize(rotateVector3(-pos, MATH_PI / 2.0f));
		VECTOR3 posToEnd = relativeEnd - pos;
		if (std::abs(getAngleFromVectorToVector(VECTOR2(tangencyDir.x, tangencyDir.z), VECTOR2(posToEnd.x, posToEnd.z))) > MATH_PI / 2.0f)
		{
			tangencyDir = -tangencyDir;
		}
	}
	pos += circleCenter;
}

float txMath::getAngleFromVectorToVector(const VECTOR2& from, const VECTOR2& to)
{
	VECTOR3 fromVec3(from.x, 0.0f, from.y);
	VECTOR3 toVec3(to.x, 0.0f, to.y);
	float angle = getAngleBetweenVector(fromVec3, toVec3);
	// 根据叉乘后向量的方向来决定角度处理
	VECTOR3 crossVec = txMath::cross(fromVec3, toVec3);
	if (crossVec.y > 0.0f)
	{
		angle = -angle;
	}
	return angle;
}

float txMath::getAngleBetweenVector(const VECTOR3& vec1, const VECTOR3& vec2)
{
	VECTOR3 curVec1 = normalize(vec1);
	VECTOR3 curVec2 = normalize(vec2);
	float dotValue = txMath::dot(curVec1, curVec2);
	clamp(dotValue, -1.0f, 1.0f);
	return std::acos(dotValue);
}

// 从矩阵中获得旋转
MATRIX3 txMath::getMatrixRotation(const MATRIX4& matrix4)
{
	VECTOR3 axisX(matrix4[0].x, matrix4[0].y, matrix4[0].z);
	VECTOR3 axisY(matrix4[1].x, matrix4[1].y, matrix4[1].z);
	VECTOR3 axisZ(matrix4[2].x, matrix4[2].y, matrix4[2].z);
	axisX = txMath::normalize(axisX);
	axisY = txMath::normalize(axisY);
	axisZ = txMath::normalize(axisZ);
	MATRIX3 rotation;
	rotation[0].x = axisX.x;
	rotation[0].y = axisX.y;
	rotation[0].z = axisX.z;
	rotation[1].x = axisY.x;
	rotation[1].y = axisY.y;
	rotation[1].z = axisY.z;
	rotation[2].x = axisZ.x;
	rotation[2].y = axisZ.y;
	rotation[2].z = axisZ.z;
	return rotation;
}

void txMath::setMatrixRotation(MATRIX4& matrix4, const MATRIX3& rotation)
{
	VECTOR3 scale = getMatrixScale(matrix4);
	matrix4[0].x = rotation[0].x;
	matrix4[0].y = rotation[0].y;
	matrix4[0].z = rotation[0].z;
	matrix4[1].x = rotation[1].x;
	matrix4[1].y = rotation[1].y;
	matrix4[1].z = rotation[1].z;
	matrix4[2].x = rotation[2].x;
	matrix4[2].y = rotation[2].y;
	matrix4[2].z = rotation[2].z;
	setMatrixScale(matrix4, scale);
}

void txMath::setMatrixScale(MATRIX4& matrix4, const VECTOR3& scale)
{
	VECTOR3 vec1(matrix4[0].x, matrix4[0].y, matrix4[0].z);
	VECTOR3 vec2(matrix4[1].x, matrix4[1].y, matrix4[1].z);
	VECTOR3 vec3(matrix4[2].x, matrix4[2].y, matrix4[2].z);
	if (isFloatZero(getLength(vec1)))
	{
		vec1 = AXIS_X;
	}
	if (isFloatZero(getLength(vec2)))
	{
		vec2 = AXIS_Y;
	}
	if (isFloatZero(getLength(vec3)))
	{
		vec3 = AXIS_Z;
	}
	setLength(vec1, scale.x);
	setLength(vec2, scale.y);
	setLength(vec3, scale.z);
	matrix4[0].x = vec1.x;
	matrix4[0].y = vec1.y;
	matrix4[0].z = vec1.z;
	matrix4[1].x = vec2.x;
	matrix4[1].y = vec2.y;
	matrix4[1].z = vec2.z;
	matrix4[2].x = vec3.x;
	matrix4[2].y = vec3.y;
	matrix4[2].z = vec3.z;
}

// 根据欧拉角获得旋转矩阵,yaw,pitch,roll分量可能与其他的有所不同,但是计算结果是正确的
MATRIX3 txMath::eulerAngleToMatrix3(const VECTOR3& angle)
{
	// 该方法计算出来的旋转矩阵中俯仰角错误,所以不使用
	//float cosX = std::cos(angle.z);
	//float sinX = std::sin(angle.z);
	//float cosY = std::cos(angle.x);
	//float sinY = std::sin(angle.x);
	//float cosZ = std::cos(angle.y);
	//float sinZ = std::sin(angle.y);
	//MATRIX3 rot;
	//rot[0][0] = cosX * cosY;
	//rot[0][1] = cosX * sinY * sinZ - sinX * cosZ;
	//rot[0][2] = cosX * sinY * cosZ + sinX * sinZ;
	//rot[1][0] = sinX * cosY;
	//rot[1][1] = sinX * sinY * sinZ + cosX * cosZ;
	//rot[1][2] = sinX * sinY * cosZ - cosX * sinZ;
	//rot[2][0] = -sinY;
	//rot[2][1] = cosY * sinZ;
	//rot[2][2] = cosY * cosZ;
	//return rot;

	// 分别计算三个分量的旋转矩阵,然后相乘得出最后的结果
	return getYawMatrix3(angle.x) * getPitchMatrix3(angle.y) * getRollMatrix3(angle.z);
}

VECTOR3 txMath::matrix3ToEulerAngle(const MATRIX3& rot)
{
	// 计算滚动角
	// 首先求出矩阵中X-Y平面与世界坐标系水平面的交线
	// 交线为X = -rot[2][2] / rot[2][0] * Z,然后随意构造出一个向量
	VECTOR3 intersectLineVector;
	float angleRoll = 0.0f;
	if (!isFloatZero(rot[2][0]) || !isFloatZero(rot[2][2]))
	{
		// 矩阵中Z轴的x分量为0,则交线在世界坐标系的X轴上,取X轴正方向上的一个点
		if (isFloatZero(rot[2][0]) && !isFloatZero(rot[2][2]))
		{
			intersectLineVector = VECTOR3(1.0f, 0.0f, 0.0f);
		}
		// 矩阵中Z轴的z分量为0,则交线在世界坐标系的Z轴上,
		else if (!isFloatZero(rot[2][0]) && isFloatZero(rot[2][2]))
		{
			// Z轴朝向世界坐标系的X轴正方向,即Z轴的x分量大于0,应该计算X轴与世界坐标系的Z轴负方向的夹角
			if (rot[2][0] > 0.0f)
			{
				intersectLineVector = VECTOR3(0.0f, 0.0f, -1.0f);
			}
			// Z轴朝向世界坐标系的X轴负方向,应该计算X轴与世界坐标系的Z轴正方向的夹角
			else
			{
				intersectLineVector = VECTOR3(0.0f, 0.0f, 1.0f);
			}
		}
		// 矩阵中Z轴的x和z分量都不为0,取X轴正方向上的一个点
		else
		{
			intersectLineVector = VECTOR3(1.0f, 0.0f, -rot[2][0] / rot[2][2]);
		}
		// 然后求出矩阵中X轴与交线的夹角
		angleRoll = getAngleBetweenVector(intersectLineVector, VECTOR3(rot[0][0], rot[0][1], rot[0][2]));
		// 如果X轴的y分量大于0,则滚动角为负
		if (rot[0][1] > 0.0f)
		{
			angleRoll = -angleRoll;
		}
	}
	// 如果Z轴的x和z分量都为0,则俯仰角为90°或者-90°,此处不计算
	else
	{
		// 此时X-Y平面与水平面相平行,计算X轴与世界坐标系中X轴的夹角,X轴的z分量小于0时,滚动角为负
		angleRoll = getAngleBetweenVector(VECTOR3(rot[0][0], rot[0][1], rot[0][2]), AXIS_X);
		if (rot[0][2] < 0.0f)
		{
			angleRoll = -angleRoll;
		}
	}

	// 计算出滚动角后,将矩阵中的滚动角归0
	MATRIX3 nonRollMat = rot;
	if (!isFloatZero(angleRoll))
	{
		nonRollMat *= getRollMatrix3(-angleRoll);
	}

	// 然后计算俯仰角
	// Z轴与Z轴在水平面上的投影的夹角
	VECTOR3 zAxisInMatrix(nonRollMat[2][0], nonRollMat[2][1], nonRollMat[2][2]);
	float anglePitch = 0.0f;
	if (!isFloatZero(zAxisInMatrix.x) || !isFloatZero(zAxisInMatrix.z))
	{
		anglePitch = getAngleBetweenVector(zAxisInMatrix, VECTOR3(zAxisInMatrix.x, 0.0f, zAxisInMatrix.z));
		// Z轴的y分量小于0,则俯仰角为负
		if (nonRollMat[2][1] < 0.0f)
		{
			anglePitch = -anglePitch;
			// 如果Y轴的y分量小于0,则说明俯仰角的绝对值已经大于90°了
			if (nonRollMat[1][1] < 0.0f)
			{
				anglePitch = -MATH_PI - anglePitch;
			}
		}
		else
		{
			if (nonRollMat[1][1] < 0.0f)
			{
				anglePitch = MATH_PI - anglePitch;
			}
		}
	}
	// 如果在水平面上的投影为0,俯仰角为90°或-90°
	else
	{
		if (nonRollMat[2][1] > 0.0f)
		{
			anglePitch = MATH_PI / 2.0f;
		}
		else
		{
			anglePitch = -MATH_PI / 2.0f;
		}
	}

	// 然后计算航向角
	// X轴与世界坐标系中的X轴的夹角
	float angleYaw = getAngleBetweenVector(VECTOR3(nonRollMat[0][0], nonRollMat[0][1], nonRollMat[0][2]), AXIS_X);
	// X轴的z分量小于0,则航向角为负
	if (nonRollMat[0][2] < 0.0f)
	{
		angleYaw = -angleYaw;
	}
	return VECTOR3(angleYaw, anglePitch, angleRoll);
}

QUATERNION txMath::matrix3ToQuat(const MATRIX3& rot)
{
	return glm::quat_cast(rot);
	// 以下代码未经测试
	//QUATERNION quat;
	//float tr, s;
	//int i, j, k;

	//int nxt[3] = { 1, 2, 0 };
	////计算矩阵轨迹
	//tr = rot[0][0] + rot[1][1] + rot[2][2];
	////检查矩阵轨迹是正还是负
	//if (tr > 0.0)
	//{
	//	s = std::sqrt(tr + 1.0f);
	//	quat[3] = s / 2.0f;
	//	s = 0.5f / s;
	//	quat[0] = (rot[1][2] - rot[2][1]) * s;
	//	quat[1] = (rot[2][0] - rot[0][2]) * s;
	//	quat[2] = (rot[0][1] - rot[1][0]) * s;
	//}
	//else
	//{
	//	//轨迹是负
	//	i = 0;
	//	if (rot[1][1] > rot[0][0])
	//	{
	//		i = 1;
	//	}
	//	if (rot[2][2] > rot[i][i])
	//	{
	//		i = 2;
	//	}
	//	j = nxt[i];
	//	k = nxt[j];
	//	s = std::sqrt((rot[i][i] - (rot[j][j] + rot[k][k])) + 1.0f);
	//	float q[4];
	//	q[i] = s * 0.5f;
	//	if (s != 0.0f)
	//	{
	//		s = 0.5f / s;
	//	}
	//	q[3] = (rot[j][k] - rot[k][j]) * s;
	//	q[j] = (rot[i][j] - rot[j][i]) * s;
	//	q[k] = (rot[i][k] - rot[k][i]) * s;
	//	quat[0] = q[0];
	//	quat[1] = q[1];
	//	quat[2] = q[2];
	//	quat[3] = q[3];
	//}

	//// 单位化四元数
	//float sqrtLen = std::sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2] + quat[3] * quat[3]);
	//quat[0] /= sqrtLen;
	//quat[1] /= sqrtLen;
	//quat[2] /= sqrtLen;
	//quat[3] /= sqrtLen;
	//return quat;
}

void txMath::collideSpeed(const VECTOR3& pos1, VECTOR3 v1, const float& m1, const VECTOR3& pos2, VECTOR3 v2, const float& m2, VECTOR3& newV1, VECTOR3& newV2)
{
	// 如果两个碰撞物的速度很小,则将速度设置为0
	if (txMath::getLength(v1) < 0.01f)
	{
		v1 = VEC3_ZERO;
	}
	if (txMath::getLength(v2) < 0.01f)
	{
		v2 = VEC3_ZERO;
	}

	// 以pos1为原点,pos1往pos2方向为Z轴正方向,构建一个右手坐标系
	MATRIX3 rotation = getRotationMatrixFromDirection(pos2 - pos1);
	// 然后将v1和v2依照构建出的右手坐标系,将速度分解到各个坐标轴上
	MATRIX3 inverseRot = txMath::inverse(rotation);
	VECTOR3 transV1 = rotateVector3(v1, inverseRot);
	VECTOR3 transV2 = rotateVector3(v2, inverseRot);
	// Z轴的速度交换,其他轴的速度保持不变,交换速度时需要考虑到质量
	VECTOR3 newTransV1 = transV1;
	VECTOR3 newTransV2 = transV2;
	newTransV1.z = transV2.z * m2 / m1;
	newTransV2.z = transV1.z * m1 / m2;
	newV1 = rotateVector3(newTransV1, rotation);
	newV2 = rotateVector3(newTransV2, rotation);
	checkInt(newV1.x);
	checkInt(newV1.y);
	checkInt(newV1.z);
	checkInt(newV2.x);
	checkInt(newV2.y);
	checkInt(newV2.z);
}

void txMath::getFrequencyZone(const short* pcmData, const int& dataCount, short* frequencyList)
{
	const int MAX_FFT_COUNT = 1024 * 8;
	static txComplex complexList[MAX_FFT_COUNT];
	if (dataCount > MAX_FFT_COUNT)
	{
		ENGINE_ERROR("pcm data count is too many, data count : %d, max count : %d", dataCount, MAX_FFT_COUNT);
		return;
	}
	for (int i = 0; i < dataCount; ++i)
	{
		complexList[i].mReal = pcmData[i];
		complexList[i].mImg = 0.0f;
	}
	fft(complexList, dataCount);
	for (int i = 0; i < dataCount; ++i)
	{
		frequencyList[i] = (short)std::sqrt(complexList[i].mReal * complexList[i].mReal + complexList[i].mImg * complexList[i].mImg);
	}
}

/*
* FFT Algorithm
* === Inputs ===
* x : complex numbers
* N : nodes of FFT. @N should be power of 2, that is 2^(*)
* === Output ===
* the @x contains the result of FFT algorithm, so the original data
* in @x is destroyed, please store them before using FFT.
*/
void txMath::fft(txComplex* x, const int& count)
{
	static const float sin_tb[] = {  // 精度(PI PI/2 PI/4 PI/8 PI/16 ... PI/(2^k))
		0.000000f, 1.000000f, 0.707107f, 0.382683f, 0.195090f, 0.098017f,
		0.049068f, 0.024541f, 0.012272f, 0.006136f, 0.003068f, 0.001534f,
		0.000767f, 0.000383f, 0.000192f, 0.000096f, 0.000048f, 0.000024f,
		0.000012f, 0.000006f, 0.000003f, 0.000003f, 0.000003f, 0.000003f,
		0.000003f
	};

	static const float cos_tb[] = {  // 精度(PI PI/2 PI/4 PI/8 PI/16 ... PI/(2^k))
		-1.000000f, 0.000000f, 0.707107f, 0.923880f, 0.980785f, 0.995185f,
		0.998795f, 0.999699f, 0.999925f, 0.999981f, 0.999995f, 0.999999f,
		1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f,
		1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f,
		1.000000f
	};
	int i, j, k;
	static float sR, sI, uR, uI;
	static txComplex tempComplex;

	/*
	* bit reversal sorting
	*/
	int l = count >> 1;
	j = l;
	int forCount = count - 2;
	for (i = 1; i <= forCount; ++i)
	{
		if (i < j) 
		{
			tempComplex = x[j];
			x[j] = x[i];
			x[i] = tempComplex;
		}
		k = l;
		while (k <= j) 
		{
			j -= k;
			k >>= 1;
		}
		j += k;
	}

	/*
	* For Loops
	*/
	int dftForCount = count - 1;
	int le = 1;
	int halfLe = 0;
	float angle = 0.0f;
	int M = (int)(log(count) / log(2));
	int ip = 0;
	for (l = 0; l < M; ++l) 
	{   
		// 在le左移1位之前保存值,也就是左移以后的值的一半
		halfLe = le;
		le <<= 1;
		uR = 1;
		uI = 0;
		sR = cos_tb[l];
		sI = -sin_tb[l];
		for (j = 0; j < halfLe; ++j)
		{   
			/* loop for each sub DFT */
			for (i = j; i <= dftForCount; i += le)
			{  
				/* loop for each butterfly */
				ip = i + halfLe;
				tempComplex.mReal = x[ip].mReal * uR - x[ip].mImg * uI;
				tempComplex.mImg = x[ip].mReal * uI + x[ip].mImg * uR;
				x[ip] = x[i] - tempComplex;
				x[i] += tempComplex;
			}
			tempComplex.mReal = uR;
			uR = tempComplex.mReal * sR - uI * sI;
			uI = tempComplex.mReal * sI + uI * sR;
		}
	}
}

/*
* Inverse FFT Algorithm
* === Inputs ===
* x : complex numbers
* N : nodes of FFT. @N should be power of 2, that is 2^(*)
* === Output ===
* the @x contains the result of FFT algorithm, so the original data
* in @x is destroyed, please store them before using FFT.
*/
void txMath::ifft(txComplex* x, const int& count)
{
	int k = 0;

	for (k = 0; k <= count - 1; k++)
	{
		x[k].mImg = -x[k].mImg;
	}

	fft(x, count);

	for (k = 0; k <= count - 1; k++)
	{
		x[k].mReal = x[k].mReal / count;
		x[k].mImg = -x[k].mImg / count;
	}
}

int txMath::pcm_db_count(const short* ptr, const int& size)
{
	long v = 0;
	for (int i = 0; i < size; ++i)
	{
		v += abs(ptr[i]);
	}
	v /= size;

	int ndb = -96;
	if (v != 0)
	{
		ndb = (int)(20.0f * log10((float)v / 0xFFFF));
	}
	return ndb;
}

txPoint txMath::pointToRelotivePoint(const txPoint& point, const VECTOR2& parentSize)
{
	VECTOR2 pos = pointToVector2(point, parentSize);
	return txPoint(txDim(pos.x / parentSize.x, 0), txDim(pos.y / parentSize.y, 0));
}

txPoint txMath::pointToAbsolutePoint(const txPoint& point, const VECTOR2& parentSize)
{
	VECTOR2 pos = pointToVector2(point, parentSize);
	return txPoint(txDim(0.0f, (int)(pos.x + 0.5f)), txDim(0.0f, (int)(pos.y + 0.5f)));
}

bool txMath::replaceKeywordAndCalculate(std::string& str, const std::string& keyword, const int& replaceValue, const bool& floatOrInt)
{
	// 如果最后的表达式中存在i,则需要把i替换为i具体的值,然后计算出最后的表达式的值
	bool replaced = false;
	int iPos;
	while ((iPos = str.find_first_of(keyword)) != -1)
	{
		replaced = true;
		str = txStringUtility::strReplace(str, iPos, iPos + keyword.length(), txStringUtility::intToString(replaceValue));
	}
	if (floatOrInt)
	{
		str = txStringUtility::floatToString(calculateFloat(str), 4);
	}
	else
	{
		str = txStringUtility::intToString(calculateInt(str));
	}
	return replaced;
}

bool txMath::replaceStringKeyword(std::string& str, const std::string& keyword, const int& keyValue, const bool& floatOrInt)
{
	bool replaced = false;
	int expressionBegin = -1;
	int expressionEnd = -1;
	// 倒序寻找
	while (txStringUtility::findSubstr(str, std::string("\\("), true, &expressionBegin, 0, false))
	{
		replaced = true;
		// 找到匹配的)
		txStringUtility::findSubstr(str, std::string(")"), true, &expressionEnd, 0, false);
		// expressionBegin + 1 去掉 /
		std::string calculateValue = str.substr(expressionBegin + 1, expressionEnd - expressionBegin + 1);
		replaceKeywordAndCalculate(calculateValue, keyword, keyValue, floatOrInt);
		// 替换掉最后一个\\()之间的内容
		str = txStringUtility::strReplace(str, expressionBegin, expressionEnd + 1, calculateValue);
	}
	return replaced;
}

float txMath::powerFloat(const float& f, int p)
{
	float ret = 1.0f;
	while (p)
	{
		ret *= f;
		--p;
	}
	return ret;
}

float txMath::calculateFloat(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			float ret = calculateFloat(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string floatStr = txStringUtility::floatToString(ret, 4);
			str = txStringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, floatStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = txStringUtility::strReplace(str, i, i + 1, EMPTY_STRING);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<float> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == strLen - 1)
		{
			std::string num = str.substr(beginpos, strLen - beginpos);
			float fNum = txStringUtility::stringToFloat(num);
			numbers.push_back(fNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				float fNum = txStringUtility::stringToFloat(num);
				numbers.push_back(fNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				float num1 = numbers[i];
				float num2 = numbers[i + 1];
				float num3 = 0.0f;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			float num1 = numbers[0];
			float num2 = numbers[1];
			float num3 = 0.0f;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

int txMath::calculateInt(std::string str)
{
	// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
	for (int i = 0; i < (int)str.length();)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.' && str[i] != '+'
			&& str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%'
			&& str[i] != '(' && str[i] != ')')
		{
			str.erase(i, 1);
		}
		else
		{
			++i;
		}
	}
	// 判断左右括号数量是否相等
	int leftBracketCount = 0;
	int rightBracketCount = 0;
	int strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		if (str[i] == '(')
		{
			++leftBracketCount;
		}
		else if (str[i] == ')')
		{
			++rightBracketCount;
		}
	}
	if (leftBracketCount != rightBracketCount)
	{
		// 计算错误,左右括号数量不对应
		return 0;
	}

	// 循环判断传入的字符串有没有括号
	while (true)
	{
		// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
		if (str.find_first_of("(") != -1 || str.find_first_of(")") != -1)
		{
			int curpos = str.find_last_of("(");
			std::string strInBracket = str.substr(curpos + 1, str.length() - curpos - 1);
			strInBracket = strInBracket.substr(0, strInBracket.find_first_of(")"));
			int ret = calculateInt(strInBracket);
			// 如果括号中的计算结果是负数,则标记为负数
			bool isMinus = false;
			if (ret < 0)
			{
				ret = -ret;
				isMinus = true;
			}
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			std::string intStr = txStringUtility::intToString(ret, 4);
			str = txStringUtility::strReplace(str, curpos, curpos + strInBracket.length() + 2, intStr);
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = curpos - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (str[i] == '+')
					{
						str[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					else if (str[i] == '-')
					{
						if (str[i - 1] >= '0' && str[i - 1] <= '9')
						{
							str[i] = '+';
						}
						else
						{
							str = txStringUtility::strReplace(str, i, i + 1, EMPTY_STRING);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					str = "-" + str;
				}
			}
		}
		else
		{
			break;
		}
	}

	txVector<int> numbers;
	txVector<char> factors;
	// 表示上一个运算符的下标+1
	int beginpos = 0;
	strLen = (int)str.length();
	for (int i = 0; i < strLen; ++i)
	{
		// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
		if (i == strLen - 1)
		{
			std::string num = str.substr(beginpos, strLen - beginpos);
			int iNum = txStringUtility::stringToInt(num);
			numbers.push_back(iNum);
			break;
		}
		// 找到第一个运算符
		if ((str[i] < '0' || str[i] > '9') && str[i] != '.')
		{
			if (i != 0)
			{
				std::string num = str.substr(beginpos, i - beginpos);
				int iNum = txStringUtility::stringToInt(num);
				numbers.push_back(iNum);
			}
			// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
			else
			{
				numbers.push_back(0);
			}
			factors.push_back(str[i]);
			beginpos = i + 1;
		}
	}
	if (factors.size() + 1 != numbers.size())
	{
		// 计算错误,运算符与数字数量不符
		return 0;
	}
	// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
	while (true)
	{
		// 表示是否还有乘除表达式
		bool hasMS = false;
		for (int i = 0; i < (int)factors.size(); ++i)
		{
			// 先遍历到哪个就先计算哪个
			if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				int num1 = numbers[i];
				int num2 = numbers[i + 1];
				int num3 = 0;
				if (factors[i] == '*')
				{
					num3 = num1 * num2;
				}
				else if (factors[i] == '/')
				{
					num3 = num1 / num2;
				}
				else if (factors[i] == '%')
				{
					num3 = num1 % num2;
				}
				// 删除第i + 1个数,然后将第i个数替换为计算结果
				numbers.erase(numbers.begin() + i + 1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[i] = num3;
				// 删除第i个运算符
				factors.erase(factors.begin() + i);
				hasMS = true;
				break;
			}
		}
		if (!hasMS)
		{
			break;
		}
	}
	// 再计算加减法
	while (true)
	{
		if (factors.size() == 0)
		{
			break;
		}
		if (factors[0] == '+' || factors[0] == '-')
		{
			// 第一个运算数的下标与运算符的下标是相同的
			int num1 = numbers[0];
			int num2 = numbers[1];
			int num3 = 0;
			if (factors[0] == '+')
			{
				num3 = num1 + num2;
			}
			else if (factors[0] == '-')
			{
				num3 = num1 - num2;
			}
			// 删除第1个数,然后将第0个数替换为计算结果
			numbers.erase(numbers.begin() + 1);
			if (numbers.size() == 0)
			{
				// 计算错误
				return 0;
			}
			numbers[0] = num3;
			// 删除第0个运算符
			factors.erase(factors.begin());
		}
	}
	if (numbers.size() != 1)
	{
		// 计算错误
		return 0;
	}
	else
	{
		return numbers[0];
	}
}

// 秒数转换为分数和秒数
void txMath::secondsToMinutesSeconds(int seconds, int& outMin, int& outSec)
{
	outMin = seconds / 60;
	outSec = seconds - outMin * 60;
}

void txMath::secondsToHoursMinutesSeconds(int seconds, int& outHour, int& outMin, int& outSec)
{
	outHour = seconds / (60 * 60);
	outMin = (seconds - outHour * (60 * 60)) / 60;
	outSec = seconds - outHour * (60 * 60) - outMin * 60;
}

float txMath::HueToRGB(float v1, float v2, float vH)
{
	if (vH < 0.0f)
	{
		vH += 1.0;
	}
	if (vH > 1.0f)
	{
		vH -= 1.0f;
	}
	if (6.0f * vH < 1.0f)
	{
		return v1 + (v2 - v1) * 6.0f * vH;
	}
	else if (2.0f * vH < 1.0f)
	{
		return v2;
	}
	else if (3.0f * vH < 2.0f)
	{
		return v1 + (v2 - v1) * (2.0f / 3.0f - vH) * 6.0f;
	}
	else
	{
		return v1;
	}
}

// rgb转换为色相(H),饱和度(S),亮度(L)
// HSL和RGB的范围都是0-1
VECTOR3 txMath::RGBtoHSL(const VECTOR3& rgb)
{
	float minRGB = std::min(std::min(rgb.r, rgb.g), rgb.b);
	float maxRGB = std::max(std::max(rgb.r, rgb.g), rgb.b);
	float delta = maxRGB - minRGB;

	float H = 0.0f;
	float S = 0.0f;
	float L = (maxRGB + minRGB) / 2.0f;
	// 如果三个分量的最大和最小相等,则说明该颜色是灰色的,灰色的色相和饱和度都为0
	if (delta > 0.0f)
	{
		if (L < 0.5f)
		{
			S = delta / (maxRGB + minRGB);
		}
		else
		{
			S = delta / (2.0f - maxRGB - minRGB);
		}

		float delR = ((maxRGB - rgb.r) / 6.0f + delta / 2.0f) / delta;
		float delG = ((maxRGB - rgb.g) / 6.0f + delta / 2.0f) / delta;
		float delB = ((maxRGB - rgb.b) / 6.0f + delta / 2.0f) / delta;

		if (rgb.r == maxRGB)
		{
			H = delB - delG;
		}
		else if (rgb.g == maxRGB)
		{
			H = 1.0f / 3.0f + delR - delB;
		}
		else if (rgb.b == maxRGB)
		{
			H = 2.0f / 3.0f + delG - delR;
		}

		//if (H < 0.0)
		//{
		//	H += 1.0;
		//}
		//if (H > 1.0)
		//{
		//	H -= 1.0;
		//}
		// 以上代码可优化为以下代码(因为在shader中条件判断语句执行都比较慢)
		float s = getSign(H * H - H);	// 判断H是否小于0或者大于1(返回1), 大于等于0且小于等于1(返回0, -1)
		clamp(s, 0.0f, 1.0f);
		H -= getSign(H) * s;
		// 以下是推导过程
		//1-------------------------------------------------------------
		//float delta = 0;
		//if (H < 0)
		//{
		//	delta = -1;
		//}
		//else if (H >= 0 && H <= 1)
		//{
		//	delta = 0;
		//}
		//else if (H > 1)
		//{
		//	delta = 1;
		//}
		//H -= delta;
		//2-------------------------------------------------------------
		//float delta = 0;
		//if (H >= 0 && H <= 1)
		//{
		//	delta = 0;
		//}
		//else
		//{
		//	delta = 1;
		//}
		//H -= sign(H) * delta;
		//-------------------------------------------------------------------
		//float delta = 0;
		//if (H * H - H <= 0)
		//{
		//	delta = 0;
		//}
		//else
		//{
		//	delta = 1;
		//}
		//H -= sign(H) * delta;
		//3-------------------------------------------------------------------
		//float delta = sign(H * H - H);
		//if (delta <= 0)
		//{
		//	delta = 0;
		//}
		//else
		//{
		//	delta = 1;
		//}
		//H -= sign(H) * delta;
		//4-------------------------------------------------------------------
		//float delta = sign(H * H - H);
		//delta = clamp(delta, 0, 1);
		//H -= sign(H) * delta;
	}
	return VECTOR3(H, S, L);
}

// 色相(H),饱和度(S),亮度(L),转换为rgb
// HSL和RGB的范围都是0-1
VECTOR3 txMath::HSLtoRGB(const VECTOR3& hsl)
{
	VECTOR3 rgb;
	float H = hsl.r;
	float S = hsl.g;
	float L = hsl.b;
	if (S == 0.0f)						//HSL from 0 to 1
	{
		rgb.r = L;				//RGB results from 0 to 255
		rgb.g = L;
		rgb.b = L;
	}
	else
	{
		float var2;
		if (L < 0.5f)
		{
			var2 = L * (1.0f + S);
		}
		else
		{
			var2 = L + S - (S * L);
		}

		float var1 = 2.0f * L - var2;
		rgb.r = HueToRGB(var1, var2, H + 1.0f / 3.0f);
		rgb.g = HueToRGB(var1, var2, H);
		rgb.b = HueToRGB(var1, var2, H - 1.0f / 3.0f);
	}
	return rgb;
}