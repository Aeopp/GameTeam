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

};

struct PackageContainer
{
	std::vector<Package> _PackageVec;
	Package _CurEditPackage;
};
#define __PACKAGE_H__
#endif