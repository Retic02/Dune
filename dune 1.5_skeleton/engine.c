//타일에 색을 칠해서 오브젝트화 -> 커서가 지나갈 때 색이 없어지는 문제
//움직이는 오브젝트 S는 커서가 지나가도 색이 변하지 않음-> 움직임을 담당하는 코드를 지워도 커서가 지나갈 때 색이 변하지 않음
//기타 오브젝트를 같은 방식으로 생성했을 때 알파벳만 남고 배경 색이 전부 사라짐

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <Windows.h>
#include "common.h"
#include "io.h"
#include "display.h"

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);


/* ================= control =================== */
int sys_clock = 0;  // system-wide clock(ms)
CURSOR cursor = { {5, 5}, {2, 2} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };

RESOURCE resource = {
    .spice = 0,
    .spice_max = 0,
    .population = 0,
    .population_max = 0
};

OBJECT_SAMPLE obj = {
    .pos = {1, 1},
    .dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
    .repr = 'S',
    .speed = 300,
    .next_move_time = 300
};

OBJECT_SAMPLE obj_B = {
    .pos = {MAP_HEIGHT - 2, 2},
    .dest = {MAP_HEIGHT - 2, 2},
    .repr = 'B',
    .speed = 0,
    .next_move_time = 0
};


/* ================= main() =================== */
int main(void) {
    srand((unsigned int)time(NULL));

    init();
    intro();
    display(resource, map, cursor);

    while (1) {
        // loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
        KEY key = get_key();

        // 키 입력이 있으면 처리
        if (is_arrow_key(key)) {
            cursor_move(ktod(key));
        }
        else {
            // 방향키 외의 입력
            switch (key) {
            case k_quit: outro();
            case k_space: {
                // 커서 위치에서 상호작용 처리
                POSITION curr = cursor.current;
                if ((curr.row == MAP_HEIGHT - 2 || curr.row == MAP_HEIGHT - 1) &&
                    (curr.column == 2 || curr.column == 3)) {
                    printf("Interacted with the object at (%d, %d)\n", curr.row, curr.column);
                }
                break;
            }
            case k_none:
            case k_undef:
            default: break;
            }
        }

        // 샘플 오브젝트 동작
        sample_obj_move();

        // 화면 출력
        display(resource, map, cursor);
        Sleep(TICK);
        sys_clock += 10;
    }
}





/* ================= subfunctions =================== */
void intro(void) {
    printf(" ####   #   #  #   #  #####      #       #####\n");
    printf(" #   #  #   #  ##  #  #          #       #    \n");
    printf(" #    # #   #  # # #  ####       #       #####\n");
    printf(" #   #  #   #  #  ##  #          #            #\n");
    printf(" ####   #####  #   #  #####      #   #   #####\n");
    Sleep(2000);
    system("cls");
}

void outro(void) {
    printf("exiting...\n");
    exit(0);
}

void init(void) {
    // layer 0(map[0])에 지형 생성
    for (int j = 0; j < MAP_WIDTH; j++) {
        map[0][0][j] = '#';
        map[0][MAP_HEIGHT - 1][j] = '#';
    }

    for (int i = 1; i < MAP_HEIGHT - 1; i++) {
        map[0][i][0] = '#';
        map[0][i][MAP_WIDTH - 1] = '#';
        for (int j = 1; j < MAP_WIDTH - 1; j++) {
            map[0][i][j] = ' '; // 빈 공간을 나타내는 문자
        }
    }

    // layer 1(map[1])은 비워 두기(-1로 채움)
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            map[1][i][j] = -1;
        }
    }

    // S 오브젝트 초기화
    map[1][obj.pos.row][obj.pos.column] = 'S';

    // B 오브젝트 초기화 (2x2 크기)
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) { map[1][obj_B.pos.row + i-1][obj_B.pos.column + j-1] = 'B'; }
    }
    
}




// (가능하다면) 지정한 방향으로 커서 이동
void cursor_move(DIRECTION dir) {
    POSITION curr = cursor.current;
    POSITION new_pos = pmove(curr, dir);

    // 커서 이동을 맵 내부로 제한 (예: 1 ≤ row < MAP_HEIGHT - 1, 1 ≤ col < MAP_WIDTH - 1)
    if (1 <= new_pos.row && new_pos.row < MAP_HEIGHT-1 &&
        1 <= new_pos.column && new_pos.column < MAP_WIDTH-1) {

        cursor.previous = cursor.current;
        cursor.current = new_pos;

    }
}


/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
    // 현재 위치와 목적지를 비교해서 이동 방향 결정    
    POSITION diff = psub(obj.dest, obj.pos);
    DIRECTION dir;

    // 목적지 도착. 지금은 단순히 원래 자리로 왕복
    if (diff.row == 0 && diff.column == 0) {
        if (obj.dest.row == 1 && obj.dest.column == 1) {
            // topleft --> bottomright로 목적지 설정
            POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
            obj.dest = new_dest;
        }
        else {
            // bottomright --> topleft로 목적지 설정
            POSITION new_dest = { 1, 1 };
            obj.dest = new_dest;
        }
        return obj.pos;
    }

    // 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
    if (abs(diff.row) >= abs(diff.column)) {
        dir = (diff.row >= 0) ? d_down : d_up;
    }
    else {
        dir = (diff.column >= 0) ? d_right : d_left;
    }

    // validation check
    // next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
    // 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
    POSITION next_pos = pmove(obj.pos, dir);
    if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 &&
        1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 &&
        map[1][next_pos.row][next_pos.column] < 0) {

        return next_pos;
    }
    else {
        return obj.pos;  // 제자리
    }
}

void sample_obj_move(void) {
    if (sys_clock <= obj.next_move_time) {
        // 아직 시간이 안 됐음
        return;
    }

    // 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
    map[1][obj.pos.row][obj.pos.column] = -1;
    obj.pos = sample_obj_next_position();
    map[1][obj.pos.row][obj.pos.column] = obj.repr;

    obj.next_move_time = sys_clock + obj.speed;
}


