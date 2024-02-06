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
//TODO macro POSY(pos.y) and POSX

#define CP_NORMAL  1
#define CP_BASE	   2

typedef struct{ int	y,x;	} vect2i;
typedef struct{	float	i,j,k;	} vect3f;

typedef struct{ int	h,w;
		int	**map,**info;	} Asset;

typedef struct{ int	h,w,y,x;
		int	**map,**info,**inter;
		char*	label;			} Interactive;

typedef struct Instance{
	int		id;
	int		h,w,y,x;
	int		**map,**info,**inter;
	char*		label;
	struct Instance	*previous,*next;	} Instance;

typedef struct{
	int		h,w;
	int		**bg,**clsn,**fg,**it;
	char*		name;
	Instance*	inst;			} Map;

typedef struct{	char*	name;
		int	y,x;	} Player;

int** malloc_arrayint2(int h,int w);
int** calloc_arrayint2(int h,int w);
int** duplicate_arrayint2(int** arr,int h,int w);

Player* create_player(char* name, int y, int x);
int** fread_map(FILE* f, int h, int w);
Asset* load_asset(char* path);
Interactive* load_inter(char* path);
void create_map(Map* map);
void free_map(Map* map);
void free_instlist(Instance* it);
void free_asset(Asset* ass);
void free_inter(Interactive* inter);
void free_player(Player* pl);

void paste_asset(Map* map, int y, int x, Asset* ass);
void add_inst(Map* map, int y, int x, Interactive* inter);

int game(vect3f hue, Map* map, Player* pl);
void movement(char c, Player* pl, Map* map);
int check_collision(vect2i pos, Map* map);

void display_map(Map* map, vect2i pos);
void display_pl(Player* pl, Map* map);
