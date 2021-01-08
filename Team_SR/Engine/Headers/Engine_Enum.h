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
	ScreenPostEffect,
	MaxCount
};

enum class ETextureType
{
	Normal,
	Cube
};

enum class ERenderPlace
{
	FRONT,
	BACK,
	NONE_PLACE,
};

END

#define __ENGINE_ENUM_H__
#endif