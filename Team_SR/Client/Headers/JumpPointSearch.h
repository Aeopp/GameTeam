#pragma once

#ifndef __JUMPPOINTSEARCH_H__
#define __JUMPPOINTSEARCH_H__

#include <list>

using namespace std;

class JumpPointSearch {
private:
	static JumpPointSearch* m_pInstance;
public:
	static JumpPointSearch* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new JumpPointSearch;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	// 길 타입
	enum class PathType {
		EMPT = 0,
		WALL,
		DEPA,
		DEST,
		NODE
	};

private:
	struct stNode
	{
		stNode() : stpParent(nullptr), dwF(0), dwG(0), dwH(0), lPosX(0), lPosY(0), byDirection(0)
		{}

		stNode* stpParent;				// 부모 포인터
		DWORD dwF;						// G + H 합
		DWORD dwG;						// 출발점에서의 거리
		DWORD dwH;						// 도착점에서의 거리
		LONG lPosX;						// 노드 위치 좌표 X
		LONG lPosY;						// 노드 위치 좌표 Y
		BYTE byDirection;				// 탐색 방향
	};

	typedef typename list<stNode*>::iterator iterator;

private:
	explicit JumpPointSearch();
	virtual ~JumpPointSearch();

public:
	void ReadyMap(vector<PathType>& _vecMaps, DWORD _dwTileWidth, DWORD _dwTileHeight, float _fTileSize, DWORD _dwRange);
	BOOL Start(vec3 _vDepa, vec3 _vDest, DWORD _dwRange = 0);
	void Finish(list<vec3>& listMovePos);
	void NearbyPath(vec3 _vDest, vec3& _vDepa);

private:
	void ReleaseJumpPointSearch();
	void JumpPointSearchSort(iterator& _iterFirst, iterator& _iterBegin, iterator& _iterEnd);	//리스트 정렬

	BYTE CheckType(LONG lPosX, LONG lPosY);
	BOOL Find_UU(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode);
	BOOL Find_RR(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode);
	BOOL Find_DD(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode);
	BOOL Find_LL(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode);
	BOOL Find_UR(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode);
	BOOL Find_DR(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode);
	BOOL Find_DL(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode);
	BOOL Find_UL(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode);
	void ConnectNode(stNode* _stpParentNode, stNode* _stpChildNode);			// 부모 노드와 자식 노드 연결
	DWORD GetDistance(LONG _lDepaX, LONG _lDepaY, LONG _lDestX, LONG _lDestY);	// 거리 값 구하기
	void OptimizePath(stNode* _stpFinishNode);	// 길찾기 경로 최적화
	BOOL RayCast(vec3 _vPos, vec3 _vDir, float _fDistance);



private:
	BYTE* m_bypMap;			// 길찾기 전체 바이트 맵
	BYTE* m_bypPathMap;		// 길찾기 할 탐색 범위 맵
	stNode*	m_stpFinishNode;// 길찾기 완료한 도착지 노드

	list<stNode*> m_openList;
	list<stNode*> m_closedList;

	DWORD m_dwTileWidth;	// 타일 가로 개수
	DWORD m_dwTileHeight;	// 타일 세로 개수
	float m_fTileSize;		// 타일 사이즈
	DWORD m_dwRange;		// 길찾기 탐색 영역 범위
	DWORD m_dwPathWidth;	// 길찾기 할 탐색 맵 가로 길이
	DWORD m_dwPathHeight;	// 길찾기 할 탐색 맵 세로 길이
	LONG m_lDepaX;			// 출발지 X
	LONG m_lDepaZ;			// 출발지 Z
	LONG m_lDestX;			// 도착지 X
	LONG m_lDestZ;			// 도착지 Z
};

#endif // !__JUMPPOINTSEARCH_H__
