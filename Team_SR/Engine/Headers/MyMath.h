#pragma once

#ifndef __MYMATH_H__
#include "Engine_Include.h"
BEGIN(Engine)
class ENGINE_DLL MATH
{
public:
	// Float ( == ) ����� ���.
	template<class T>
	typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
	static	almost_equal(T x, T y)
	{
		return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * 1
			
			|| std::fabs(x - y) < (std::numeric_limits<T>::min)();
	}

	static vec3 Mul(const vec3& Lhs, const mat& Rhs)
	{
		vec3 Return;
		D3DXVec3TransformCoord(&Return, &Lhs, &Rhs);
		return Return;
	}
	static vec3 MulNormal(const vec3& Lhs, const mat& Rhs)
	{
		vec3 Return;
		D3DXVec3TransformNormal(&Return, &Lhs, &Rhs);
		return Return;
	}

	static bool InnerPointFromFace(const vec3 & Point,const std::array<vec3, 3ul>& Face)
	{
		std::array <vec3, 3ul> ToVertexs;

		for (size_t i = 0; i < ToVertexs.size(); ++i)
		{
			ToVertexs[i] = MATH::Normalize(Face[i] - Point);
		}

		float Radian= 0;
		Radian += std::acosf(MATH::Dot(ToVertexs[0], ToVertexs[1]));
		Radian += std::acosf(MATH::Dot(ToVertexs[1], ToVertexs[2]));
		Radian += std::acosf(MATH::Dot(ToVertexs[2], ToVertexs[0]));

		return almost_equal(Radian, MATH::PI * 2.f);
	}

	// ������ ��ġ ���͸� ��鿡 ������Ų ��ġ ���͸� ��ȯ.
	static vec3 ProjectionPointFromFace(D3DXPLANE _Plane, const vec3& Point)
	{
		vec3 Normal = { _Plane.a,_Plane.b,_Plane.c };
		Normal=MATH::Normalize(Normal);
		const float distance = MATH::Dot(Normal, Point) +_Plane.d;
		Normal = -Normal;
		return Point  + ( Normal* distance);
	}
								// �ﰢ���� �ѷ�ġ�� ���� 3��.
	static std::array<Segment, 3ul> MakeSegmentFromFace(const std::array<vec3, 3ul>& Face)
	{
		std::array<Segment, 3ul> Segments;

		{
			Segments[0]._Ray.Start = Face[0];
			const vec3 Distance = Face[1] - Face[0];
			Segments[0]._Ray.Direction = MATH::Normalize(Distance);
			Segments[0].t = MATH::Length(Distance);
		}

		{
			Segments[1]._Ray.Start = Face[1];
			const vec3 Distance = Face[2] - Face[1];
			Segments[1]._Ray.Direction = MATH::Normalize(Distance);
			Segments[1].t = MATH::Length(Distance);
		}

		{
			Segments[2]._Ray.Start = Face[2];
			const vec3 Distance = Face[0] - Face[2];
			Segments[2]._Ray.Direction = MATH::Normalize(Distance);
			Segments[2].t = MATH::Length(Distance);
		}

		return Segments;
	}
	
	static constexpr float PI = 3.141592653589793238462643383279502884197169399375f;
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

	static mat WorldMatrix(const vec3& Scale, const vec3& Rotation,
		const vec3& Location)
	{
		mat Temp, World;
		D3DXMatrixScaling(&World, Scale.x, Scale.y, Scale.z);
		D3DXMatrixRotationX(&Temp, MATH::ToRadian(Rotation.x) );
		World *= Temp;
		D3DXMatrixRotationY(&Temp, MATH::ToRadian(Rotation.y) );
		World *= Temp;
		D3DXMatrixRotationZ(&Temp, MATH::ToRadian(Rotation.z) );
		World *= Temp;
		D3DXMatrixTranslation(&Temp, Location.x, Location.y, Location.z);
		World *= Temp;
		return World;
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

END

#define __MYMATH_H__
#endif