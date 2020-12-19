#include "stdafx.h"
#include "Vertexs.h"

LPDIRECT3DVERTEXDECLARATION9 typename Vertex::Texture::GetVertexDecl(IDirect3DDevice9* _Device)
{
	LPDIRECT3DVERTEXDECLARATION9 VertexDeclaration;
	D3DVERTEXELEMENT9 decl[] =
	{
		// ?? ?? ?? 4??? ???? ????? ?.
		{ 0, 0/*?????? ??? ?? ???*/, /*??? ??*/D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT, /*???*/ D3DDECLUSAGE_POSITION, 0/*???*/ },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	_Device->CreateVertexDeclaration(decl, &VertexDeclaration);
	return VertexDeclaration;
};

const uint32_t typename Vertex::CubeTexture::FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
const D3DFORMAT Index::_16_t::Format = D3DFMT_INDEX16;





