#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define FPS 10

int main(int ac,char** av){
initscr(); refresh();

clock_t start, end, diff;
int frame_count =0;
char c='a';

while(frame_count<50){
start =clock();

// do something here
if (c>'z') c='a';
addch('\n');
addch(c);
refresh();
c++;

end =clock();
diff =end-start;
usleep(1000000/FPS-diff);
frame_count++;
}

getch();
endwin();
return 0;}
