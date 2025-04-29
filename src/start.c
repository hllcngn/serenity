#include "serenity.h"

int main(int ac, char** av){
srand(time(NULL)); //init
initscr(); cbreak(); noecho(); curs_set(0); start_color(); refresh();
init_color(COLOR_BLACK,rand()%100,rand()%100,rand()%100);
init_color(COLOR_WHITE,rand()%600+400,rand()%600+400,rand()%550+450);

//title_screen();

Ui* ui =create_ui(); //creating ui
Ref* ref =load_ref(); //loading assets reference

Game*	game; //creating game
Player*	pl;
World*	world;
//if (ac>1 && !strcmp(av[1],"random"))
	game =new_game(ref, &pl, &world, RANDOM);
//else	game =new_game(ref, &pl, &map, NORANDOM);
//TODO load game

run_game(game, ui, ref, pl, world); //launching game

free_ref(ref); //end
free_player(pl);
free_world(world); //TODO free maplist
free_game(game);
free_ui(ui);
endwin(); return 0;}


void title_screen(){
FILE* f =fopen("ass/titles/title.txt","r");
int h,w; fsize_map(f,&h,&w); rewind(f);
char l[w+2]; for (int i=0;fgets(l,w+2,f);i++)
	mvprintw((LINES-h)/2+i,(COLS-w)/2,"%s",l);
fclose(f); getch();}
