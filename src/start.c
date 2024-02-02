#include "serenity.h"

int main(int ac, char** av){
initscr(); cbreak(); noecho(); curs_set(0);
start_color(); refresh();

FILE* f =fopen("ass/title.txt","r");
char* l =NULL; size_t n;
while(getline(&l,&n,f)!=-1)
	printw("%s",l);
free(l); fclose(f);

getch();

endwin();	return 0;}
