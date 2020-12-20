#pragma once


#include "Stage.h"

USING(Engine)
class CStageBoss final : public CStage
{
private:
	explicit CStageBoss(LPDIRECT3DDEVICE9 pDevice);
public:
	using Super = CStage;
	virtual HRESULT ReadyScene() override;
	virtual _uint UpdateScene(float fDeltaTime) override;
	virtual _uint LateUpdateScene() override;
protected:
	virtual _uint KeyProcess(float fDeltaTime) override;
	virtual void PlayerKeyProcess(class CPlayer* const _CurrentPlayer, float fDeltaTime)override;
public:
	static CStageBoss* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual void Free() override;
public:
};
