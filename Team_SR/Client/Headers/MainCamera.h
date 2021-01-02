#pragma once
#ifndef __MAINCAMERA_H__

#include "Camera.h"

USING(Engine)
class CMainCamera  final : public CCamera
{
private:
	explicit CMainCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CMainCamera() = default;
public:
	using Super = CCamera;
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	static CMainCamera* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
public:
	bool bThirdPerson = false;
	float FirstPersonRotationSpeed = 10.f;
	void Shake(const float Force,const vec3 Dir, const float Duration);
private:
	vec3 Offset{ 0,0,-20};
	vec3 OffsetRotation{ 0,0,0 };
private:
	void CameraEditProcess()&;
private:
	struct ShakeInfo
	{
		float Force{ 1.f };
		vec3 Dir{ 1,0,0 };
		float Duration{ 0.1f };
	};
	std::vector<typename CMainCamera::ShakeInfo> ShakeInfos{};
	vec3 CurrentShake{ 0,0 , 0 };
	void CalcCurrentShake(const float DeltaTime);
};

#define __MAINCAMERA_H__
#endif