#include "serenity.h"


int main(int ac, char** av){

// init
initscr(); cbreak(); noecho(); curs_set(0); start_color(); refresh();
srand(time(NULL));
init_color(COLOR_BLACK,rand()%100,rand()%100,rand()%100);
init_color(COLOR_WHITE,rand()%600+400,rand()%600+400,rand()%600+400);

//title_screen();

// loading instantiable assets (into ref structure)
Ref	*ref =malloc(sizeof(Ref));
ref->action =create_actiontable();
ref->interactive =create_intertable(ref->action);

// creating new game
v3f	hue;
Map*	map;
int	difficulty;
Player*	pl;
//if (ac>1 && !strcmp(av[1],"random"))
	new_game(ref, &hue, &map, &difficulty, &pl, RANDOM);
//else	new_game(&hue, &map, &difficulty, &pl, NORANDOM);
create_map(map,ref);

// launch game
//game(hue, map, pl, ref, OLDSCHOOL);
game(hue, map, pl, ref, MODERN);

// end
free_actiontable(ref->action); free_intertable(ref->interactive); free(ref);
free_player(pl); free_map(map);
endwin(); return 0;}

//mvprintw(0,0,"segv\n");getch();


void title_screen(){
FILE* f =fopen("ass/titles/title.txt","r");
int h,w; fsize_map(f,&h,&w); rewind(f);
char l[w+2]; for (int i=0;fgets(l,w+2,f);i++)
	mvprintw((LINES-h)/2+i,(COLS-w)/2,"%s",l);
fclose(f); getch();}
