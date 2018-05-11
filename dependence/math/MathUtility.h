#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#include "MathHeader.h"

namespace txMath
{
	const float PI = 3.141593f;
	const float MIN_DELTA = 0.00001f;

	// direction
	void getYawPitchFromDirection(txVector3 direction, float& yaw, float& pitch); // 根据方向得到航向角和俯仰角
	txMatrix3 getMatrix3FromDirection(txVector3 direction); // 根据方向得到旋转矩阵
	txMatrix3 getMatrix3FromYawPitchRoll(float yaw, float pitch, float roll = 0.0f);
	txQuaternion getQuaternionFromDirection(txVector3 direction); // 根据方向得到旋转四元数
	txVector3 getDirectionFromYawPitch(float yaw, float pitch); // 根据航向角和俯仰角得到方向
	txVector3 getDirectionFromMatrix3(txMatrix3 mat); // 根据3阶旋转矩阵得到方向
	txVector3 getDirectionFromQuaternion(txQuaternion qua); // 根据四元数得到方向

	// angle
	void adjustAngle360(float& radianAngle);// 将弧度制的角度值限制在0到3.14 * 2之间
	void adjustAngle180(float& radianAngle);// 将弧度制的角度值限制在-3.14到3.14之间

	// zero
	void checkInt(float& value); // 判断传入的参数是否已经接近于整数,如果接近于整数,则设置为整数
	bool isFloatZero(float value); // 判断传入的参数是否等于0

	// matrix3
	txMatrix3 operator * (const txMatrix3& m1, const txMatrix3& m2);
	txVector3 operator * (const txVector3& v1, const txMatrix3& m1);
	txMatrix3 getMatrix3FromYaw(float yaw); // 根据弧度制的航向角的到旋转矩阵,从Y轴正方向往Y轴负方向看逆时针为正
	txMatrix3 getMatrix3FromPitch(float pitch); // 根据弧度制的俯仰角的到旋转矩阵, 从X轴负方向往X轴正方向看顺时针为正
	txMatrix3 getMatrix3FromRoll(float roll); // 根据弧度制的滚动角的到旋转矩阵, 从Z轴负方向往Z轴正方向看顺时针为正
	txQuaternion matrix3ToQuaternion(txMatrix3 mat3); // 根据3阶矩阵得到四元数
	txMatrix3 quaternionToMatrix3(txQuaternion q); // 根据四元数得到3阶矩阵

	// matrix4
	txMatrix4 operator * (const txMatrix4& m, float s);
	txMatrix4 operator * (const txMatrix4& m1, const txMatrix4& m2);
	txVector3 operator * (const txVector3& v1, const txMatrix4& m1);
	txMatrix4 getScaleMatrix4(txVector3 scale);
	txMatrix4 getTransformMatrix4(txVector3 transform);
	txMatrix4 getRotationMatrix4(float yaw, float pitch, float row);
	txMatrix4 getViewMatrix4RH(txVector3 eye, txVector3 center, txVector3 up);
	txMatrix4 getPerspectiveMatrix4RH(float fovy, float aspect, float zNear, float zFar);
	txMatrix4 getOrthoMatrix4(float left, float right, float bottom, float top, float zNear, float zFar);

	// quaternion
	txVector3 operator * (txVector3 v, txQuaternion q);
	float dot(txQuaternion q1, txQuaternion q2);
	txQuaternion operator / (txQuaternion q, float s);
	txQuaternion operator * (txQuaternion q, float s);
	txQuaternion operator * (float s, txQuaternion q);
	txQuaternion operator * (txQuaternion q1, txQuaternion q2);
	txQuaternion operator - (txQuaternion q2);
	txQuaternion operator + (txQuaternion q1, txQuaternion q2);
	float mix(float x, float y, float a);
	txQuaternion mix(txQuaternion x, txQuaternion y, float a);
	txQuaternion lerp(txQuaternion x, txQuaternion y, float a); // 线性插值
	txQuaternion slerp(txQuaternion q1, txQuaternion q2, float a); // 球面线性插值

	// vector2
	txVector2 operator + (txVector2 vec1, txVector2 vec2);
	txVector2 operator - (txVector2 vec1, txVector2 vec2);
	txVector2 operator * (txVector2 vec1, float scale);
	txVector2 operator / (txVector2 vec1, float scale);

	// vector3
	txVector3 operator + (txVector3 vec1, txVector3 vec2);
	txVector3 operator - (txVector3 vec1, txVector3 vec2);
	txVector3 operator * (txVector3 vec1, float scale);
	txVector3 operator / (txVector3 vec1, float scale);

	// vector4
	txVector4 operator + (const txVector4& v1, const txVector4& v2);
	txVector4 operator - (const txVector4& v1, const txVector4& v2);
}

#endif