#pragma once

#ifndef __CLIENT_ENUM_H__

enum class ESceneID :int8_t
{
	CurrentScene =-1,
	Static=0,
	Logo,
	Menu,
	Stage1st,
	Stage2nd,
	Stage3rd,
	Stage4th,
	Stage5th,
	Stage6th,
	Stage7th,
	Stage8th,
	StageMidBoss,
	StageFinalBoss,
	MaxCount
};

#define __CLIENT_ENUM_H__
#endif