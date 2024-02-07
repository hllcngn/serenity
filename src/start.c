#include "serenity.h"
void clear_screen(int cp);
vect3f hue_selection(void);
Map* mapsize_selection(void);
void set_names(Map* map, Player* pl);


int main(int ac, char** av){
initscr(); cbreak(); noecho(); curs_set(0);
start_color(); refresh();
srand(time(NULL));

FILE* f =fopen("ass/title.txt","r");
char* l =NULL; size_t n;
int i=0; while (getline(&l,&n,f)!=-1){
	mvprintw((LINES-7)/2+i,(COLS-100)/2,"%s",l); i++;}
free(l); fclose(f); getch();

vect3f hue =hue_selection();
Map* map =mapsize_selection();
Player* pl =create_player(NULL,2,5);
clear_screen(2); refresh(); //refresh needed?
set_names(map,pl);
clear_screen(2); refresh();

mvprintw(0,0,"about to create Info struct\n"); getch();
Info *info =malloc(sizeof(Info));
info->actions =create_actionstable();
info->interactives =create_intertable(info->actions);
mvprintw(0,0,"about to create map\n"); getch();
create_map(map,info);

mvprintw(0,0,"map created, about to start game\n"); getch();
game(hue, map, pl, info);

free_actionstable(info->actions); free_intertable(info->interactives);
free_player(pl); free_map(map);
endwin();	return 0;}



vect3f hue_selection(void){
for (int cp=20,i=1000;cp<30;cp++,i-=30){
	init_color(cp,i,i,i);
	init_pair(cp,COLOR_BLACK,cp);
	init_pair(100+cp,cp,COLOR_BLACK);}
for (int cp=30,i=1000,j=1000;cp<40;cp++,i-=10,j-=20){
	init_color(cp,1000,i,j);
	init_pair(cp,COLOR_BLACK,cp);
	init_pair(100+cp,cp,COLOR_BLACK);}
for (int cp=40,i=1000;cp<50;cp++,i-=25){
	init_color(cp,i,i,1000);
	init_pair(cp,COLOR_BLACK,cp);
	init_pair(100+cp,cp,COLOR_BLACK);}
WINDOW* whue =newwin(7,30,(LINES-7)/2,(COLS-30)/2);
char c=0; int hl =20; do {
switch (c){  case 'a':	if (hl%10)   hl--;	break;
	     case 'd':	if (hl%10<9) hl++;	break;
	     case 'w':	if (hl>29)   hl-=10;	break;
	     case 's':	if (hl<40)   hl+=10;	break;
	     default:				break;}
clear_screen(hl); refresh();
wattron(whue,COLOR_PAIR(100+hl)); box(whue,0,0);
mvwprintw(whue,0,(30-15)/2,"pls correct hue");
mvwprintw(whue,6,(30-18)/2,"wasd - 1 to select");
wmove(whue,2,5); for (int i=20;i<30;i++){
	wattron(whue,COLOR_PAIR(i));
	if (i==hl){ waddch(whue, '.'); waddch(whue, '.');}
	else { waddch(whue,' '); waddch(whue,' ');}}
wmove(whue,3,5); for (int i=30;i<40;i++){
	wattron(whue,COLOR_PAIR(i));
	if (i==hl){ waddch(whue, '.'); waddch(whue, '.');}
	else { waddch(whue,' '); waddch(whue,' ');}}
wmove(whue,4,5); for (int i=40;i<50;i++){
	wattron(whue,COLOR_PAIR(i));
	if (i==hl){ waddch(whue, '.'); waddch(whue, '.');}
	else { waddch(whue,' '); waddch(whue,' ');}}
wrefresh(whue); } while ((c=getch())!='1'); delwin(whue);
vect3f hue;
if (hl>40){	int i =1000-hl%10*25;
		init_color(21,i,i,1000);
		hue =(vect3f){i/1000.0,i/1000.0,1};}
else if (hl>30){ int i =1000-hl%10*10; int j =1000-hl%10*20;
		init_color(21,1000,i,j);
		hue =(vect3f){1,i/1000.0,j/1000.0};}
else {		int i =1000-hl%10*30;
		init_color(21,i,i,i);
		hue =(vect3f){i/1000.0,i/1000.0,i/1000.0};}
init_pair(1,21,COLOR_BLACK);
init_pair(2,COLOR_BLACK,21);	return hue;}


Map* mapsize_selection(void){
WINDOW* wmap =newwin(9,30,(LINES-9)/2,(COLS-30)/2);
wattron(wmap, COLOR_PAIR(1)); box(wmap,0,0);
mvwprintw(wmap,0,(30-15)/2,"choose map size");
mvwprintw(wmap,2,3,"1. pocket");
mvwprintw(wmap,3,3,"2. small");
mvwprintw(wmap,4,3,"3. normal");
mvwprintw(wmap,5,3,"4. large");
mvwprintw(wmap,6,3,"5. xtra large"); wrefresh(wmap);
char c; while ((c=getch())<'1'||c>'5'); delwin(wmap);
Map* map =malloc(sizeof(Map));
switch (c){  case '1':	map->h=50;  map->w=100;	break;
	     case '2':	map->h=100; map->w=200;	break;
	     case '3':	map->h=150; map->w=300;	break;
	     case '4':	map->h=200; map->w=400;	break;
	     case '5':	map->h=250; map->w=500; break;
	     default:				break;}	return map;}


void set_names(Map* map, Player* pl){
char* name =malloc(32); name[0]='\0';
strcpy(name, "Domain of Saint ");
switch (rand()%3){ case 0:  strcat(name, "George");	break;
		   case 1:  strcat(name, "Joseph");	break;
		   default: strcat(name, "Martin");	break;}
WINDOW* wname =newwin(7,37,(LINES-7)/2,(COLS-37)/2);
wattron(wname, COLOR_PAIR(1)); box(wname,0,0);
mvwprintw(wname,1,3,"Map name:");
mvwprintw(wname,2,3,"%s",name);
mvwprintw(wname,4,3,"Player name:");
mvwprintw(wname,6,(37-29)/2,"space: change - enter: accept");
wmove(wname,2,3+22); curs_set(1); wrefresh(wname);
char c=0; do { if (c!=' ') c =getch();
	  if (c==' '){
		name[16]='\0';
		mvwprintw(wname,2,3+16,"                ");
		wmove(wname,2,3+16);
		c=0; int len=16; do {	//TODO backspace
			if (c>='a'&&c<='z'){
				name[len]=c; len++; name[len]='\0';}
			else if (c==' ') break;
			mvwprintw(wname,2,3,"%s",name);
			wrefresh(wname);
			}while ((c=getch())!='\n'&&len<31);}
}while (c!='\n'); map->name =strdup(name);
c=' '; do { if (c!=' ') c= getch();
	  if (c==' '){
		name[0]='\0';
		mvwprintw(wname,5,3,"                ");
		wmove(wname,5,3);
		c=0; int len=0; do {	//TODO backspace
			if (c>='a'&&c<='z'){
				name[len]=c; len++; name[len]='\0';}
			else if (c==' ') break;
			mvwprintw(wname,5,3,"%s",name);
			wrefresh(wname);
			}while ((c=getch())!='\n'&&len<16);}
}while (c!='\n'); pl->name =strdup(name);
curs_set(0); free(name); delwin(wname);	return;}
