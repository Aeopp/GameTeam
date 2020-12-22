#pragma once

#ifndef __MYMATH_H__
#include "Engine_Include.h"
BEGIN(Engine)
class ENGINE_DLL MATH
{
public:
	static constexpr float Gravity = 0.1f;
	// Float ( == ) 연산시 사용.
	template<class T>
	typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
	static	almost_equal(T x, T y)
	{
		return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * 1
			
			|| std::fabs(x - y) < (std::numeric_limits<T>::min)();
	}

	// 벡터 4로 반환되며 기본값 사용시 방향 벡터로 간주
	FORCEINLINE static vec4 ConvertVec4(const vec3& Lhs, const float w = 0)
	{
		return vec4{ Lhs.x, Lhs.y, Lhs.z, w };
	};

	static vec3 GetNormalFromFace(const vec3& p0,
		const vec3& p1, const vec3& p2);

	static vec3 Mul(const vec3& Lhs, const mat& Rhs);
	static vec3 MulNormal(const vec3& Lhs, const mat& Rhs);

	static Ray GetRayScreenProjection(const vec3& ScreenPos,
		IDirect3DDevice9* const _Device, const float Width, const float Height);
	static bool InnerPointFromFace(const vec3& Point, const std::array<vec3, 3ul>& Face);

	// 임의의 위치 벡터를 평면에 투영시킨 위치 벡터를 반환.
	static vec3 ProjectionPointFromFace(D3DXPLANE _Plane, const vec3& Point);

								// 삼각형을 둘러치는 선분 3개.
	static std::array<Segment, 3ul> MakeSegmentFromFace(const std::array<vec3, 3ul>& Face);
	
	static constexpr float PI = 3.141592653589793238462643383279502884197169399375f;
	static float ToRadian(float Degree){return Degree * ( PI / 180.0f);};
	static float ToDegree(float Radian) { return Radian* (180.0f / PI);   };
	static const _vector AxisX;
	static const _vector AxisY;
	static const _vector AxisZ;

	// 해당 축으로 Lhs 을 회전시켜서 리턴
	// D3DXVec3TransformCoord 함수를 사용함을 유의.
	static _vector RotationVec(const _vector& Lhs,
		const _vector& Axis, const float Degree);

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
	};
	FORCEINLINE static float LengthSq(const vec4& Lhs)
	{
		return D3DXVec4LengthSq(&Lhs);
	};
	FORCEINLINE static float Dot(const vec3& Lhs, const vec3& Rhs)
	{
		return D3DXVec3Dot(&Lhs, &Rhs);
	}
	FORCEINLINE static vec3 Cross(const vec3& Lhs, const vec3& Rhs)
	{
		vec3 ReturnValue;
		D3DXVec3Cross(&ReturnValue, &Lhs, &Rhs);
		return ReturnValue;
	}

	static mat Inverse(const mat& _mat)
	{
		mat _inv;
		D3DXMatrixInverse(&_inv, nullptr, &_mat);
		return _inv;
	}


	static mat Transpose(const mat& _mat)
	{
		mat _transpose;
		D3DXMatrixTranspose(&_transpose, &_mat);
		return _transpose;
	}

	//static float Parabolic(const float InitY/*운동을 시작한 타이밍의 높이 좌표*/,
	//	const float Speed/*단위 벡터가 아님 속도도 곱해야함*/,
	//	const float Degree, const float t, const float Gravity);
	static mat RotationMatrixFromAxis(const vec3 Axis, float Degree)
	{
		mat rot;
		D3DXMatrixRotationAxis(&rot, &Axis, MATH::ToRadian(Degree));
		return rot;
	};

	static mat WorldMatrix(const vec3& Scale, const vec3& Rotation,
		const vec3& Location);

#pragma region RANDOM
	static std::random_device Rd;
	static std::mt19937 gen;

	static void RandomInit(){gen.seed(Rd());};

	static int32_t RandInt(const std::pair<int32_t, int32_t>& Range);
	static float RandReal(const std::pair<float, float>& Range);
	static vec3 RandVec();
#pragma endregion RANDOM
};

END

#define __MYMATH_H__
#endif