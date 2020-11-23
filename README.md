# GameTeam
팀프로젝트 DirectX 3D

1. 컴파일 옵션 엔진 클라
2. 참조 추가 할지 말지
3. 빌드후 이벤트 추가 (PDB 파일도 옮기게)
copy /Y "$(SolutionDir)\\Engine\\Bin\\" "$(SolutionDir)\\Client\\Bin\\"
4. 복사 생성자 삭제 이후에  가상함수를 새로 정의 CloneEventAddRef ?? =>  소유권을 서로 공유하는 포인터들 Clone  AddRef 하는 코드 규칙 이며 이 함수는 Clone 에서 반드시 호출.
