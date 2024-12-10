/*
* raw(?) I/O
*/
#include "io.h"
#include "display.h"

void gotoxy(POSITION pos) {
	COORD coord = { pos.column, pos.row }; // 행, 열 반대로 전달
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void printc(POSITION pos, char ch, int color) {
	// 배경색 설정 제거
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	gotoxy(pos);
	printf("%c", ch); // 문자 출력
	// 색상 복원
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR_DEFAULT);
}


KEY get_key(void) {
    if (!_kbhit()) {  // 입력된 키가 있는지 확인
        return k_none;
    }

    int byte = _getch();    // 입력된 키를 전달 받기
    switch (byte) {
    case 'q': return k_quit;  // 'q'를 누르면 종료
    case ' ': return k_space; // 스페이스바 입력 감지
    case 224:
        byte = _getch();  // MSB 224가 입력 되면 1바이트 더 전달 받기
        switch (byte) {
        case 72: return k_up;
        case 75: return k_left;
        case 77: return k_right;
        case 80: return k_down;
        default: return k_undef;
        }
    default: return k_undef;
    }
}


