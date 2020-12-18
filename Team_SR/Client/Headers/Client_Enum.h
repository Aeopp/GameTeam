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

typedef enum class Item {
	HealthBig,		// 겁나큰 체력 포션
	HealthSmall,	// 작은 체력 포션
	ManaBig,		// 겁나큰 마나 포션
	ManaSmall,		// 작은 마나 포션
	Ammo,			// 총알
	KeyBlue,		// 파란 열쇠
	KeyRed,			// 빨간 열쇠
	KeyYellow,		// 노랑 열쇠
	Upgrade			// 업그레이드 재화
}ITEM;

#define __CLIENT_ENUM_H__
#endif