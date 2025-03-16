#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define FPS 10
#define H 4
#define W 6
#define FIRE_CHARS_N 4
char fire_chars[FIRE_CHARS_N] ={'(',')','`',';'};

int main(int ac,char** av){
srand(time(NULL));
initscr(); refresh();

clock_t start, end, diff;
int frame_count =0;
//char c='a';
char** sprite =malloc(sizeof(char*)*H);
for (int y=0; y<H; y++){
	sprite[y] =malloc(W+1);
	sprite[y][W] ='\0';}

while(1){//frame_count<50){
start =clock();

for (int y=0; y<H; y++)
	for (int x=0; x<W; x++)
		sprite[y][x] =fire_chars[rand()%FIRE_CHARS_N];
for (int y=0; y<H; y++)
	mvprintw(y,0,"%s",sprite[y]);
refresh();

// do something here
/*if (c>'z') c='a';
addch('\n');
addch(c);
refresh();
c++;*/

end =clock();
diff =end-start;
usleep(1000000/FPS-diff);
frame_count++;
}

getch();
endwin();
return 0;}
