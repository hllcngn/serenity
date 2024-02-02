#include "serenity.h"
void clear_screen(int cp);


int main(int ac, char** av){
initscr(); cbreak(); noecho(); curs_set(0);
start_color(); refresh();

FILE* f =fopen("ass/title.txt","r");
char* l =NULL; size_t n;
while (getline(&l,&n,f)!=-1)
	printw("%s",l);
free(l); fclose(f); getch();

for (int cp=20,i=1000;cp<30;cp++,i-=30){
	init_color(cp,i,i,i);
	init_pair(cp,0,cp);
	init_pair(100+cp,cp,0);}
for (int cp=30,i=1000,j=1000;cp<40;cp++,i-=10,j-=20){
	init_color(cp,1000,i,j);
	init_pair(cp,0,cp);
	init_pair(100+cp,cp,0);}
for (int cp=40,i=1000;cp<50;cp++,i-=25){
	init_color(cp,i,i,1000);
	init_pair(cp,0,cp);
	init_pair(100+cp,cp,0);}
WINDOW* whue =newwin(7,30,(LINES-7)/2,(COLS-30)/2);
char c=0; int hl =20; do {
switch(c) {
	case 'a':	if (hl%10)   hl--;	break;
	case 'd':	if (hl%10<9) hl++;	break;
	case 'w':	if (hl>19)   hl-=10;	break;
	case 's':	if (hl<40)   hl+=10;	break;
	default:				break;}
clear_screen(hl); refresh();
wattron(whue,COLOR_PAIR(100+hl)); box(whue,0,0);
mvwprintw(whue,0,(30-15)/2,"pls correct hue");
mvwprintw(whue,6,(30-18)/2,"wasd - 1 to select");
wmove(whue,2,5);
for (int i=20;i<30;i++){
	wattron(whue,COLOR_PAIR(i));
	if (i==hl) { waddch(whue, '.'); waddch(whue, '.');}
	else { waddch(whue,' '); waddch(whue,' ');}}
wmove(whue,3,5);
for (int i=30;i<40;i++){
	wattron(whue,COLOR_PAIR(i));
	if (i==hl) { waddch(whue, '.'); waddch(whue, '.');}
	else { waddch(whue,' '); waddch(whue,' ');}}
wmove(whue,4,5);
for (int i=40;i<50;i++){
	wattron(whue,COLOR_PAIR(i));
	if (i==hl) { waddch(whue, '.'); waddch(whue, '.');}
	else { waddch(whue,' '); waddch(whue,' ');}}
wrefresh(whue);
} while ((c=getch())!='1');

endwin();	return 0;}


void clear_screen(int cp){
move(0,0); attron(COLOR_PAIR(cp));
for (int y=0;y<LINES;y++)
	for (int x=0;x<COLS;x++)
		addch(' ');	return;}
