# GameTeam
팀프로젝트 DirectX 3D

Client와 Engine 설정 컴파일 속도 /mp 사용

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
