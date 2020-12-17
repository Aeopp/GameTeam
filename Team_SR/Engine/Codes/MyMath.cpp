#include "MyMath.h"

USING(Engine)
std::random_device MATH::Rd{};
std::mt19937 MATH::gen{};
const _vector MATH::AxisX{ 1,0,0 };
const _vector MATH::AxisY{ 0,1,0 };
const _vector MATH::AxisZ{ 0,0,1 };

// 삼각형을 둘러치는 선분 3개.


// 임의의 위치 벡터를 평면에 투영시킨 위치 벡터를 반환.

 vec3 MATH::GetNormalFromFace(const vec3& p0, const vec3& p1, const vec3& p2)
{
	const vec3 u = p1 - p0;
	const vec3 v = p2 - p0;
	return MATH::Normalize(MATH::Cross(u, v));
}

  vec3 MATH::Mul(const vec3& Lhs, const mat& Rhs)
 {
	 vec3 Return;
	 D3DXVec3TransformCoord(&Return, &Lhs, &Rhs);
	 return Return;
 }

  vec3 MATH::MulNormal(const vec3& Lhs, const mat& Rhs)
 {
	 vec3 Return;
	 D3DXVec3TransformNormal(&Return, &Lhs, &Rhs);
	 return Return;
 }

Ray MATH::GetRayScreenProjection(const vec3& ScreenPos, IDirect3DDevice9* const _Device, const float Width, const float Height)
{
	vec3 Dir{ (ScreenPos.x),(ScreenPos.y),1.f };
	const float xfactor = (2.f / Width);
	const float yfactor = -(2.f / Height);
	Dir.x = Dir.x * xfactor - 1.f;
	Dir.y = Dir.y * yfactor + 1.f;
	mat Proj, InvView;
	_Device->GetTransform(D3DTS_PROJECTION, &Proj);
	Dir.x /= Proj(0, 0);
	Dir.y /= Proj(1, 1);
	Dir = MATH::Normalize(Dir);
	_Device->GetTransform(D3DTS_VIEW, &InvView);
	InvView = MATH::Inverse(InvView);
	Dir = MATH::MulNormal(Dir, InvView);
	vec3 Origin = { InvView(3,0),InvView(3,1),InvView(3,2) };
	Ray _Ray;
	_Ray.Start = std::move(Origin);
	_Ray.Direction = std::move(Dir);
	return _Ray;
}

bool MATH::InnerPointFromFace(const vec3& Point, const std::array<vec3, 3ul>& Face)
{
	std::array <vec3, 3ul> ToVertexs;

	for (size_t i = 0; i < ToVertexs.size(); ++i)
	{
		ToVertexs[i] = MATH::Normalize(Face[i] - Point);
	}

	float Radian = 0;
	Radian += std::acosf(MATH::Dot(ToVertexs[0], ToVertexs[1]));
	Radian += std::acosf(MATH::Dot(ToVertexs[1], ToVertexs[2]));
	Radian += std::acosf(MATH::Dot(ToVertexs[2], ToVertexs[0]));

	return almost_equal(Radian, MATH::PI * 2.f);
}

vec3 MATH::ProjectionPointFromFace(D3DXPLANE _Plane, const vec3& Point)
{
	vec3 Normal = { _Plane.a,_Plane.b,_Plane.c };
	Normal = MATH::Normalize(Normal);
	const float distance = MATH::Dot(Normal, Point) + _Plane.d;
	Normal = -Normal;
	return Point + (Normal * distance);
}

std::array<Segment, 3ul> MATH::MakeSegmentFromFace(const std::array<vec3, 3ul>& Face)
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

 // 해당 축으로 Lhs 을 회전시켜서 리턴
 // D3DXVec3TransformCoord 함수를 사용함을 유의.

 _vector MATH::RotationVec(const _vector& Lhs, const _vector& Axis, const float Degree)
 {
	 _matrix Rotation;
	 D3DXMatrixRotationAxis(&Rotation, &Axis, ToRadian(Degree));
	 _vector RotationVec;
	 D3DXVec3TransformCoord(&RotationVec, &Lhs, &Rotation);
	 return RotationVec;
 }

 mat MATH::WorldMatrix(const vec3& Scale, const vec3& Rotation, const vec3& Location)
 {
	 mat Temp, World;
	 D3DXMatrixScaling(&World, Scale.x, Scale.y, Scale.z);
	 D3DXMatrixRotationX(&Temp, MATH::ToRadian(Rotation.x));
	 World *= Temp;
	 D3DXMatrixRotationY(&Temp, MATH::ToRadian(Rotation.y));
	 World *= Temp;
	 D3DXMatrixRotationZ(&Temp, MATH::ToRadian(Rotation.z));
	 World *= Temp;
	 D3DXMatrixTranslation(&Temp, Location.x, Location.y, Location.z);
	 World *= Temp;
	 return World;
 }

  int32_t MATH::RandInt(const std::pair<int32_t, int32_t>& Range)
 {
	 std::uniform_int_distribution<int32_t> Dis(Range.first, Range.second);
	 return Dis(gen);
 }

 float MATH::RandReal(const std::pair<float, float>& Range)
 {
	 std::uniform_real_distribution<float> Dis(Range.first, Range.second);
	 return Dis(gen);
 }

 vec3 MATH::RandVec()
 {
	 return MATH::Normalize(vec3{
		 RandReal({ -1.f,1.f }),
		 RandReal({ -1.f,1.f }),
		 RandReal({ -1.f,1.f })
		 });
 }
