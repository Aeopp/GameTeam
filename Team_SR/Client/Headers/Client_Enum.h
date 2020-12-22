#pragma once

#ifndef __CLIENT_ENUM_H__

enum class ESceneID :int8_t
{
	CurrentScene =-1,
	Static=0,
	Logo,
	Menu,
	Stage1st,
	MaxCount
};

enum class ESetupUVDir :int8_t
{
	EAST,
	WEST,
	SOUTH,
	NORTH,
	DIR_END,
};

#define __CLIENT_ENUM_H__
#endif