#include "stdafx.h"
#include "Package.h"
#include "DXWrapper.h"

void PackageContainer::Initialize(IDirect3DDevice9* const _Device) & noexcept
{
	_MapFloorTexture = CreateTextures(_Device, L"..\\Resources\\Texture\\Map\\Floor\\", 53);
	/*_MapFloorTexture = CreateTextures(_Device, L"..\\Resources\\Texture\\Map\\Wall\\", 53);
	_MapFloorTexture = CreateTextures(_Device, L"..\\Resources\\Texture\\Map\\Wall\\", 53);*/
}

void PackageContainer::Release() & noexcept
{
	for (auto&  _CurMapTex :_MapFloorTexture)
	{
		SafeRelease(_CurMapTex);
	}
}

void PackageContainer::CurInfoPush()
{
	_PackageVec.push_back(_CurEditPackage);
};

void PackageContainer::CurInfoFileRecord(const std::string& FileName)
{
	std::wofstream Out(FileName);

	if (!Out.is_open()){PRINT_LOG(__FUNCTIONW__, __FUNCTIONW__);	}

	for (const auto& _Package : _PackageVec)
	{
		std::wstring WName(std::begin(_Package.Name), std::end(_Package.Name));

		Out << L"Name " << WName << std::endl;
		Out << L"Location " << _Package.Location .x << " " << _Package.Location.y << " "<<_Package.Location.z<< std::endl;
		Out << L"Scale " << _Package.Scale.x << " " << _Package.Scale.y << " " << _Package.Scale.z << std::endl;
		Out << L"Rotation " << _Package.Rotation.x << " " << _Package.Rotation.y << " " << _Package.Rotation.z << std::endl;

		Out << std::endl;
	};
}
