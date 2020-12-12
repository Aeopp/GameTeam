#pragma once

#ifndef __CLINET_TYPEDEF_H__

typedef bool _bool;
typedef char _char;
typedef int _int;

typedef unsigned char _uchar;
typedef unsigned int _uint;

typedef D3DXVECTOR3 _vector;
typedef D3DXMATRIX _matrix;

using vec4 = D3DXVECTOR4; 
using vec3 = D3DXVECTOR3;
using vec2 = D3DXVECTOR2;
using mat  = D3DXMATRIX;

#define __CLINET_TYPEDEF_H__
#endif