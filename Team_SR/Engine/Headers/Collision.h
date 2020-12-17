#pragma once

#ifndef __COLLISION_H__
#include "MyMath.h"
BEGIN(Engine)
class   ENGINE_DLL Collision
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
		// ?? ?? ? ?? ?? ?
		float CrossValue;
	};

	// ??? ???? ?? ??? R(t) = Q+V(t) ??? t? ??.
	// 1. ??? ?? ??
	// 2. ??? ?? ??? ??? ??? ??? ?? ??.
	// 1 2 ?? ???? ???.
	//  ????? ??? ??? ??? ?? ??? + ?? ?? * t ?? ?.
	// ???? ???? ?? ????? ???? ??.
	static bool IsTriangleToRay(const PlaneInfo& Lhs, const Ray& Rhs,
		float& t,vec3& IntersectPoint)
	{
		const vec3 Normal = MATH::Normalize({ Lhs._Plane.a ,
								Lhs._Plane.b,
								Lhs._Plane.c });

		const float dot = MATH::Dot(Normal, Rhs.Direction);
		// ??? ????? ???? ?? ? ??.
		if (MATH::almost_equal(dot, 0.f))return false;
		float NQ_D = -(MATH::Dot(Normal, Rhs.Start) + Lhs._Plane.d);

		t = NQ_D /dot	;
		// ??? ?? ???? ????? ? ? ??.
		if (t < 0)return false;

		IntersectPoint = Rhs.Start + Rhs.Direction * t;

		return MATH::InnerPointFromFace(IntersectPoint, Lhs.Face);
	};

	// ?? ??.
	static std::pair<bool, Collision::Info> IsSegmentToSphere(const Segment& Lhs, const Sphere& Rhs,
		float& t0,float& t1/*??? ??? ??????? ??? ?????
				  ???? ??? ?? ???? t? ?? ??? ?? ?? ??*/)
	{
		auto _Info=IsRayToSphere(Lhs._Ray, Rhs,t0,t1);

		bool bInnerSegment =  ( ((t0 >= 0 ) && (t0 <= Lhs.t) && _Info.first) ||
								((t1 >= 0) &&  (t1 <= Lhs.t) && _Info.first) );

		std::pair<bool, Collision::Info> OInfo;
		//??
		if (bInnerSegment)
		{
			OInfo.first = true;
			OInfo.second = std::move(_Info.second);
			return OInfo;
		}

		//const vec3 StartToCenter= Lhs._Ray.Start - Rhs.Center;
		//const vec3 EndToCenter =  (Lhs._Ray.Start + (Lhs._Ray.Direction * Lhs.t)) -Rhs.Center;

		//// ??
		//if ((MATH::Length(StartToCenter) < Rhs.Radius) ||
		//	(MATH::Length(EndToCenter)   < Rhs.Radius))
		//{
		//	OInfo.first = true;
		//	OInfo.second = std::move(_Info.second);
		//	return OInfo;
		//}

		return OInfo;
	}

	// ????? ??? ??? + ??? ?? * t0 or t1 ? ??? ??? ? ?? ? ??.
	// ??? ???? ?? ?? ????? ???? t0 ? ????? t1? ???? ??.
	static std::pair<bool, Collision::Info> IsRayToSphere(const Ray& Lhs ,const Sphere& Rhs,
		float& t0,float& t1 /*??? ??? ??????? ??? ?????
				  ???? ??? ?? ???? t? ?? ??? ?? ?? ??*/)
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
		/*t0 = ??? ??? ?? ???? ??? ???*/
		t0=dot - q;
		/*t1 = t0 ?? ?? ???? ??? ??? ???? ??? ??? ???*/
		t1=dot + q;
		if (Distance < Rhs.Radius && (t0>=0 ||t1>=0) )
		{
			OInfo.first = true;	
			// ??
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
		// ??? ??? ??? ???? ??? ???? ??? ????? ?? ?? 

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

		if (Fomula <= Rhs.Radius/*??*/ && Fomula>=0/*(MATH::Dot(Normal, ToPlaneCenter) < 0.f)*/)
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
