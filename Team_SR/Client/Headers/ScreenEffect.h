#pragma once
#ifndef __SCREEN_EFFECT_H__
#include "GameObject.h"

USING(Engine)
class CScreenEffect final : public CGameObject
{
private:
	explicit CScreenEffect(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScreenEffect() = default;
public:
	using Super = CGameObject;
	// CMonster을(를) 통해 상속됨
	virtual HRESULT ReadyGameObjectPrototype() override;
	virtual HRESULT ReadyGameObject(void * pArg = nullptr) override;
	virtual _uint UpdateGameObject(float fDeltaTime) override;
	virtual _uint LateUpdateGameObject(float fDeltaTime) override;
	virtual HRESULT RenderGameObject() override;
public:
	static CScreenEffect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone(void * pArg = nullptr) override;
	virtual void Free() override;
	void ItemInteractionEffect();
	void BloodEffect();
	void FreezeEffect();
	void Blur()&;
	void Shield(const uint8_t ImgIdx)&;
private:
	uint8_t ShieldImgIdx{8l};
	float FreezeTime = 0.0f;
	float BlurTime = 0.0f;
	float IteminteractionEffecT = 0.0f;
	float BloodEffectT = 0.0f;
	uint32_t VertexCount;
	uint32_t TriangleCount;
	uint16_t VertexByteSize;
	std::shared_ptr<IDirect3DVertexBuffer9> VertexBuf;
	std::shared_ptr<IDirect3DVertexDeclaration9> VertexDecl;
	std::map<std::wstring,std::shared_ptr<IDirect3DTexture9>> Textures;
};

#define __SCREEN_EFFECT_H__
#endif