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

#define __CLIENT_ENUM_H__
#endif