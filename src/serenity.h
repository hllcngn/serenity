#pragma once

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define K_QUIT	'='
#define K_UP	'w'
#define K_DOWN	's'
#define K_LEFT	'a'
#define K_RIGHT	'd'

#define WGAMEH	30
#define WGAMEW	50
#define WGAMEY  (LINES-WGAMEH)/2
#define WGAMEX  (COLS-WGAMEW)/2

#define CP_NORMAL  1
#define CP_BASE	   2

typedef struct{ int	y,x;	} vect2i;
typedef struct{	float	i,j,k;	} vect3f;

typedef struct{	char*	name;
		int	y,x;	} Player;

typedef struct{ int	h,w;
		int	**map,**info;	} Asset;

typedef struct Interactive{ int	id;
		int	h,w,y,x;
		char*	label;
		int	**map,**info,**inter;
		struct Interactive*	next;	} Interactive;

typedef struct{ int	h,w;
		char*	name;
		int	**bg,**clsn,**fg,**it;
		Interactive*	inter;		} Map;

int** malloc_arrayint2(int h,int w);
int** calloc_arrayint2(int h,int w);

void create_map(Map* map);
void free_map(Map* map);
void free_interlist(Interactive* inter);
Asset* load_asset(char* path);
Interactive* load_inter(char* path);
int** fread_map(FILE* f, int h, int w);
void free_asset(Asset* ass);
void free_inter(Interactive* inter);
void paste_asset(Map* map, int y, int x, Asset* ass);
void add_inter(Map* map, int y, int x, Interactive* inter);
Player* create_player(char* name, int y, int x);
void free_player(Player* pl);

void display_map(Map* map, vect2i pos);
void display_pl(Player* pl, Map* map);

int game(vect3f hue, Map* map, Player* pl);
void movement(char c, Player* pl, Map* map);
int check_collision(vect2i pos, Map* map);
