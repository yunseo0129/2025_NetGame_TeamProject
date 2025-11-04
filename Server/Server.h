#pragma once
// <TCP/IP>=====================================================================
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 9000
#define BUFSIZE 4096    

enum ACTION {
	ACTION_NONE,
	ACTION_MOVE_R,
	ACTION_MOVE_L,
	ACTION_JUMP_UP,
	ACTION_JUMP_DOWN,
	ACTION_FIRE
};