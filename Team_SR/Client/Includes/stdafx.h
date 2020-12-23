// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>
#include <iostream>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>
#include <cassert>

/*
dx9
*/
#include <d3d9.h>
#include <d3dx9.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#pragma warning(disable:4819)	// UTF-8 컴파일 관련 경고 무시

#include "Client_Typedef.h"
#include "Client_Define.h"
#include "Client_Extern.h"
#include "Client_Enum.h"
#include "Client_Struct.h"
#include "Management.h"
#include "Engine_Include.h"

/**********************************************************/
#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")
#include "SoundMgr.h"
/**********************************************************/

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#endif