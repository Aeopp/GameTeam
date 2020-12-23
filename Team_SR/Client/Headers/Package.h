#pragma once
#ifndef __PACKAGE_H__

#include "Engine_Include.h"

USING(Engine)

struct Package
{
public:
	vec3 Scale{ 1,1,1 };
	vec3 Rotation{ 0,0,0 };
	vec3 Location{ 0,0,0 };
	std::string Name{};
};

struct MapPlanePackage
{
	vec3 Location{ 0,0,0 };
	vec3 Rotation{ 0,0,0 };
	vec3 Normal{ 0,0,0 };
};

struct PackageContainer
{
public :
	void Initialize(IDirect3DDevice9* const _Device)&noexcept;
	void Release() & noexcept;
	void CurInfoPush();
	void CurInfoFileRecord(const std::string& FileName);
	std::vector<Package> _PackageVec;
	Package _CurEditPackage;
	MapPlanePackage _CurMapPlanePackage;
	std::vector<IDirect3DTexture9*> _MapFloorTexture;
public:
};
#define __PACKAGE_H__
#endif