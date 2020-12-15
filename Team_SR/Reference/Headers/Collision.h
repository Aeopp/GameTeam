#pragma once

#ifndef __COLLISION_H__
#include "MyMath.h"
BEGIN(Engine)
class  Collision
{
public:
	struct Info
	{
	public:
		static Info Make(const vec3& Dir, const float CrossValue)
		{
			Info _Info;
			_Info.Dir = Dir;
			_Info.CrossValue = CrossValue;
			return _Info;
		};
		// Lhs -> Rhs
		vec3 Dir;
		// 많이 교차 할 수록 값이 큼
		float CrossValue;
	};

	// 평면과 반직선의 충돌 여부와 R(t) = Q+V(t) 에서의 t를 반환.
	// 1. 평면과 충돌 검사
	// 2. 평면과 충돌 한다면 삼각형 내부에 있는지 까지 검사.
	// 1 2 둘다 만족해야 충돌임.
	//  충돌한다면 교점을 구하는 방법은 광선 시작점 + 광선 방향 * t 하면 됨.
	// 충돌하지 않는다면 반환 매개변수는 유효하지 않음.
	static bool IsTriangleToRay(const PlaneInfo& Lhs, const Ray& Rhs,
		float& t,vec3& IntersectPoint)
	{
		const vec3 Normal = MATH::Normalize({ Lhs._Plane.a ,
								Lhs._Plane.b,
								Lhs._Plane.c });

		const float dot = MATH::Dot(Normal, Rhs.Direction);
		// 평면과 방향벡터는 평행하다 만날 수 없다.
		if (MATH::almost_equal(dot, 0.f))return false;
		float NQ_D = -(MATH::Dot(Normal, Rhs.Start) + Lhs._Plane.d);

		t = NQ_D /dot	;
		// 교점이 뒤에 생기므로 충돌한다고 볼 수 없다.
		if (t < 0)return false;

		IntersectPoint = Rhs.Start + Rhs.Direction * t;

		return MATH::InnerPointFromFace(IntersectPoint, Lhs.Face);
	};

	// 작성 바람.
	static std::pair<bool, Collision::Info> IsSegmentToSphere(const Segment& Lhs, const Sphere& Rhs,
		float& t0,float& t1/*구체가 광선의 시작점으로부터 얼마나 가까운가?
				  여러개가 동시에 피킹 되었다면 t가 작은 물체로 하는 것이 좋음*/)
	{
		auto _Info=IsRayToSphere(Lhs._Ray, Rhs,t0,t1);

		bool bInnerSegment =  ( ((t0 >= 0 ) && (t0 <= Lhs.t) && _Info.first) ||
								((t1 >= 0) &&  (t1 <= Lhs.t) && _Info.first) );

		std::pair<bool, Collision::Info> OInfo;
		//충돌
		if (bInnerSegment)
		{
			OInfo.first = true;
			OInfo.second = std::move(_Info.second);
			return OInfo;
		}

		//const vec3 StartToCenter= Lhs._Ray.Start - Rhs.Center;
		//const vec3 EndToCenter =  (Lhs._Ray.Start + (Lhs._Ray.Direction * Lhs.t)) -Rhs.Center;

		//// 충돌
		//if ((MATH::Length(StartToCenter) < Rhs.Radius) ||
		//	(MATH::Length(EndToCenter)   < Rhs.Radius))
		//{
		//	OInfo.first = true;
		//	OInfo.second = std::move(_Info.second);
		//	return OInfo;
		//}

		return OInfo;
	}

	// 교차한다면 광선의 시작점 + 광선의 방향 * t0 or t1 을 하여서 교차점 을 구할 수 있음.
	// 광선의 시작점이 구체 안에 존재한다면 교차점은 t0 만 교차점이고 t1은 교차점이 아님.
	static std::pair<bool, Collision::Info> IsRayToSphere(const Ray& Lhs ,const Sphere& Rhs,
		float& t0,float& t1 /*구체가 광선의 시작점으로부터 얼마나 가까운가?
				  여러개가 동시에 피킹 되었다면 t가 작은 물체로 하는 것이 좋음*/)
	{
		const vec3 RayStartToCenter = Rhs.Center - Lhs.Start;
		const float dot =MATH::Dot(Lhs.Direction, RayStartToCenter);
		const vec3 IntersectPoint = Lhs.Start + Lhs.Direction * dot;
		vec3 ItPtToCenter  =IntersectPoint - Rhs.Center;
		const float Distance =MATH::Length(ItPtToCenter);

		std::pair<bool, Collision::Info> OInfo;
		OInfo.first = false;

		OInfo.second.CrossValue = Rhs.Radius - Distance;
		OInfo.second.Dir = MATH::Normalize(Rhs.Center - IntersectPoint);
		const float m = MATH::Length(IntersectPoint - Rhs.Center);
		const float q = std::sqrtf((Rhs.Radius * Rhs.Radius) - (m * m));
		/*t0 = 광선과 구체의 호의 외곽선의 첫번째 교차점*/
		t0=dot - q;
		/*t1 = t0 에서 이미 교차하고 광선이 구체를 벗어나기 직전의 두번째 교차점*/
		t1=dot + q;
		if (Distance < Rhs.Radius && (t0>=0 ||t1>=0) )
		{
			OInfo.first = true;	
			// 충돌
		}
		else
		{
			OInfo.first = false;
		}
		return OInfo;
	}

	static std::pair<bool, Collision::Info> IsSphereToSphere(const Sphere& Lhs, const Sphere& Rhs)
	{
		const vec3 ToRhs = Rhs.Center - Lhs.Center;
		const float Distance = MATH::Length(ToRhs);
		const float RadiusSum = Lhs.Radius + Rhs.Radius;

		std::pair<bool, Collision::Info> OInfo{ false,Collision::Info{} };

		if (Distance < RadiusSum)
		{
			OInfo.first = true;
			OInfo.second = Collision::Info::Make(MATH::Normalize(ToRhs), RadiusSum - Distance);
			return OInfo;
		}
		else
		{
			return OInfo;
		}
	};

	static std::pair<bool, Collision::Info> IsPlaneToSphere(const PlaneInfo& Lhs,
		const Sphere& Rhs)
	{
		// 평면의 법선과 구체의 센터에서 평면을 바라보는 벡터가 마주볼때만 충돌 수행 

		//FLOAT a, b, c, d;
		// D3DXPLANE;

		/*D3DXPLANE _Plane;
		vec3 Center;*/
		vec3 Normal = { Lhs._Plane.a, Lhs._Plane.b,Lhs._Plane.c };
		const float d = Lhs._Plane.d;
		const float Fomula = MATH::Dot(Normal, Rhs.Center) + d;
		vec3 ToPlaneCenter = Lhs.Center - Rhs.Center;

		std::pair<bool, Collision::Info> OInfo;
		OInfo.first = false;

		if (Fomula <= Rhs.Radius/*충돌*/ && Fomula>=0/*(MATH::Dot(Normal, ToPlaneCenter) < 0.f)*/)
		{
			Normal = MATH::Normalize(Normal);
			OInfo.first = true;
			OInfo.second = Collision::Info::Make(Normal, Rhs.Radius - Fomula);
			return OInfo;
		}

		return OInfo;
	};
};
END


#define __COLLISION_H__
#endif
