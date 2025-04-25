#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>

#define W 117
#define H 29

#define SCREEN_W 1535
#define SCREEN_H 865

#define CLEAN_RATE 1000
#define FPS 20


const unsigned DELAY = 1000000 / FPS;
const char *BRIGHT = "  .:ooOO0@";
const char BR_LEVEL = 10;
const char FLAME_WEIGHTS[3][3]=
{
  {-1, 1, -1},
  {1, 2, 1},
  {2, 3, 2}
};

const char WATER_WEIGHTS[3][3]=
{
  {0, 0, 0},
  {0, 2, 0},
  {0, 2, 0}
};

void buf_print(char buf[H][W]);

void copy(char from[H][W], char to[H][W]){
  int i, j;
  for (i = 0; i < H; ++i){
      for (j = 0; j < W; ++j){
        to[i][j] = from[i][j];
      }
    }
  return;
}

char get_mean(char buf[H][W], int i, int j){
  char res = 0;
  int l, m;
  
  for (l = -1; l <= 1; ++l){
    for (m = -1; m <= 1; ++m){
      if (!(i+l < 0 || i+l >= H || j+m < 0 || j+m >= W)){
        res += buf[i+l][j+m] * FLAME_WEIGHTS[1+l][1+m];
      }
    }
  }
  if (res > 0){
    res -= rand()%4;
  }
  return res / 9 % BR_LEVEL;
}

void buf_clean(char buf[H][W]){
  int i, j;
  for (i = 0; i < H; ++i){
    for (j = 0; j < W; ++j){
      buf[i][j] = 0;
    }
  }

  return;
}

void buf_draw_flame(char buf[H][W], int x, int y){
  int i, j;

  for (i = y - 2; i < y + 3; ++i){
    for (j = x - 2; j < x + 3; ++j)
      if(!(i < 0 || i >= H || j < 0 || j >= W))
        buf[i][j] = rand() % BR_LEVEL;
  }
}

void buf_update(char buf[H][W]){
  char tmp[H][W];
  int i, j;

  for (i = 0; i < H; ++i){
    for (j = 0; j < W; ++j){
      tmp[i][j] = get_mean(buf, i, j);
    }
  }

  copy(tmp, buf);
  return;
}

void buf_print(char buf[H][W]){
  int i, j;
  printf("\033[H");
  for (i = 0; i < H; ++i){
    for (j = 0; j < W; ++j){
      printf("%c", BRIGHT[buf[i][j]]);
    }
    printf("\n");
  }
  return;
}

int main(){
#if 1
  Display *display;
  Window rootWindow, childWindow;
  int x, y, program_x, program_y, clean;
  unsigned int mask;
#endif
  char buf[H][W] = {0};
#if 1
  // Открываем соединение с X сервером
  display = XOpenDisplay(NULL);
    if (display == NULL) {
      fprintf(stderr, "Не удалось открыть соединение с X сервером.\n");
      return 1;
    }

  // Получаем корневое окно
  rootWindow = DefaultRootWindow(display);
#endif
  system("clear");
  clean = 0;
  for(;;){
    if (clean == CLEAN_RATE){ 
      system("clear");
      clean = 0;
    }
#if 1
    // Получаем координаты мыши
    XQueryPointer(display, rootWindow, &childWindow, &childWindow, &x, &y, &mask, &mask, &mask);

    // Нормализуем координаты
    program_x = (x * W) / SCREEN_W;
    program_y = (y * H) / SCREEN_H;
    
    // Добавляем источник пламени
    buf_draw_flame(buf, program_x, program_y);
#endif
#if 0
    buf_draw_flame(buf, 40, 14);
    buf_draw_flame(buf, 80, 14);
    buf_draw_flame(buf, 20, 10);
#endif
    //Рендерим кадр
    buf_update(buf);

    //Рисуем кадр
    buf_print(buf);
    usleep(DELAY);

    clean +=1;
  }

  return 0;
}
