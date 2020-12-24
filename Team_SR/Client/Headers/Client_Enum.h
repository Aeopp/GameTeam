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
	HealthBig,		// �̳�ū ü�� ����
	HealthSmall,	// ���� ü�� ����
	ManaBig,		// �̳�ū ���� ����
	ManaSmall,		// ���� ���� ����
	Ammo,			// �Ѿ�
	KeyBlue,		// �Ķ� ����
	KeyRed,			// ���� ����
	KeyYellow,		// ��� ����
	Upgrade			// ���׷��̵� ��ȭ
}ITEM;

typedef enum class MyEffect {
	BloodHit_Big,	// ū ����
	End
}EFFECT;

typedef enum class Decorator {
	Torch,			// ȶ�� - ����
	Candle,			// ���� - ����
	Barrel,			// ���� ��
	BarrelBomb,		// ��ź �� - �ǵ帮�� ����
	BarrelWaste,	// ��⹰ ��
	Box1,			// ���� ���� 1
	Box2,			// ���� ���� 2
	BoxSteel1,		// ��ö ���� 1
	BoxSteel2,		// ��ö ���� 2
	Headstone1,		// ���� 1
	Headstone2,		// ���� 2
	Headstone3,		// ���� 3
	ThornyVine,		// ���� ���� - �����̰��� �������� �Ա���
	TreeBlight,		// ���� ����
	//------�� ���δ� ���ݹ����� �μ����� ��Ĺ���, �Ʒ��δ� �Ⱥμ����� ��Ĺ���
	Tree1,			// ���� 1
	Tree2,			// ���� 2
	Dead_Caleb,		// ���� ���� - ������ �з���, �𵥵� ����
	Dead_Doomguy,	// ���� �Ұ��� - �� �з���, �Ǹ� �л���
	Dead_Duke,		// ���� ��ũ - ��ũ ���� �з���, ���ڸ� �����ϴ� ����
	Dead_Sam,		// ���� �� - �ø�� �� �з���, ������ ħ���� �ܰ����� �л��ϴ� ����
	Dead_Wang,		// ���� �� - ������ ������ �з���, �������� ������� ����
	End
}DECO;
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