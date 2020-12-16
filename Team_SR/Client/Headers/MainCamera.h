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
	void Shake(const float Duration,
				const float Force,
				vec3 AxisScale,
					
					const float Vibration);
	bool bThirdPerson = true;
	float FirstPersonRotationSpeed = 5.f;
private:
	struct ShakeInfo
	{
		float Duration{ 0.f };
		float Force{ 1.f };
		float Vibration{ 1.f };
		vec3 AxisScale{ 1,1,1 };
		vec3 Goal{ 0,0,0 };
	};
	vec3 Offset{ 0,0,-20};
	vec3 OffsetRotation{ 0,0,0 };

private:
	void CameraEditProcess()&;
	void Shaking(const float DeltaTime);
	ShakeInfo _CurrentShake;
};

#define __MAINCAMERA_H__
#endif