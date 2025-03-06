#include "serenity.h"

void new_game(vect3f* hue, Map** map, int* diff, Player** pl, int random){
*hue	=hue_selection(random);
*map	=mapsize_selection(random);
clear_screen(2); refresh();
*diff	=choose_difficulty(random);
*pl     =create_player(NULL,(*map)->h/2,(*map)->w/2);
clear_screen(2); refresh();
set_names(*map,*pl, random);
clear_screen(2); refresh();}


vect3f hue_selection(int random){
vect3f hue;
if (random == 1){
	int i = rand()%650+350, j = rand()%650+350, k = rand()%650+350;
	init_color(21,i,j,k);
	hue = (vect3f){i/1000.0,j/1000.0,k/1000.0};
} else {
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
if (hl>40){	int i =1000-hl%10*25;
		init_color(21,i,i,1000);
		hue =(vect3f){i/1000.0,i/1000.0,1};}
else if (hl>30){ int i =1000-hl%10*10; int j =1000-hl%10*20;
		init_color(21,1000,i,j);
		hue =(vect3f){1,i/1000.0,j/1000.0};}
else {		int i =1000-hl%10*30;
		init_color(21,i,i,i);
		hue =(vect3f){i/1000.0,i/1000.0,i/1000.0};}}
init_pair(CP_NORMAL,21,COLOR_BLACK);
init_pair(CP_BASE,COLOR_BLACK,21);	return hue;}


Map* mapsize_selection(int random){
char c;
if (random == 1) {
c = rand()%3+1 +'0';
} else {
WINDOW* wmap =newwin(9,30,(LINES-9)/2,(COLS-30)/2);
wattron(wmap, COLOR_PAIR(CP_NORMAL)); box(wmap,0,0);
mvwprintw(wmap,0,(30-15)/2,"choose map size");
mvwprintw(wmap,2,3,"1. pocket");
mvwprintw(wmap,3,3,"2. small");
mvwprintw(wmap,4,3,"3. normal");
mvwprintw(wmap,5,3,"4. large");
mvwprintw(wmap,6,3,"5. xtra large"); wrefresh(wmap);
while ((c=getch())<'1'||c>'5'); delwin(wmap);}
Map* map =malloc(sizeof(Map));
switch (c){  case '1':	map->h=50;  map->w=100;	break;
	     case '2':	map->h=100; map->w=200;	break;
	     case '3':	map->h=150; map->w=300;	break;
	     case '4':	map->h=200; map->w=400;	break;
	     case '5':	map->h=250; map->w=500; break;
	     default:				break;}	return map;}


int choose_difficulty(int random){
char c;
if (random == 1)
c = rand()%4+1 +'0';
else {
WINDOW* wdiff =newwin(8,30,(LINES-8)/2,(COLS-30)/2);
wattron(wdiff, COLOR_PAIR(CP_NORMAL)); box(wdiff,0,0);
mvwprintw(wdiff,0,(30-17)/2,"select difficulty");
mvwprintw(wdiff,2,3,"1. solar");
mvwprintw(wdiff,3,3,"2. solar - infinite");
mvwprintw(wdiff,4,3,"3. troubled");
mvwprintw(wdiff,5,3,"4. fall of heavens"); wrefresh(wdiff);
char c; while ((c=getch())<'1'||c>'5'); delwin(wdiff);}
return c-'0';}


void set_names(Map* map, Player* pl, int random){
char* name =malloc(32); name[0]='\0';
strcpy(name, "Domain of Saint ");
int casex = rand()%3;
switch (casex){	case 0:  strcat(name, "George");	break;
		case 1:  strcat(name, "Joseph");	break;
		default: strcat(name, "Martin");	break;}

if (random == 1) {
int mnl=rand()%6+5;
char* mapname = malloc(mnl+1); mapname[mnl]='\0';
mapname[0]='A'+rand()%26;
for (int i=1; i<mnl; i++)
	mapname[i]='a'+rand()%26;
map->name = mapname;

name[0]='\0';
switch (casex){	case 0:  strcat(name, "George");	break;
		case 1:  strcat(name, "Joseph");	break;
		default: strcat(name, "Martin");	break;}
pl->name =strdup(name);

} else {
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
}while (c!='\n');
map->name =strdup(name);
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
}while (c!='\n');
pl->name =strdup(name); curs_set(0); delwin(wname);}
free(name);}
