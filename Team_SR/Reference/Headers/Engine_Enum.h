#pragma once

#ifndef __ENGINE_ENUM_H__

BEGIN(Engine)

enum class EDisplayMode
{ 
	Full,
	Window
};

enum class ERenderID
{
	Priority,
	NoAlpha,
	Alpha,
	UI,
	MaxCount
};

enum class ETextureType
{
	Normal,
	Cube
};

END

#define __ENGINE_ENUM_H__
#endif