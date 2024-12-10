/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
#define COLOR_DEFAULT   (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)
#define COLOR_CURSOR    (BACKGROUND_INTENSITY | BACKGROUND_BLUE)
#define COLOR_RESOURCE  (BACKGROUND_INTENSITY | BACKGROUND_BLUE)
#define COLOR_ATREIDES  (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define COLOR_HARKONNEN (BACKGROUND_RED | BACKGROUND_INTENSITY)
#define COLOR_SANDWORM  (BACKGROUND_RED | BACKGROUND_GREEN)
#define COLOR_PLATE     (0)
#define COLOR_SPICE     (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define COLOR_ROCK      (BACKGROUND_INTENSITY)
#define COLOR_EMPTY     (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED) // 빈 공간의 기본 배경색


// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor
);

void display_status();
void display_command();
void update_screen();
void setColor(HANDLE hConsole, int color);
#endif
