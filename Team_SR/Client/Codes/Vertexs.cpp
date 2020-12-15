﻿#include "stdafx.h"
#include "Vertexs.h"

LPDIRECT3DVERTEXDECLARATION9 typename Vertex::Texture::GetVertexDecl(IDirect3DDevice9* _Device)
{
	LPDIRECT3DVERTEXDECLARATION9 VertexDeclaration;
	D3DVERTEXELEMENT9 decl[] =
	{
		// 아래 주석 부분 4가지만 신경써서 정의해주면 됨.
		{ 0, 0/*선두로부터의 바이트 시작 오프셋*/, /*데이터 타입*/D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT, /*표현식*/ D3DDECLUSAGE_POSITION, 0/*인덱스*/ },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	_Device->CreateVertexDeclaration(decl, &VertexDeclaration);
	return VertexDeclaration;
}

