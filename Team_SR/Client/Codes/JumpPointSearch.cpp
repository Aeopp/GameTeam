#include "stdafx.h"
#include "JumpPointSearch.h"

#define DIRECTION_UU 1
#define DIRECTION_UR 2
#define DIRECTION_RR 4
#define DIRECTION_DR 8
#define DIRECTION_DD 16
#define DIRECTION_DL 32
#define DIRECTION_LL 64
#define DIRECTION_UL 128
#define DIRECTION_ALL 255;

JumpPointSearch* JumpPointSearch::m_pInstance = nullptr;

JumpPointSearch::JumpPointSearch()
	: m_bypMap(nullptr), m_bypPathMap(nullptr), m_stpFinishNode(nullptr), m_dwTileWidth(0), m_dwTileHeight(0),
	m_fTileSize(0.f), m_dwRange(0), m_dwPathWidth(0), m_dwPathHeight(0),
	m_lDepaX(0), m_lDepaZ(0), m_lDestX(0), m_lDestZ(0)
{
}

JumpPointSearch::~JumpPointSearch()
{
	ReleaseJumpPointSearch();
}

// _vecMaps : 맵 타일 타입
// _dwTileWidth : 맵 타일 가로 개수
// _dwTileHeight : 맵 타일 세로 개수
// _dwTileSize : 타일 사이즈
// _dwRange : 길찾기 범위
void JumpPointSearch::ReadyMap(vector<PathType>& _vecMaps, DWORD _dwTileWidth, DWORD _dwTileHeight, float _fTileSize, DWORD _dwRange)
{
	m_dwTileWidth = _dwTileWidth;
	m_dwTileHeight = _dwTileHeight;
	DWORD dwEndIndex = _dwTileWidth * _dwTileHeight;
	m_fTileSize = _fTileSize;
	m_dwRange = _dwRange;

	SafeDeleteArray(m_bypMap);			// 이전에 준비된 맵 정보 정리
	m_bypMap = new BYTE[dwEndIndex];	// 길찾기 알고리즘에서 사용할 BYTE 맵

	for (DWORD dwCnt = 0; dwCnt < dwEndIndex; ++dwCnt) {
		m_bypMap[dwCnt] = static_cast<BYTE>(_vecMaps[dwCnt]);
	}
}

// _vDepa : 출발지
// _vDest : 도착지
// _dwRange : 탐색 범위 - 값이 없다면 기본 범위 값 사용
BOOL JumpPointSearch::Start(vec3 _vDepa, vec3 _vDest, DWORD _dwRange)
{
	// 이전 길찾기 초기화
	SafeDeleteArray(m_bypPathMap);
	for (auto& rNode : m_openList)
	{
		SafeDelete(rNode);
	}
	m_openList.clear();
	for (auto& rNode : m_closedList)
	{
		SafeDelete(rNode);
	}
	m_closedList.clear();
	m_stpFinishNode = nullptr;

	// 타일 인덱스 단위 출발지와 도착지 좌표 구하기
	m_lDepaX = LONG(_vDepa.x / m_fTileSize);
	m_lDepaZ = LONG(_vDepa.z / m_fTileSize);
	m_lDestX = LONG(_vDest.x / m_fTileSize);
	m_lDestZ = LONG(_vDest.z / m_fTileSize);

	
	RECT mapRect;
	DWORD dwRange;
	// 파라미터로 범위 값을 받았다면 해당 범위 값을 사용
	if (_dwRange) {
		dwRange = _dwRange;
	}
	else {
		dwRange = m_dwRange;
	}
	// 길찾기 탐색 범위 구하기
	if (m_lDepaX < m_lDestX) {
		if (0 <= m_lDepaX - (LONG)dwRange) {
			mapRect.left = m_lDepaX - dwRange;
		}
		else {
			mapRect.left = 0;
		}

		if (m_dwTileWidth > m_lDestX + dwRange) {
			mapRect.right = m_lDestX + dwRange;
		}
		else {
			mapRect.right = m_dwTileWidth - 1;
		}
	}
	else {
		if (0 <= m_lDestX - (LONG)dwRange) {
			mapRect.left = m_lDestX - dwRange;
		}
		else {
			mapRect.left = 0;
		}

		if (m_dwTileWidth > m_lDepaX + dwRange) {
			mapRect.right = m_lDepaX + dwRange;
		}
		else {
			mapRect.right = m_dwTileWidth - 1;
		}
	}

	if (m_lDepaZ < m_lDestZ) {
		if (0 <= m_lDepaZ - (LONG)dwRange) {
			mapRect.top = m_lDepaZ - dwRange;
		}
		else {
			mapRect.top = 0;
		}

		if (m_dwTileHeight > m_lDestZ + dwRange) {
			mapRect.bottom = m_lDestZ + dwRange;
		}
		else {
			mapRect.bottom = m_dwTileHeight - 1;
		}
	}
	else {
		if (0 <= m_lDestZ - (LONG)dwRange) {
			mapRect.top = m_lDestZ - dwRange;
		}
		else {
			mapRect.top = 0;
		}

		if (m_dwTileHeight > m_lDepaZ + dwRange) {
			mapRect.bottom = m_lDepaZ + dwRange;
		}
		else {
			mapRect.bottom = m_dwTileHeight - 1;
		}
	}

	// 탐색 범위로 길찾기 할 맵 생성
	m_dwPathWidth = mapRect.right - mapRect.left;
	m_dwPathHeight = mapRect.bottom - mapRect.top;

	m_bypPathMap = new BYTE[m_dwPathWidth * m_dwPathHeight];		// 탐색에 사용할 BYTE 맵
	for (DWORD dwCnt = 0; dwCnt < m_dwPathHeight; ++dwCnt) {
		memcpy_s(&m_bypPathMap[dwCnt * m_dwPathWidth], m_dwPathWidth, &m_bypMap[(dwCnt + mapRect.top) * m_dwTileWidth + mapRect.left], m_dwPathWidth);
	}

	LONG lDepaIndex = (m_lDepaZ - mapRect.top) * m_dwPathWidth + (m_lDepaX - mapRect.left);
	LONG lDestIndex = (m_lDestZ - mapRect.top) * m_dwPathWidth + (m_lDestX - mapRect.left);

	// 출발지와 도착지가 비어있지 않으면 flase
	if (m_bypPathMap[lDepaIndex] != static_cast<BYTE>(PathType::EMPT)) {
		return FALSE;
	}
	if (m_bypPathMap[lDestIndex] != static_cast<BYTE>(PathType::EMPT)) {
		return FALSE;
	}

	m_bypPathMap[lDepaIndex] = static_cast<BYTE>(PathType::DEPA);	// 출발지
	m_bypPathMap[lDestIndex] = static_cast<BYTE>(PathType::DEST);	// 도착지


	stNode* stpCreateNode = new stNode;	// 출발지 노드 생성
	stpCreateNode->stpParent = nullptr;	// 처음 시작 노드는 부모가 없다
	stpCreateNode->dwF = GetDistance(m_lDepaX, m_lDepaZ, m_lDestX, m_lDestZ);
	stpCreateNode->dwG = 0;
	stpCreateNode->dwH = stpCreateNode->dwF;	// 출발지라서 F값과 같다
	stpCreateNode->lPosX = m_lDepaX;
	stpCreateNode->lPosY = m_lDepaZ;
	stpCreateNode->byDirection = DIRECTION_ALL;	// 노드 탐색 방향 초기화 추가
	m_openList.emplace_back(stpCreateNode);

	stNode* stpOpenNode;	// 오픈 리스트 노드
	BYTE byDirection;		// 탐색 방향
	LONG lPosX;				// 탐색 인덱스 X
	LONG lPosY;				// 탐색 인덱스 Y
	while (!m_openList.empty()) {
		stpOpenNode = *m_openList.begin();
		m_openList.pop_front();
		m_closedList.emplace_back(stpOpenNode);

		byDirection = stpOpenNode->byDirection;

		lPosX = stpOpenNode->lPosX;
		lPosY = stpOpenNode->lPosY;

		if (CheckType(lPosX, lPosY) == static_cast<BYTE>(PathType::DEST)) {
			// 길찾기 완료
			OptimizePath(stpOpenNode);
			m_stpFinishNode = stpOpenNode;
			return TRUE;
		}

		// 위
		if (byDirection & DIRECTION_UU) {
			if (Find_UU(lPosX, lPosY, &stpCreateNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		// 오른쪽
		if (byDirection & DIRECTION_RR) {
			if (Find_RR(lPosX, lPosY, &stpCreateNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		// 아래쪽
		if (byDirection & DIRECTION_DD) {
			if (Find_DD(lPosX, lPosY, &stpCreateNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		//왼쪽
		if (byDirection & DIRECTION_LL) {
			if (Find_LL(lPosX, lPosY, &stpCreateNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		//위 오른쪽
		if (byDirection & DIRECTION_UR) {
			if (Find_UR(lPosX, lPosY, &stpCreateNode, stpOpenNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		//아래 오른쪽
		if (byDirection & DIRECTION_DR) {
			if (Find_DR(lPosX, lPosY, &stpCreateNode, stpOpenNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		//아래 왼쪽
		if (byDirection & DIRECTION_DL) {
			if (Find_DL(lPosX, lPosY, &stpCreateNode, stpOpenNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		//위 왼쪽
		if (byDirection & DIRECTION_UL) {
			if (Find_UL(lPosX, lPosY, &stpCreateNode, stpOpenNode)) {
				ConnectNode(stpOpenNode, stpCreateNode);
			}
		}

		// 오픈 리스트 정렬
		JumpPointSearchSort(m_openList.begin(), m_openList.begin(), m_openList.end());
	}

	// 찾지 못함
	return FALSE;
}

void JumpPointSearch::Finish(list<vec3>& listMovePos)
{
	float fTileCenter = m_fTileSize * 0.5f;
	stNode* stpNode = m_stpFinishNode;

	while (stpNode->stpParent != nullptr) {
		listMovePos.emplace_front(vec3(float(stpNode->lPosX * m_fTileSize + fTileCenter),
			float(stpNode->lPosY * m_fTileSize + fTileCenter),
			0.f));

		stpNode = stpNode->stpParent;
	}
}

// 길찾기에 실패해서 대체할 새로운 길을 찾아야 한다
// 보통 출발지가 막힌경우 보다 도착지가 막힌 길인 경우가 많다
// _vDest : 도착지지만 사실 출발지
// _vDepa : 출발지지만 변경될 도착지
void JumpPointSearch::NearbyPath(vec3 _vDest, vec3 & _vDepa)
{
	vec3 vDir = { _vDest.x - _vDepa.x, _vDest.y - _vDepa.y, 0.f };
	float fDistance = D3DXVec3Length(&vDir);	// 거리
	D3DXVec3Normalize(&vDir, &vDir);
	vec3 vPos = { _vDepa.x, _vDepa.y, 0.f };

	vDir = vDir * m_fTileSize;

	while (fDistance > 0.f)
	{
		vPos = vPos + vDir;

		if (CheckType(LONG(vPos.x / m_fTileSize), LONG(vPos.y / m_fTileSize)) == static_cast<BYTE>(PathType::EMPT)) {
			break;
		}

		fDistance = fDistance - (float)m_fTileSize;
	}

	_vDepa = vPos;
}

void JumpPointSearch::ReleaseJumpPointSearch() {
	delete[] m_bypMap;
	delete[] m_bypPathMap;
	for (auto& rNode : m_openList)
	{
		SafeDelete(rNode);
	}
	m_openList.clear();
	for (auto& rNode : m_closedList)
	{
		SafeDelete(rNode);
	}
	m_closedList.clear();
}

void JumpPointSearch::JumpPointSearchSort(iterator & _iterFirst, iterator & _iterBegin, iterator & _iterEnd) {
	stNode* stpSort;					//정렬할 값
	stNode* stpNext;					//비교할 값
	iterator iterNext;					//비교할 이터레이터
	
	if (_iterFirst._Ptr->_Next == _iterEnd._Ptr)
		return;

	iterNext = _iterFirst;
	stpSort = iterNext._Ptr->_Next->_Myval;

	while (iterNext != _iterBegin) {
		stpNext = *iterNext;
		// 비교
		if (stpNext->dwF <= stpSort->dwF) {
			break;
		}
		//F 더 높은거 뒤로 보냄
		iterNext._Ptr->_Next->_Myval = stpNext;
		iterNext._Ptr->_Myval = stpSort;

		--iterNext;
	}

	++_iterFirst;
	JumpPointSearchSort(_iterFirst, _iterBegin, _iterEnd);
}

// lPosX : 체크 좌표 X
// lPosY : 체크 좌표 Y
BYTE JumpPointSearch::CheckType(LONG lPosX, LONG lPosY) {
	DWORD dwIndex = lPosY * m_dwPathWidth + lPosX;
	return m_bypPathMap[dwIndex];
}

// lPosX : 탐색 좌표 X
// lPosY : 탐색 좌표 Y
// ppCreateNode : 생성해서 반환할 노드
BOOL JumpPointSearch::Find_UU(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode) {
	BYTE byType;
	// 위쪽 탐색
	while (_lPosY - 1 >= 0) {
		_lPosY = _lPosY - 1;	// 위로 한칸

		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 맵의 끝임
		if (_lPosY - 1 < 0) {
			return FALSE;
		}

		// 왼쪽 벽
		// □□
		// ■★
		if (_lPosX - 1 >= 0) {
			if (CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX - 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_RR | DIRECTION_UR | DIRECTION_UL;
				return TRUE;
			}
		}
		

		// 오른쪽 벽
		// □□
		// ★■
		if(_lPosX + 1 < (LONG)m_dwPathWidth) {
			if (CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX + 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_LL | DIRECTION_UR | DIRECTION_UL;
				return TRUE;
			}
		}
	}// while (_lPosY - 1 >= 0)

	return FALSE;
}

// lPosX : 탐색 좌표 X
// lPosY : 탐색 좌표 Y
// ppCreateNode : 생성해서 반환할 노드
BOOL JumpPointSearch::Find_RR(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode) {
	BYTE byType;
	// 오른쪽 탐색
	while (_lPosX + 1 < (LONG)m_dwPathWidth) {
		_lPosX = _lPosX + 1;
		
		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 맵의 끝임
		if (_lPosX + 1 >= (LONG)m_dwPathWidth) {
			return FALSE;
		}

		// 위쪽 벽
		// ■□
		// ★□
		if (_lPosY - 1 >= 0) {
			if (CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX + 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_RR | DIRECTION_DR | DIRECTION_UR;
				return TRUE;
			}
		}

		// 아래 벽
		// ★□
		// ■□
		if (_lPosY + 1 < (LONG)m_dwPathHeight) {
			if (CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX + 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_RR | DIRECTION_DR | DIRECTION_UR;
				return TRUE;
			}
		}
	}// while (_lPosX + 1 < m_dwPathWidth)

	return FALSE;
}

// lPosX : 탐색 좌표 X
// lPosY : 탐색 좌표 Y
// ppCreateNode : 생성해서 반환할 노드
BOOL JumpPointSearch::Find_DD(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode) {
	BYTE byType;
	// 아래쪽
	while (_lPosY + 1 < (LONG)m_dwPathHeight) {
		_lPosY = _lPosY + 1;
		
		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 맵의 끝임
		if (_lPosY + 1 >= (LONG)m_dwPathHeight) {
			return FALSE;
		}

		// 왼쪽 벽
		// ■★
		// □□
		if (_lPosX - 1 >= 0) {
			if (CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX - 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_RR | DIRECTION_DR | DIRECTION_DL;
				return TRUE;
			}
		}

		// 오른쪽 벽
		// ★■
		// □□
		if (_lPosX + 1 < (LONG)m_dwPathWidth) {
			if (CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX + 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_LL | DIRECTION_DR | DIRECTION_DL;
				return TRUE;
			}
		}
	}// while (_lPosY + 1 < m_dwPathHeight)

	return FALSE;
}

// lPosX : 탐색 좌표 X
// lPosY : 탐색 좌표 Y
// ppCreateNode : 생성해서 반환할 노드
BOOL JumpPointSearch::Find_LL(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode) {
	BYTE byType;
	// 왼쪽
	while (_lPosX - 1 >= 0) {
		_lPosX = _lPosX - 1;

		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 맵의 끝임
		if (_lPosX - 1 < 0) {
			return FALSE;
		}

		//위 벽
		// □■
		// □★
		if (_lPosY - 1 >= 0) {
			if (CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX - 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_LL | DIRECTION_DL | DIRECTION_UL;
				return TRUE;
			}
		}
		
		// 아래 벽
		// □★
		// □■
		if (_lPosY + 1 < (LONG)m_dwPathHeight) {
			if (CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX - 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::EMPT)) {
				//노드 생성
				*_ppCreateNode = new stNode;
				(*_ppCreateNode)->lPosX = _lPosX;
				(*_ppCreateNode)->lPosY = _lPosY;
				(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_LL | DIRECTION_DL | DIRECTION_UL;
				return TRUE;
			}
		}
	}// while (_lPosX - 1 >= 0)

	return FALSE;
}


BOOL JumpPointSearch::Find_UR(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode) {
	stNode* stpBridgeeNode = nullptr;	// 직선 탐색시 연결 노드
	BYTE byType;

	//위 오른쪽
	while (_lPosY - 1 >= 0 && _lPosX + 1 < (LONG)m_dwPathWidth) {
		_lPosY = _lPosY - 1;
		_lPosX = _lPosX + 1;
		
		// 2020.11.18
		// 대각선 벽 사이 허용하지 않음
		// ■★
		// ↗■
		if (CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
			&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::WALL)) {
			break;
		}

		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 2018.04.30
		// 대각선은 코너 확인해야됨
		// 왼쪽 코너
		// □□
		// ■★
		// ↗
		if (_lPosX - 1 >= 0 && _lPosY - 1 >= 0
				&& CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX - 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_RR | DIRECTION_UR | DIRECTION_UL;
			return TRUE;
		}

		// 아래 코너
		// □★□
		// ↗■□
		if (_lPosX + 1 < (LONG)m_dwPathWidth && _lPosY + 1 < (LONG)m_dwPathHeight
				&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX + 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_RR | DIRECTION_UR | DIRECTION_DR;
			return TRUE;
		}

		//위
		if (Find_UU(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_UU | DIRECTION_RR | DIRECTION_UR;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}

		//오른쪽
		if (Find_RR(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_UU | DIRECTION_RR | DIRECTION_UR;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}
	}// while (_lPosY - 1 >= 0 && _lPosX + 1 < m_iRight)
	return FALSE;
}


BOOL JumpPointSearch::Find_DR(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode) {
	stNode* stpBridgeeNode = nullptr;	// 직선 탐색시 연결 노드
	BYTE byType;

	//아래 오른쪽
	while (_lPosY + 1 < (LONG)m_dwPathHeight && _lPosX + 1 < (LONG)m_dwPathWidth) {
		_lPosY = _lPosY + 1;
		_lPosX = _lPosX + 1;

		// 2020.11.18
		// 대각선 벽 사이 허용하지 않음
		// ↘■
		// ■★
		if (CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
			&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::WALL)) {
			break;
		}

		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 2018.04.30
		// 대각선은 코너 확인해야됨
		// 왼쪽 코너
		// ↘
		// ■★
		// □□
		if (_lPosX - 1 >= 0 && _lPosY + 1 < (LONG)m_dwPathHeight
				&& CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX - 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_RR | DIRECTION_DR | DIRECTION_DL;
			return TRUE;
		}

		// 위 코너
		// ↘■□
		// □★□
		if (_lPosX + 1 < (LONG)m_dwPathWidth && _lPosY - 1 >= 0
				&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX + 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_RR | DIRECTION_UR | DIRECTION_DR;
			return TRUE;
		}

		//아래
		if (Find_DD(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_DD | DIRECTION_RR | DIRECTION_DR;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}

		//오른쪽
		if (Find_RR(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_DD | DIRECTION_RR | DIRECTION_DR;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}
	}// while (_lPosY + 1 < m_iBottom && _lPosX + 1 < m_iRight)
	return FALSE;
}


BOOL JumpPointSearch::Find_DL(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode) {
	stNode* stpBridgeeNode = nullptr;	// 직선 탐색시 연결 노드
	BYTE byType;

	//아래 왼쪽
	while (_lPosY + 1 < (LONG)m_dwPathHeight && _lPosX - 1 >= 0) {
		_lPosY = _lPosY + 1;
		_lPosX = _lPosX - 1;

		// 2020.11.18
		// 대각선 벽 사이 허용하지 않음
		// ■↙
		// ★■
		if (CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
			&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::WALL)) {
			break;
		}

		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 2018.04.30
		// 대각선은 코너 확인해야됨
		// 위 코너
		// □■↙
		// □★
		if (_lPosX - 1 >= 0 && _lPosY - 1 >= 0
				&& CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX - 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_LL | DIRECTION_DL | DIRECTION_UL;
			return TRUE;
		}

		// 오른쪽 코너
		// □↙
		// ★■
		// □□
		if (_lPosX + 1 < (LONG)m_dwPathWidth && _lPosY + 1 < (LONG)m_dwPathHeight
				&& CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX + 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_DD | DIRECTION_LL | DIRECTION_DL | DIRECTION_DR;
			return TRUE;
		}

		//아래
		if (Find_DD(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_DD | DIRECTION_LL | DIRECTION_DL;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}

		//왼쪽
		if (Find_LL(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_DD | DIRECTION_LL | DIRECTION_DL;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}
	}// while (_lPosY + 1 < m_iBottom && _lPosX - 1 >= 0)
	return FALSE;
}


BOOL JumpPointSearch::Find_UL(LONG _lPosX, LONG _lPosY, stNode** _ppCreateNode, stNode *_stpParentNode) {
	stNode* stpBridgeeNode = nullptr;	// 직선 탐색시 연결 노드
	BYTE byType;

	//위 왼쪽
	while (_lPosY - 1 >= 0 && _lPosX - 1 >= 0) {
		_lPosY = _lPosY - 1;
		_lPosX = _lPosX - 1;

		// 2020.11.18
		// 대각선 벽 사이 허용하지 않음
		// ★■
		// ■↖
		if (CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
			&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::WALL)) {
			break;
		}

		byType = CheckType(_lPosX, _lPosY);
		// 진행 불가
		if (byType == static_cast<BYTE>(PathType::WALL) || byType == static_cast<BYTE>(PathType::NODE)) {
			break;
		}
		// 도착지 체크
		else if (byType == static_cast<BYTE>(PathType::DEST)) {
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			return TRUE;
		}

		// 2018.04.30
		// 대각선은 코너 확인해야됨
		// 아래 코너
		// □★
		// □■↖
		if (_lPosX - 1 >= 0 && _lPosY + 1 < (LONG)m_dwPathHeight
				&& CheckType(_lPosX - 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX - 1, _lPosY + 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_LL | DIRECTION_UL | DIRECTION_DL;
			return TRUE;
		}

		// 오른쪽 코너
		// □□
		// ★■
		// □↖
		if (_lPosX + 1 < (LONG)m_dwPathWidth && _lPosY - 1 >= 0
				&& CheckType(_lPosX + 1, _lPosY) == static_cast<BYTE>(PathType::WALL)
				&& CheckType(_lPosX, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)
				&& CheckType(_lPosX + 1, _lPosY - 1) == static_cast<BYTE>(PathType::EMPT)) {
			//노드 생성
			*_ppCreateNode = new stNode;
			(*_ppCreateNode)->lPosX = _lPosX;
			(*_ppCreateNode)->lPosY = _lPosY;
			(*_ppCreateNode)->byDirection |= DIRECTION_UU | DIRECTION_LL | DIRECTION_UL | DIRECTION_UR;
			return TRUE;
		}

		//위
		if (Find_UU(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_UU | DIRECTION_LL | DIRECTION_UL;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}

		//왼쪽
		if (Find_LL(_lPosX, _lPosY, _ppCreateNode)) {
			stpBridgeeNode = new stNode;
			stpBridgeeNode->lPosX = _lPosX;
			stpBridgeeNode->lPosY = _lPosY;
			stpBridgeeNode->byDirection |= DIRECTION_UU | DIRECTION_LL | DIRECTION_UL;
			// 여기서 연결하고 전부 나감 함수 바깥에서 연결할 필요 없음
			ConnectNode(_stpParentNode, stpBridgeeNode);
			ConnectNode(stpBridgeeNode, *_ppCreateNode);
			return FALSE;
		}
	}// while (_lPosY - 1 >= 0 && _lPosX - 1 >= 0)S
	return FALSE;
}


// _stpParentNode : 부모 노드
// _stpChildNode : 자식 노드
void JumpPointSearch::ConnectNode(stNode* _stpParentNode, stNode* _stpChildNode) {
	// 부모와 연결 및 나머지 변수 셋팅
	_stpChildNode->stpParent = _stpParentNode;
	_stpChildNode->dwG = _stpParentNode->dwG + GetDistance(_stpParentNode->lPosX, _stpParentNode->lPosY, _stpChildNode->lPosX, _stpChildNode->lPosY);
	_stpChildNode->dwH = GetDistance(_stpChildNode->lPosX, _stpChildNode->lPosY, m_lDestX, m_lDestZ);
	_stpChildNode->dwF = _stpChildNode->dwG + _stpChildNode->dwH;

	DWORD dwIndex = _stpChildNode->lPosY * m_dwPathWidth + _stpChildNode->lPosX;

	if (m_bypPathMap[dwIndex] != static_cast<BYTE>(PathType::DEST)) {
		m_bypPathMap[dwIndex] = static_cast<BYTE>(PathType::NODE);
	}

	m_openList.emplace_back(_stpChildNode);
}

// 두 좌표 사이의 거리
DWORD JumpPointSearch::GetDistance(LONG _lDepaX, LONG _lDepaY, LONG _lDestX, LONG _lDestY)
{
	DWORD dwRetval;
	LONG lX = _lDestX - _lDepaX;
	LONG lY = _lDestY - _lDepaY;

	lX = labs(lX);
	lY = labs(lY);

	if (lX < lY)
	{
		dwRetval = (lY - lX) * 10;		// 직선 거리 값
		dwRetval = dwRetval + lX * 14;	// 대각선 거리 값
	}
	else
	{
		dwRetval = (lX - lY) * 10;		// 직선 거리 값
		dwRetval = dwRetval + lY * 14;	// 대각선 거리 값
	}

	return dwRetval;
}

void JumpPointSearch::OptimizePath(stNode * _stpFinishNode)
{
	stNode* stpNode = _stpFinishNode;	// 현제 진행중인 노드
	stNode* stpNext = nullptr;			// 다음 노드
	stNode* stpRay = nullptr;			// 다음 노드의 다음 RayCast 대상
	vec3 vDir = {};
	vec3 vPos = {};
	float fDistance;


	// 출발지까지 되감는다
	while (stpNode != nullptr) {
		if (stpNode->stpParent == nullptr)
			goto NEXT;

		// 다음 포인터
		stpNext = stpNode->stpParent;

		if (stpNext->stpParent == nullptr)
			goto NEXT;

		// 다음 포인터의 다음 포인터
		stpRay = stpNext->stpParent;
		while (stpRay != nullptr) {
			vDir = { float(stpNode->lPosX - stpRay->lPosX), float(stpNode->lPosY - stpRay->lPosY), 0.f };
			fDistance = D3DXVec3Length(&vDir);
			D3DXVec3Normalize(&vDir, &vDir);
			vPos = { (float)stpRay->lPosX, (float)stpRay->lPosY, 0.f };

			// Ray... - Next - Node 의 관계에서
			// 중간에있는 Next 노드를 지울 수 있나 검사한다
			if (!RayCast(vPos, vDir, fDistance)) {
				break;	// 종료
			}

			// 부모 교체
			stpNode->stpParent = stpRay;

			// 그다음 노드 검사
			stpRay = stpRay->stpParent;
		}
	NEXT:
		stpNode = stpNode->stpParent;
	}
}

// _vPos : 위치
// _vDir : 방향
// _fDistance : 거리
BOOL JumpPointSearch::RayCast(vec3 _vPos, vec3 _vDir, float _fDistance)
{
	// 목표 지점까지 간다
	while (_fDistance > 0.f)
	{
		_vPos = _vPos + _vDir;

		if (CheckType((LONG)_vPos.x, (LONG)_vPos.y) == static_cast<BYTE>(PathType::WALL)) {
			return FALSE;
		}

		_fDistance = _fDistance - 1.f;
	}

	return TRUE;
}
