#include <ncurses.h>

int main(int ac, char** av){
initscr(); start_color(); refresh();

init_color(21, 800,100,200);
init_pair(10, COLOR_BLACK, 21);

attron(COLOR_PAIR(10));
printw("should be red");
getch();

endwin();
return 0;}
