#pragma once
#ifndef __VERTEXS_H__

#include "Engine_Include.h"

USING(Engine)

namespace Vertex
{
	struct Texture
	{
		vec3 Location;
		vec3 Normal;
		vec3 Tangent;
		vec3 BiNormal;
		vec2 TexCoord;
		static LPDIRECT3DVERTEXDECLARATION9 GetVertexDecl(IDirect3DDevice9* _Device);
	};

	struct CubeTexture
	{
		vec3 Location;
		vec3 UV;
		static const uint32_t FVF;
	};
}

namespace Index
{
	struct _16_t
	{
		WORD _1, _2, _3;
		static const D3DFORMAT Format;
	};
}

#define __VERTEXS_H__
#endif