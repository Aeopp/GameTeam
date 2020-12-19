#include "stdafx.h"
#include "..\Headers\NormalUVVertexBuffer.h"
#include "..\Headers\VIBuffer.h"

USING(Engine)

CNormalUVVertexBuffer::CNormalUVVertexBuffer(LPDIRECT3DDEVICE9 pDevice) :
	Super(pDevice ) 
{
}

HRESULT CNormalUVVertexBuffer::ReadyComponentPrototype()
{
	std::vector<Vertex::Texture> _Vertexs(6);

	for (size_t i = 0; i < VertexCount; ++i)
	{
		_Vertexs[i].Normal = { 0.f,0.f,-1.f };
	}

	_Vertexs[0].TexCoord = { 0.012f,0.988f };
	_Vertexs[0].Location = { -0.5f,-0.5f,0.f };
	_Vertexs[1].TexCoord = { 0.012f,0.012f };
	_Vertexs[1].Location = { -0.5f,+0.5f,0.f };
	_Vertexs[2].TexCoord = { 0.988f,0.012f };
	_Vertexs[2].Location = { +0.5f,+0.5f,0.f };
	_Vertexs[3].TexCoord = { 0.012f,0.988f };
	_Vertexs[3].Location = { -0.5f,-0.5f,0.f };
	_Vertexs[4].TexCoord = { 0.988f,0.012f };
	_Vertexs[4].Location = { +0.5f,+0.5f,0.f };
	_Vertexs[5].TexCoord = { 0.988f,0.988f };
	_Vertexs[5].Location = { +0.5f,-0.5f,0.f };

	CreateVertex(m_pDevice, _Vertexs, VertexCount, TriangleCount, VertexByteSize, _VertexBuf, VertexDecl);
	Vertex::Texture* _VertexPtr = nullptr;
	_VertexBuf->Lock(0, 0, (void**)&_VertexPtr, 0);

	for (size_t i = 0; i < VertexCount; i += 3)
	{
		const auto Tangent_BiNormal = Mesh::CalculateTangentBinormal(
			TempVertexType{ _VertexPtr[i].Location,  _VertexPtr[i].TexCoord },
			TempVertexType{ _VertexPtr[i + 1].Location,_VertexPtr[i + 1].TexCoord },
			TempVertexType{ _VertexPtr[i + 2].Location,_VertexPtr[i + 2].TexCoord });

		const vec3 Normal = Mesh::CalculateNormal(Tangent_BiNormal.first, Tangent_BiNormal.second);

		for (size_t j = i; j <= (i + 2); ++j)
		{
			_VertexPtr[j].Normal = Normal;
			_VertexPtr[j].Tangent = Tangent_BiNormal.first;
			_VertexPtr[j].BiNormal = Tangent_BiNormal.second;
		};
	};
	
	//  
	//VertexCount = 6;
	//TriangleCount = 2;
	//VertexByteSize = sizeof(Vertex::Texture);

	//IDirect3DVertexBuffer9* _VertexBuffer;
	//if (FAILED(m_pDevice->CreateVertexBuffer(VertexByteSize * VertexCount,
	//	D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT,
	//	&_VertexBuffer, 0)))
	//{
	//	PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
	//	return E_FAIL;
	//}

	//Vertex::Texture* _VertexPtr;

	//_VertexBuffer->Lock(0, sizeof(Vertex::Texture) * VertexCount, (void**)&_VertexPtr, 0);

	//for (size_t i = 0; i < VertexCount; ++i)
	//{
	//	_VertexPtr[i].Normal = { 0.f,0.f,-1.f };
	//}

	//_VertexPtr[0].TexCoord = { 0.f,1.f };
	//_VertexPtr[0].Location = { -0.5f,-0.5f,0.f };

	//_VertexPtr[1].TexCoord = { 0.f,0.f };
	//_VertexPtr[1].Location = { -0.5f,+0.5f,0.f };

	//_VertexPtr[2].TexCoord = { 1.f,0.f };
	//_VertexPtr[2].Location = { +0.5f,+0.5f,0.f };

	//_VertexPtr[3].TexCoord = { 0.f,1.f };
	//_VertexPtr[3].Location = { -0.5f,-0.5f,0.f };

	//_VertexPtr[4].TexCoord = { 1.f,0.f };
	//_VertexPtr[4].Location = { +0.5f,+0.5f,0.f };

	//_VertexPtr[5].TexCoord = { 1.f,1.f };
	//_VertexPtr[5].Location = { +0.5f,-0.5f,0.f };

	//for (size_t i = 0; i < VertexCount; i += 3)
	//{
	//	const auto Tangent_BiNormal = Mesh::CalculateTangentBinormal(
	//		TempVertexType{ _VertexPtr[i].Location,  _VertexPtr[i].TexCoord },
	//		TempVertexType{ _VertexPtr[i+1].Location,_VertexPtr[i+1].TexCoord },
	//		TempVertexType{ _VertexPtr[i+2].Location,_VertexPtr[i+2].TexCoord });
	//	const vec3 Normal = Mesh::CalculateNormal(Tangent_BiNormal.first, Tangent_BiNormal.second);

	//	for (size_t j = i; j <= (i + 2); ++j)
	//	{
	//		_VertexPtr[j].Normal = Normal;
	//		_VertexPtr[j].Tangent = Tangent_BiNormal.first;
	//		_VertexPtr[j].BiNormal = Tangent_BiNormal.second;
	//	};
	//};

	//_VertexBuffer->Unlock();


	//VertexDecl = decltype(VertexDecl)(Vertex::Texture::GetVertexDecl(m_pDevice),
	//	[](decltype(VertexDecl)::element_type* Target)
	//	{
	//		Target->Release();
	//	});

	//_VertexBuf = decltype(_VertexBuf)(_VertexBuffer,
	//	[](decltype(_VertexBuf)::element_type* Target)
	//	{
	//		Target->Release();
	//	});





	return S_OK;
};

HRESULT CNormalUVVertexBuffer::ReadyComponent(void* pArg)
{
	return S_OK;
};

HRESULT CNormalUVVertexBuffer::Render()
{
	m_pDevice->SetStreamSource(0, _VertexBuf.get(), 0, VertexByteSize);
	m_pDevice->SetVertexDeclaration(VertexDecl.get());
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, TriangleCount);

	return S_OK;
}

CNormalUVVertexBuffer* CNormalUVVertexBuffer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CNormalUVVertexBuffer* pInstance = new CNormalUVVertexBuffer(pDevice);
	if (FAILED(pInstance->ReadyComponentPrototype()))
	{
		PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent* CNormalUVVertexBuffer::Clone(void* pArg)
{
	CNormalUVVertexBuffer* pClone = new CNormalUVVertexBuffer(*this);
	SafeAddRef(m_pDevice);
	if (FAILED(pClone->ReadyComponent(pArg)))
	{
		PRINT_LOG(__FUNCTIONW__ , __FUNCTIONW__);
		SafeRelease(pClone);
	}

	return pClone;
}

void CNormalUVVertexBuffer::Free()
{
	Super::Free();
}
