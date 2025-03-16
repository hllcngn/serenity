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
ref->anim =create_animtable();

// creating new game
v3f	hue;
Map*	map;
int	difficulty;
Player*	pl;
//if (ac>1 && !strcmp(av[1],"random"))
	new_game(ref, &hue, &map, &difficulty, &pl, RANDOM);
//else	new_game(&hue, &map, &difficulty, &pl, NORANDOM);
create_map(map,ref);

// creating ui
Ui* ui =malloc(sizeof(Ui));
v2i gamw_pos ={(LINES-GWIN_H)/2,(COLS-GWIN_W)/2};
ui->gamw =newwin(GWIN_H,GWIN_W,gamw_pos.y,gamw_pos.x);
ui->guiw =newwin(1,GWIN_W,gamw_pos.y-2,gamw_pos.x);
//ui->style =OLDSCHOOL;
ui->style =MODERN;

// launch game
game(hue, map, pl, ref, ui);

// end
free_actiontable(ref->action); free_intertable(ref->interactive);
free_animtable(ref->anim); free(ref);
free_player(pl); free_map(map);
delwin(ui->gamw); delwin(ui->guiw); free(ui);
endwin(); return 0;}



void title_screen(){
FILE* f =fopen("ass/titles/title.txt","r");
int h,w; fsize_map(f,&h,&w); rewind(f);
char l[w+2]; for (int i=0;fgets(l,w+2,f);i++)
	mvprintw((LINES-h)/2+i,(COLS-w)/2,"%s",l);
fclose(f); getch();}
