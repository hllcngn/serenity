#include "serenity.h"


int main(int ac, char** av){
// init
initscr(); cbreak(); noecho(); curs_set(0); start_color(); refresh();
srand(time(NULL));
init_color(0,rand()%100,rand()%100,rand()%100);
init_color(7,rand()%650+350,rand()%650+350,rand()%650+350);
//title_screen();

// loading assets
Info *info =malloc(sizeof(Info));
info->action =create_actiontable();
info->interactive =create_intertable(info->action);

// new game
vect3f hue; Map* map; int difficulty; Player* pl;
if (ac>1 && !strcmp(av[1],"random"))
	new_game(&hue, &map, &difficulty, &pl, 1);
else	new_game(&hue, &map, &difficulty, &pl, 0);
create_map(map,info);

game(hue, map, pl, info);

free_actiontable(info->action); free_intertable(info->interactive);
free_player(pl); free_map(map); endwin();	return 0;}



void title_screen(){
FILE* f =fopen("ass/title.txt","r");
char* l =NULL; size_t n;
int i=0; while (getline(&l,&n,f)!=-1){
	mvprintw((LINES-7)/2+i,(COLS-100)/2,"%s",l); i++;}
	//these numbers correspond to the size of the boxed title text
//TODO use a generic tool function similar to fread_map
free(l); fclose(f); getch();}
