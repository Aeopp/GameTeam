#pragma once
#ifndef __DXWRAPPER_H__

#include "Engine_Include.h"
#include "MyMath.h"

USING(Engine)

template<typename _VertexType>
void CreateVertex(
	LPDIRECT3DDEVICE9 _Device,
	const vector<_VertexType>& _VertexVector
	, uint32_t& OutVertexCount,
	uint32_t& OutTriangleCount,
	uint16_t& OutVertexByteSize,
	std::shared_ptr<IDirect3DVertexBuffer9>& OutVertexBuf,
	std::shared_ptr<IDirect3DVertexDeclaration9>& OutVertexDecl)
{
	OutVertexCount = _VertexVector.size();
	OutTriangleCount = OutVertexCount / 3;
	OutVertexByteSize = sizeof(_VertexType);

	IDirect3DVertexBuffer9* _VertexBuffer;
	if (FAILED(_Device->CreateVertexBuffer(OutVertexByteSize * OutVertexCount,
		D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT,
		&_VertexBuffer, 0)))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		return;
	}

	_VertexType* _VertexPtr;
	_VertexBuffer->Lock(0, OutVertexByteSize * OutVertexCount, (void**)&_VertexPtr, 0);
	memcpy(_VertexPtr, _VertexVector.data(), OutVertexByteSize * OutVertexCount);
	_VertexBuffer->Unlock();

	OutVertexDecl = std::shared_ptr<IDirect3DVertexDeclaration9>
		(_VertexType::GetVertexDecl(_Device),
		[](std::shared_ptr<IDirect3DVertexDeclaration9>::element_type* Target)
		{
			Target->Release();
		});

	OutVertexBuf = std::shared_ptr<IDirect3DVertexBuffer9>(_VertexBuffer,
		[](std::shared_ptr<IDirect3DVertexBuffer9>::element_type* Target)
		{
			Target->Release();
		});
}

struct MyLight
{
	static MyLight Make(const vec3 Location, const vec4 Diffuse, const float Radius, const int32_t Priority)
	{
		MyLight _Light;
		_Light.Location = MATH::ConvertVec4(Location, 1.f);
		_Light.Diffuse = Diffuse;
		_Light.Radius = Radius;
		_Light.Priority;
		return _Light;
	};

	vec4 Location = { 0, 0 , 0 , 0 } ;
	vec4 Diffuse = { 1,1,1,1 };
	float Radius =10.f;
	int32_t Priority = (std::numeric_limits<int32_t>::max)();
};

std::vector<IDirect3DTexture9*> CreateTextures (IDirect3DDevice9* const _Device ,
	const std::wstring& Path,
	const size_t TextureNum);

std::vector<std::tuple<IDirect3DTexture9*, IDirect3DTexture9*, IDirect3DTexture9*> >
	CreateTexturesSpecularNormal(IDirect3DDevice9* const _Device,
	const std::wstring& Path,
	const size_t TextureNum);;


struct AnimationTextures
{
	using NotifyType = std::map<uint32_t, std::function<void()> >;
	                                          // Diffuse                 // Specular           // Normal
	std::map<std::wstring,std::vector<std::tuple<IDirect3DTexture9* , IDirect3DTexture9*, IDirect3DTexture9*> >> _TextureMap;
	FORCEINLINE const std::wstring& GetAnimationKey() { return CurrentAnimKey;  };
	FORCEINLINE size_t GetCurrentImgFrame() { return CurrentImgFrame; };
	const std::tuple<IDirect3DTexture9*, IDirect3DTexture9*, IDirect3DTexture9*>& GetCurrentTexture();
	const std::tuple<IDirect3DTexture9*, IDirect3DTexture9*, IDirect3DTexture9*>& GetTexture(const std::wstring& _AnimKey, const size_t _ImgFrame);

	void Release()& noexcept;
	void AddRef()& noexcept;
	void Update(const float DeltaTime);

	void ChangeAnim(
		std::wstring AnimKey, 
		const float AnimDelta,
		const size_t ImgNum,
		const bool bLoop = false,
		NotifyType  SetAnimNotify = {},
		const float InitT = 0.0f, 
		const size_t StartImgFrame = 0ul);
private:
	NotifyType  CurrentAnimNotify;
	std::wstring CurrentAnimKey;
	float AnimDelta = 0.1f;
	float CurrentT = 0.0f;
	size_t CurrentImgFrame = 0ul;
	size_t ImgNum = 1ul;
	bool bLoop = false;
};

struct MtrlInfo
{
	enum class Illumination :uint8_t
	{

	};
public:
	std::wstring TextureName{};
	std::wstring MtrlName{};
	D3DXCOLOR Ambient{ 0,0,0,1.f };
	D3DXCOLOR Diffuse = { 0,0,0,1.f };
	D3DXCOLOR Specular{ 0,0,0,1.f };
	D3DXCOLOR Emissive{ 0,0,0,1.f };
	float Shine{ 0 };
	Illumination Illu;
public:
	D3DMATERIAL9 ConvertMtrl()
	{
		D3DMATERIAL9 _Mtrl;
		_Mtrl.Ambient = this->Ambient;
		_Mtrl.Diffuse = this->Diffuse;
		_Mtrl.Power = this->Shine;
		_Mtrl.Specular = Specular;
		_Mtrl.Emissive = D3DXCOLOR{ 0.f,0.f,0.f,0.f };
		return _Mtrl;
	}
};

struct SubSetInfo
{
public:
	uint32_t TriangleCount = 0;
	MtrlInfo MaterialInfo{};

	LPDIRECT3DVERTEXDECLARATION9 Decl{ nullptr };
	IDirect3DVertexBuffer9* VtxBuf{ nullptr };
	IDirect3DTexture9* Diffuse{ nullptr };
	IDirect3DTexture9* Specular{ nullptr };
	IDirect3DTexture9* Normal{ nullptr };

	static std::shared_ptr<SubSetInfo> MakeShared()noexcept;
	// 경로와 이름까지 입력 확장자는 입력하지 않기.
	static std::shared_ptr<std::vector<SubSetInfo>> GetMeshFromObjFile(IDirect3DDevice9* const _Device, const std::wstring& FilePath)noexcept;
	void Release() & noexcept;
};


LPDIRECT3DTEXTURE9 LOAD_TEXTURE(IDirect3DDevice9* _Device, const std::wstring& FileName);

class Effect
{
public:
	class Info
	{
	public:
		IDirect3DPixelShader9* PsShader{ nullptr };
		ID3DXConstantTable* PsTable{ nullptr };
		IDirect3DVertexShader9* VsShader{ nullptr };
		ID3DXConstantTable* VsTable{ nullptr };
		/*ConstantHandleInitialize 로 세팅하면 편함.*/
		std::map<std::string, D3DXHANDLE> VsHandleMap;
		std::map<std::string, D3DXHANDLE> PsHandleMap;
		std::map<std::string, D3DXCONSTANT_DESC> TextureDescMap;

		void Release();
		void AddRef();
		uint8_t GetTexIdx(const std::string& SamplerName);

		D3DXHANDLE GetVSConstantHandle(const std::string& HandleKey);
		D3DXHANDLE GetPSConstantHandle(const std::string& HandleKey);
		template<typename _Type>
		bool  SetVSConstantData(IDirect3DDevice9* const _Device, const std::string& ConstantHandleMapKey, const _Type& Data , const size_t Num = 1);
		template<typename _Type>
		bool  SetPSConstantData(IDirect3DDevice9* const _Device, const std::string& ConstantHandleMapKey, const _Type& Data, const size_t Num = 1);
	};
public:
	static std::map<std::wstring, Info> _EffectInfoMap;
	static std::vector<MyLight> _CurMapLights;
public:
	static void RegistLight(MyLight _Light)noexcept;
	static void ClearRegisteredLighting() noexcept;
	static Effect::Info& GetEffectFromName(const std::wstring& EffectName);
	static void EffectRelease();
	static void EffectInitialize(IDirect3DDevice9* const _Device);
	static Effect::Info CompileAndCreate(IDirect3DDevice9* _Device, const std::wstring& FileName);
	static void Update(IDirect3DDevice9* const _Device, const vec4& CameraLocation,const vec4& LightLocation);

	static std::map<std::string, D3DXHANDLE> ConstantHandleInitialize(
		ID3DXConstantTable* _ConstantTable,
		const std::vector<std::string>& _ConstantDataNames);

	static std::map<std::string, D3DXCONSTANT_DESC > ConstantHandleDescInitialize(
		ID3DXConstantTable* _ConstantTable,
		const std::vector<std::string>& _ConstantTextureNames
	);
};

// 파일명 hlsl 확장자 없이 파일명만 입력
	// 파일명+VS or PS 형식으로 제한
template<typename _Type>
bool typename Effect::Info::SetVSConstantData(IDirect3DDevice9* const _Device, const std::string& ConstantHandleMapKey, const _Type& Data , const size_t Num  )
{
	const uint32_t DataSize = sizeof(std::decay_t<_Type>) * Num;
#if _DEBUG
	if (!_Device  || DataSize == 0 || ConstantHandleMapKey.empty())  PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
#endif
	if (FAILED(VsTable->SetValue(_Device, VsHandleMap[ConstantHandleMapKey], reinterpret_cast<const void*>(&Data), DataSize)))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		return false;
	}
	else
		return true;
}
template<typename _Type>
bool typename Effect::Info::SetPSConstantData(IDirect3DDevice9* const _Device, const std::string& ConstantHandleMapKey, const _Type& Data ,const size_t Num)
{
	const uint32_t DataSize = sizeof(std::decay_t<_Type>) * Num;
#if _DEBUG
	if (!_Device || DataSize == 0 || ConstantHandleMapKey.empty())
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
	}
#endif
	if (FAILED(PsTable->SetValue(_Device, PsHandleMap[ConstantHandleMapKey], reinterpret_cast<const void*>(&Data), DataSize)))
	{
	//	PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		return false;
	}
	else
		return true;
}

//namespace Effect
//{
//	LPD3DXEFFECT Function(IDirect3DDevice9* _Device, const std::wstring& FileName);
//	LPDIRECT3DTEXTURE9 LoadTexture(IDirect3DDevice9* _Device, const std::wstring& FileName);
//}

struct TempVertexType
{
	vec3 location;
	vec3 TexCoord;
};

namespace Mesh
{
	// 삼각형 3 버텍스 로부터 탄젠트 바이노멀 벡터를 계산.
	std::pair<vec3,vec3/* 1st Tangent, 2nd BiNormal*/> 
		CalculateTangentBinormal(TempVertexType vertex1,
		TempVertexType vertex2, TempVertexType vertex3);
	// 탄젠트와 바이노멀로 새로운 노멀을 계산.
	vec3 CalculateNormal(const vec3 Tangent, const vec3 BiNormal);
}

namespace Color
{
	const D3DXCOLOR      WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR      BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR        RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR      GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR       BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR     YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR       CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR    MAGENTA(D3DCOLOR_XRGB(255, 0, 255));
};

namespace Light
{
	/// <summary>
	/// "Diffuse"><난반사광의 양>
	/// "Ambient"><환경광의 양>
	/// "Specular"><정반사광의 양>
	/// "Emissive"><전반적인 표면의 컬러(물체자체가 발광하는 경우)>
	/// "Power"><정반사광의 날카로운 정도(하이라이트가 강조)>
	D3DMATERIAL9 GetMtrl(D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular,
		D3DXCOLOR Emissive, const float Power);

	const D3DMATERIAL9 WHITE_MTRL = GetMtrl(Color::WHITE, Color::WHITE, Color::WHITE, Color::BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL = GetMtrl(Color::RED, Color::RED, Color::RED, Color::BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL = GetMtrl(Color::GREEN, Color::GREEN, Color::GREEN, Color::BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL = GetMtrl(Color::BLUE, Color::BLUE, Color::BLUE, Color::BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = GetMtrl(Color::YELLOW, Color::YELLOW, Color::YELLOW, Color::BLACK, 2.0f);

	D3DLIGHT9 GetDirectional(const vec3& Direction,const D3DXCOLOR& Color);
	D3DLIGHT9 GetPoint(const vec3& Location, const D3DXCOLOR& Color);
	D3DLIGHT9 GetSpot(const vec3& Location,const vec3& Direction,const D3DXCOLOR & Color);
};


#define __DXWRAPPER_H__
#endif