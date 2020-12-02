# GameTeam
팀프로젝트 DirectX 3D

Client와 Engine 설정 컴파일 속도 /mp 사용

Resource -> https://drive.google.com/drive/folders/1OrckZcW49mwoDA3r8OxzBtLUd4vg6WIt

**#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 터미널 열어서 디버깅 편하게 하자.** 


## Pull 한 이후에 해야 하는것
1. Client 프로젝트 -> 참조 -> Engine 프로젝트 참조에 추가
2. Client 와 Engine 빌드옵션 32Bit 맞는지 확인
3. Engine 한번 빌드해서 Bin 파일들 Client으로 옮기기

## ReadMe
1. 클라는 엔진 참조 걸어서 **엔진 내용이 바뀌면** 자동으로 엔진빌드
2. 엔진이 빌드되면 헤더와 바이너리 파일들 자동으로 클라로 옮김 ***(Bat 파일 실행 시킬 필요 X)***
3. PDB 파일도 옮겨서 엔진코드도 디버깅 가능
4. 복사 생성자 삭제 이후에  가상함수를 새로 정의 CloneEventAddRef ?? =>  소유권을 서로 공유하는 포인터들 Clone  AddRef 하는 코드 규칙 이며 이 함수는 Clone 에서 반드시 호출.

## FrameWork
### 프로토 타입 추가시
**Scene이 로딩할때 다음 Scene의 정보를 미리 로딩..... Scene는 pLoading 로 들어가서 다음씬 정보 로딩**

### Scene을 추가할 때
* Loading 클래스에서 Scene 에 추가할 클래스들의 프로토타입을 만든다.
* 해당 Scene이 생성되면 클론들을 만들어서 레이어에 넣는다.

### Clone 추가
* 포인터 공유시 AddRef Release 까먹지 말기
* 포인터 공유할때 프로토타입이 지워야 하는지 확실히 구분
