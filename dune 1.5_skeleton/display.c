/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);


void display(
    RESOURCE resource,
    char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
    CURSOR cursor
) {


    // 나머지 내용 표시
    display_map(map);                    // 맵 표시
    display_cursor(cursor);              // 커서 표시
    display_resource(resource);          // 자원 상태 표시
    display_status();                    // 상태창 표시
    display_command();                   // 명령창 표시
}


void display_resource(RESOURCE resource) {
    POSITION pos = { 0, 0 };  // 우측 상단 경계선 내부로 조정
    gotoxy(pos);
    set_color(COLOR_RESOURCE);
    printf("Spice: %d/%d   Population: %d/%d",
        resource.spice, resource.spice_max,
        resource.population, resource.population_max);
    set_color(COLOR_DEFAULT);  // 기본 색상으로 복원
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            for (int k = 0; k < N_LAYER; k++) {
                if (src[k][i][j] >= 0) {
                    dest[i][j] = src[k][i][j];
                }
            }
        }
    }
}

char colorbuf[MAP_HEIGHT][MAP_WIDTH];

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
    project(map, backbuf);

    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (frontbuf[i][j] != backbuf[i][j]) {
                POSITION pos = { i, j };
                int color = COLOR_DEFAULT; // 기본 색상

                // 문자의 종류에 따라 색상 설정
                switch (backbuf[i][j]) {
                case '#':
                    color = COLOR_DEFAULT;
                    break;
                case 'B':
                    color = COLOR_ATREIDES;
                    break;
                case 'S':
                    color = COLOR_SANDWORM;
                    break;

                    // 다른 문자의 경우 추가
                default:
                    color = COLOR_DEFAULT;
                    break;
                }

                printc(padd(map_pos, pos), backbuf[i][j], color);
                colorbuf[i][j] = color; // 색상 저장
            }
            frontbuf[i][j] = backbuf[i][j];
        }
    }
}


// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
    POSITION prev = cursor.previous;
    POSITION curr = cursor.current;

    // 커서가 이전 위치에서 원래 색상으로 복원
    char prev_ch = frontbuf[prev.row][prev.column];
    int prev_color = getColorAtPosition(prev);
    printc(padd(map_pos, prev), prev_ch, prev_color);

    // 커서가 현재 위치에서 색상 변경
    char curr_ch = frontbuf[curr.row][curr.column];
    printc(padd(map_pos, curr), curr_ch, COLOR_CURSOR);
}

// 지정된 위치의 색상을 반환하는 함수
int getColorAtPosition(POSITION pos) {
    return colorbuf[pos.row][pos.column];
}





void display_status() {
    POSITION pos = { 23, 66 };  // 우측 하단 경계선 내부로 조정
    gotoxy(pos);
    set_color(COLOR_DEFAULT);

}

void display_command() {
    POSITION pos = { MAP_HEIGHT + 4, MAP_WIDTH + 4 };  // 명령창 경계선 내부로 조정
    gotoxy(pos);
    set_color(COLOR_DEFAULT);
    printf("Commands:\n");
    printf("[M] Move   [H] Harvest   [Q] Quit\n");
}

void update_screen() {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            if (backbuf[i][j] != frontbuf[i][j]) {
                POSITION pos = { i + 2, j + 2 };  // 맵 위치에 맞게 조정
                printc(pos, backbuf[i][j], COLOR_DEFAULT);  // 변경된 문자 출력
                frontbuf[i][j] = backbuf[i][j];  // frontbuf 업데이트
            }
        }
    }
}

void setColor(HANDLE hConsole, int color) {
    SetConsoleTextAttribute(hConsole, color);
}




