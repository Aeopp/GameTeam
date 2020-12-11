#pragma once

#ifndef __ENGINE_STRUCT_H__

BEGIN(Engine)

struct Ray
{
	vec3 Start;
	vec3 Direction;
};

struct Segment
{
	Ray _Ray;

	// 시작점에서 끝점 까지의 거리
	// ex) startpoint + dir * t = endpoint
	float t;
};

struct PlaneInfo
{
	static PlaneInfo Make(std::array<vec3, 3ul> Face)
	{
		PlaneInfo _Info;
		_Info.Face = std::move(Face);
		_Info.Center = (Face[0] + Face[1] + Face[2]) / Face.size();
		D3DXPlaneFromPoints(&_Info._Plane, &_Info.Face[0], &_Info.Face[1], &_Info.Face[2]);
		return _Info;
	};
	D3DXPLANE _Plane;
	vec3 Center;
	// 로컬이 아닌 월드 기준
	std::array<vec3, 3ul> Face;
};

struct Sphere
{
	float Radius;
	vec3 Center;
};

typedef struct tagVertexColor
{
	D3DXVECTOR3 vPosition;
	_uint iColor;
}VTX_COLOR;

const _uint FVF_VTX_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertexTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR2 vUV; /* 텍스처 한장의 UV좌표 범위는 0,0 ~ 1,1 */
}VTX_TEXTURE;

const _uint FVF_VTX_TEXTURE = D3DFVF_XYZ | D3DFVF_TEX1 /*| D3DFVF_TEXCOORDSIZE2(0)*/;

typedef struct tagVertexCubeTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vUV;
}VTXCUBE_TEXTURE;

const _uint FVF_VTXCUBE_TEXTURE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef struct tagIndex16
{
	WORD _1, _2, _3;
}INDEX16;

typedef struct tagIndex32
{
	DWORD _1, _2, _3;
}INDEX32;

typedef struct tagTransformDesc
{
	tagTransformDesc()
	{
		ZeroMemory(this, sizeof(tagTransformDesc));
		D3DXMatrixIdentity(&matWorld);
	}
	_vector	vPosition;
	_vector	vScale;
	_vector	vRotation; // For.Degree
	_matrix matWorld;

	float fSpeedPerSec = 0.f;
	float fRotatePerSec = 0.f;
}TRANSFORM_DESC;

typedef struct tagCameraDesc
{
	tagCameraDesc()
	{
		ZeroMemory(this, sizeof(tagCameraDesc));
		D3DXMatrixIdentity(&matView);
		D3DXMatrixIdentity(&matProj);
	}
	_matrix	matView;
	_vector vEye;
	_vector vAt;
	_vector vUp;

	_matrix matProj;
	float	fFovY;	// Degree
	float	fAspect;
	float	fNear;
	float	fFar;
}CAMERA_DESC;

END

#define __ENGINE_STRUCT_H__
#endif