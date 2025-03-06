#include "serenity.h"


int main(int ac, char** av){

// init
initscr(); cbreak(); noecho(); curs_set(0); start_color(); refresh();
srand(time(NULL));
init_color(COLOR_BLACK,rand()%100,rand()%100,rand()%100);
init_color(COLOR_WHITE,rand()%600+400,rand()%600+400,rand()%600+400);

//title_screen();

// loading assets (into info structure)
Info	*info =malloc(sizeof(Info));
info->action =create_actiontable();
info->interactive =create_intertable(info->action);

// creating new game
vect3f	hue;
Map*	map;
int	difficulty;
Player*	pl;
//if (ac>1 && !strcmp(av[1],"random"))
	new_game(&hue, &map, &difficulty, &pl, 1);
//else	new_game(&hue, &map, &difficulty, &pl, 0);
create_map(map,info);

// launch game
game(hue, map, pl, info);

// end
free_actiontable(info->action); free_intertable(info->interactive); free(info);
free_player(pl); free_map(map);
endwin();

return 0;}



void title_screen(){
FILE* f =fopen("ass/title.txt","r");
int h,w; fsize_map(f,&h,&w); rewind(f);
char l[w+2]; for (int i=0;fgets(l,w+2,f);i++)
	mvprintw((LINES-h)/2+i,(COLS-w)/2,"%s",l);
fclose(f); getch();}
