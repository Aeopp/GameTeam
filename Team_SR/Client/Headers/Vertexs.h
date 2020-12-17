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
}

#define __VERTEXS_H__
#endif