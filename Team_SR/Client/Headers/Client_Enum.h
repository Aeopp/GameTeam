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

typedef enum class MyEffect {
	BloodHit_Big,	// 큰 출혈
	End
}EFFECT;

typedef enum class Decorator {
	Torch,			// 횃불 - 조명
	Candle,			// 양초 - 조명
	Barrel,			// 나무 통
	BarrelBomb,		// 폭탄 통 - 건드리면 터짐
	BarrelWaste,	// 폐기물 통
	Box1,			// 나무 상자 1
	Box2,			// 나무 상자 2
	BoxSteel1,		// 강철 상자 1
	BoxSteel2,		// 강철 상자 2
	Headstone1,		// 묘비 1
	Headstone2,		// 묘비 2
	Headstone3,		// 묘비 3
	ThornyVine,		// 가시 덩굴 - 가까이가면 데미지를 입긴함
	TreeBlight,		// 죽은 나무
	//------이 위로는 공격받으면 부셔지는 장식물들, 아래로는 안부셔지는 장식물들
	Tree1,			// 나무 1
	Tree2,			// 나무 2
	Dead_Caleb,		// 죽은 갈렙 - 블러드 패러디, 언데드 영웅
	Dead_Doomguy,	// 죽은 둠가이 - 둠 패러디, 악마 학살자
	Dead_Duke,		// 죽은 듀크 - 듀크 뉴켐 패러디, 여자를 혐오하는 상남자
	Dead_Sam,		// 죽은 샘 - 시리어스 샘 패러디, 지구를 침공한 외계인을 학살하는 남자
	Dead_Wang,		// 죽은 왕 - 쉐도우 워리어 패러디, 괴물들을 때려잡는 닌자
	End
}DECO;

#define __CLIENT_ENUM_H__
#endif