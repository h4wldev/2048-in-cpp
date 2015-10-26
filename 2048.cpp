/*
*  2048 puzzle game in c/c++
*  Made by hawlkim of Korea Digital Media High School
*
*  Do not delete this comment
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <math.h>

// 맵 사이즈 정의
#define MAPSIZE 4

// 4방향 키코드 정의
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80

// 디버그 모드
#define ISDEBUG false

// 콘솔 색상 정의
enum { BLACK, D_BLUE, D_GREEN, D_SKYBLUE, D_RED, D_VIOLET, D_YELLOW, GRAY, D_GRAY, BLUE, GREEN, SKYBLUE, RED, VIOLET, YELLOW, WHITE };

int score;
int map[MAPSIZE][MAPSIZE] = { 0 };
bool Gameover = false;

void Initialize();
void printMap();
void printNumber(int number, int x, int y);
void printFrame(int code);
void blockMove(int direction);
void getDirection();
void randomGenerate();
void SetColor(int backcolor, int fontcolor) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), backcolor * 16 + fontcolor); }
void gotoxy(int x, int y) { COORD pos = { x, y }; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); }
bool isGameover();
int isBlock(int x, int y) { return map[x][y]; };
int *getBlock(int x, int y, int direction);

int main(void) {
   system("mode con cols=43 lines=27");
   system("color f0");
   srand(unsigned(time(NULL)));

   Initialize();
   printMap();

   while(!Gameover) {
      getDirection();
      printMap();
   }
   system("cls");
   printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\tGame Over\n");+

   system("pause>nul");
}

// 게임 초깃값 설정
void Initialize() {
   score = 100;
   for (int i = 0; i < 2; i++) randomGenerate();
}

// 맵 프린트
void printMap() {
   system("cls");
   printf("      _______ _______ _   ___  _____\n");
   printf("     |       |  _    | | |   ||  _  |\n");
   printf("     |____   | | |   | |_|   || |_| |\n");
   printf("      ____|  | | |   |       |   _   |\n");
   printf("     | ______| |_|   |___    |  | |  |\n");
   printf("     | |_____|       |   |   |  |_|  |\n");
   printf("     |_______|_______|   |___|_______|\n");

   printFrame(7);
   printFrame(6);
   printf("          Score : %10d          ", score);
   printFrame(6);
   printf("\n");
   printFrame(1);
   for (int y = 0; y < MAPSIZE; y++) {
      printFrame(2); printFrame(6);
      for (int x = 0; x < MAPSIZE; x++) {
         printf("        ");
         printFrame(6);
      }
      printFrame(3);
      if (y == MAPSIZE - 1) printFrame(5);
      else printFrame(4);
      printf("\n");
   }

   for (int y = 0; y < MAPSIZE; y++) {
      for (int x = 0; x < MAPSIZE; x++) {
         printNumber(map[x][y], (x * 10) + 2, (y * 4) + 10);
      }
   }
}

// 색상과 함께 숫자 출력
void printNumber(int number, int x, int y) {
   switch (number) {
      case 0: case 2: case 4:
         SetColor(WHITE, BLACK);
         break;
      case 8: case 16: case 32: case 64:
         SetColor(GRAY, WHITE);
         break;
      case 128: case 256: case 512: case 1024: case 2048:
         SetColor(YELLOW, BLACK);
         break;
      default:
         SetColor(RED, WHITE);
         break;
   }
   int len = ((int)log10((double)number) + 1 < 0) ? 1 : (int)log10((double)number) + 1;

   gotoxy(x, y);
   printf("        ");

   gotoxy(x, y + 1);
   for (int i = 0; i < (9 - len)/2; i++) printf(" ");
   printf("%d", number);
   for (int i = 0; i < (8 - len)/2; i++) printf(" ");
   
   gotoxy(x, y + 2);
   printf("        ");

   SetColor(WHITE, BLACK);
}
// 뼈대 프린트
void printFrame(int code) {
   switch (code) {
   case 1: printf("├────┬────┬────┬────┤"); printf("\n"); break;
   case 2: printf("│        │        │        │        │"); printf("\n"); break;
   case 3: printf("\n"); printf("│        │        │        │        │"); break;
   case 4: printf("\n"); printf("├────┼────┼────┼────┤"); break;
   case 5: printf("\n"); printf("└────┴────┴────┴────┘"); break;
   case 6: printf("│"); break;
   case 7: printf("┌───────────────────┐"); printf("\n"); break;
   }
}

// 블럭 이동
void blockMove(int direction) {
   int randomly;
   bool check = false;

   for (int y = 0; y < MAPSIZE; y++) {
      for (int x = 0; x < MAPSIZE; x++) {
         if (*getBlock(x, y, direction)) {
            for (int randomly = x + 1; randomly < MAPSIZE; randomly++) {
               if (*getBlock(x, y, direction) == *getBlock(randomly, y, direction)) {
                  score += map[x][y] * map[x][y];
                  *getBlock(x, y, direction) *= 2;
                  *getBlock(randomly, y, direction) = 0;
                  check = true;
                  break;
               }
               else if (*getBlock(randomly, y, direction)) {
                  break;
               }
            }
            for (randomly = x - 1; randomly >= 0; randomly--) {
               if (*getBlock(randomly, y, direction)) {
                  break;
               }
            }
            randomly++;

            if (x != randomly) {
               *getBlock(randomly, y, direction) = *getBlock(x, y, direction);
               *getBlock(x, y, direction) = 0;
               check = true;
            }
         }
      }
   }

   if (isGameover()) {
      printf("Gameover");
      Gameover = true;
   }
   if (check) randomGenerate();
}

// 키보드 입력
void getDirection() {
   int get;
   get = getch();
   get = getch();
   blockMove(get);
}

// 숫자 랜덤 생성
void randomGenerate() {
   int number = 2, location[2] = { 0 };

   do {
      if (rand() % 10 < 2) number = 4;
      location[0] = rand() % 4; // x
      location[1] = rand() % 4; // y
   } while (isBlock(location[0], location[1]));

   map[location[0]][location[1]] = number;

   if (ISDEBUG) printf("[BLOCK_GENERATE] x: %d, y: %d, number: %d\n", location[0], location[1], number);
}

// 게임 오버 확인
bool isGameover() {
   for (int y = 0; y < MAPSIZE; y++) {
      for (int x = 0; x < MAPSIZE; x++) {
         if (!map[x][y]) return false;
      }
   }
   return true;
}

// 방향 전환
int *getBlock(int x, int y, int direction) {
   switch (direction) {
   case LEFT: return &map[x][y];                      // left
   case UP: return &map[(MAPSIZE - 1) - y][x];      // up
   case RIGHT: return &map[(MAPSIZE - 1) - x][y];      // right
   case DOWN: return &map[y][(MAPSIZE - 1) - x];     // down
   }
}
