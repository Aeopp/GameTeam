#pragma once
#include "Engine_Include.h"

#ifndef __MYMATH_H__

class MATH
{
public:
	// Float ( == ) ����� ���.
	template<class T>
	typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
		static almost_equal(T x, T y)
	{
		return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y)
			|| std::fabs(x - y) < (std::numeric_limits<T>::min)();
	}
	
	static constexpr float PI = 3.14159265359f;
	static float ToRadian(float Degree){return Degree * ( PI / 180.0f);};
	static float ToDegree(float Radian) { return Radian* (180.0f / PI);   };
	static const _vector AxisX;
	static const  _vector AxisY;
	static const _vector AxisZ;

	// �ش� ������ Lhs �� ȸ�����Ѽ� ����
	// D3DXVec3TransformCoord �Լ��� ������� ����.
	static _vector RotationVec(const _vector& Lhs,
		const _vector& Axis, const float Degree)
	{
		_matrix Rotation;
		D3DXMatrixRotationAxis(&Rotation, &Axis, ToRadian(Degree) );
		_vector RotationVec ;
		D3DXVec3TransformCoord(&RotationVec, &Lhs, &Rotation);
		return RotationVec;
	}
};
const _vector MATH::AxisX{ 1,0,0 };
const _vector MATH::AxisY{ 0,1,0 };
const _vector MATH::AxisZ{ 0,0,1 };
#define __MYMATH_H__
#endif