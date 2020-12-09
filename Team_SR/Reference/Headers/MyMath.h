#pragma once
#include "Engine_Include.h"
#include <random>

#ifndef __MYMATH_H__

class MATH
{
public:
	// Float ( == ) 연산시 사용.
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

	// 해당 축으로 Lhs 을 회전시켜서 리턴
	// D3DXVec3TransformCoord 함수를 사용함을 유의.
	static _vector RotationVec(const _vector& Lhs,
		const _vector& Axis, const float Degree)
	{
		_matrix Rotation;
		D3DXMatrixRotationAxis(&Rotation, &Axis, ToRadian(Degree) );
		_vector RotationVec ;
		D3DXVec3TransformCoord(&RotationVec, &Lhs, &Rotation);
		return RotationVec;
	}

	/* a + t(b-a)*/
	// a=50 b=100 t=0.3 -> Lerp(x) = 65
	template<typename _Ty>
	static _Ty Lerp(_Ty Lhs, _Ty Rhs, float t)
	{
		return Lhs + t * (Rhs - Lhs);
	}

	FORCEINLINE static vec3 Normalize(const vec3& Lhs)
	{
		vec3 NormVec;
		D3DXVec3Normalize(&NormVec ,&Lhs );
		return NormVec;
	}
	FORCEINLINE static float Length(const vec3& Lhs)
	{
		return D3DXVec3Length(&Lhs);
	}
	FORCEINLINE static float LengthSq(const vec3& Lhs)
	{
		return D3DXVec3LengthSq(&Lhs);
	}
#pragma region RANDOM
	static std::random_device Rd;
	static std::mt19937 gen;

	static void RandomInit()
	{
		gen.seed(Rd());
	};

	static int32_t RandInt(const std::pair<int32_t, int32_t>& Range)
	{
		std::uniform_int_distribution<int32_t> Dis(Range.first, Range.second);
		return Dis(gen);
	}

	static float RandReal(const std::pair<float, float>& Range)
	{
		std::uniform_real_distribution<float> Dis(Range.first, Range.second);
		return Dis(gen);
	}

	static vec3 RandVec()
	{
		return MATH::Normalize(vec3{
			RandReal({ -1,1 }),
			RandReal({ -1,1 }),
			RandReal({ -1,1 })
			});
	}
#pragma endregion RANDOM
};
std::random_device MATH::Rd{};
std::mt19937 MATH::gen{};
const _vector MATH::AxisX{ 1,0,0 };
const _vector MATH::AxisY{ 0,1,0 };
const _vector MATH::AxisZ{ 0,0,1 };
#define __MYMATH_H__
#endif