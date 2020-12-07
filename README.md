# GameTeam
팀프로젝트 DirectX 3D

## IMGUI 적용하자 !
***ImGuiHelper.h Example 네임스페이스에 사용법 코드+주석 올리니 사용할 사람 참고.***

Resource -> https://drive.google.com/drive/folders/1OrckZcW49mwoDA3r8OxzBtLUd4vg6WIt

**#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 터미널 열어서 디버깅 편하게 하자.** 

* 현재 빌드 후 이벤트
copy /Y "$(SolutionDir)\Engine\Bin\" "$(SolutionDir)\Client\Bin\" 
copy /Y "$(SolutionDir)\Engine\Headers\" "$(SolutionDir)\Reference\Headers\"
* Client 는 Engine 참조 걸어놓기
* Client Engine C++ ->  일반 -> 다중프로세서컴파일 (YES) 로 바꿈

## Git 규칙
기능 구현시에 Master 브랜치에서 새로운 기능 브랜치를 만든 이후에
디버깅 테스트 끝나고 기능 구현이 완료되면 해당 브랜치와 마스터를 병합하고 
병합이후에 테스트가 끝나면 Master 로 커밋

## ReadMe
1. 클라는 엔진 참조 걸어서 **엔진 내용이 바뀌면** 자동으로 엔진빌드
2. 엔진이 빌드되면 헤더와 바이너리 파일들 자동으로 클라로 옮김 ***(Bat 파일 실행 시킬 필요 X)***
3. PDB 파일도 옮겨서 엔진코드도 디버깅 가능

## FrameWork
### 프로토 타입 추가시
**Scene이 로딩할때 다음 Scene의 정보를 미리 로딩..... Scene는 pLoading 로 들어가서 다음씬 정보 로딩**

### Scene을 추가할 때
* Loading 클래스에서 Scene 에 추가할 클래스들의 프로토타입을 만든다.
* 해당 Scene이 생성되면 클론들을 만들어서 레이어에 넣는다.

### Clone 추가
* 포인터 공유시 AddRef Release 까먹지 말기
* 포인터 공유할때 프로토타입이 지워야 하는지 확실히 구분

* 리소스 정리
* 프레임워크 (회의때 나온 의견 반영 , Time)
* 플레이어
* 몬스터
* 맵을 프레임 워크에 추가 (조명 , 노말 맵안쪽으로 잘 나오는지 , 구체랑 충돌 , 맵 하나와 씬 하나 매핑해서 씬 넘어가는것 까지)
* 빌보드 
* AI AStar
* 충돌
* 카메라 연출


### 각자 역활
* 호준 플레이어,맵,충돌매니저
* 명준행님 몬스터1,2 보스1
* 철우형님 UI,인벤
* 지성 몬스터3,4 보스2,기본 프레임워크
* 아이템,이펙트,카메라, AI(몬1~4)

***우선순위***

**기본 프레임워크**
