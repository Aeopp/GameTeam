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

#define __CLIENT_ENUM_H__
#endif