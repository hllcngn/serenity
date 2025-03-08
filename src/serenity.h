#pragma once
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "controls.h"
#include "settings.h"

#define CP_NORMAL  1
#define CP_BASE	   2

#define NB_ACTION  3
#define NB_INTER   3

#define DIF_NORMAL	1
#define DIF_INFINITE	2
#define DIF_TROUBLED	3
#define DIF_FALL	4

#define OLDSCHOOL	0
#define MODERN		1

typedef struct{ int	y,x;	} vect2i;
typedef struct{	float	i,j,k;	} vect3f;

typedef struct{	char*	name;
		int	y,x,hp;	} Player;

typedef struct info	Info;
typedef struct map	Map;
typedef struct interactive Interactive;
typedef struct instance	Instance;
typedef struct action	Action;
typedef struct actionlist Actionlist;

struct info{
	Action**	action;
	Interactive**	interactive;};

typedef struct{ int	h,w;
		int	**map,**info;	} Asset;

typedef struct{	int	h,w;
		int	**map,**info;
		char*	path;		} House;

struct interactive{
	int		h,w;
	int		**map,**info,**inter;
	Actionlist*	actionlist;};

struct instance{
	int		id, y,x;
	Interactive*	inter;
	Instance	*previous,*next;};

struct action{
	int		c,k,labellen;
	char*		label;
	void	(*action)(Instance* inst,Map* map,Info* info);};

struct actionlist{
	Action		*action;
	Actionlist	*previous,*next;};

struct map{
	int		type;
	char*		name;
	int		h,w;
	int		**bg,**clsn,**fg;
	int		**it,**tp;
	Instance*	inst;
	House*		house;};

int** malloc_arrayint2(int h,int w);
int** calloc_arrayint2(int h,int w);
int** duplicate_arrayint2(int** arr,int h,int w);
void free_arrayint2(int** arr,int h,int w);
char* fread_line(FILE* f);
int flen_line(FILE* f);
void fsize_map(FILE* f, int* h, int* w);
int** fread_map(FILE* f, int h, int w);
void clear_screen(int cp);
void debug_msg(const char* str);
char* path_cat(const char* path, const char* file);

Action** create_actiontable(void);
void free_actiontable(Action** actions);
void add_action(Actionlist** actionlist, Action* action);
void destroy_action(Actionlist* al);
void free_actionlist(Actionlist* al);
void fall_tree(Instance* inst, Map* map, Info* info);
void pull_stump(Instance* inst, Map* map, Info* info);
void harvest_fruits(Instance* inst, Map* map, Info* info);

Asset* load_asset(char* path);
void paste_asset(Map* map, int y, int x, Asset* ass);
void free_asset(Asset* ass);
House* load_house(char* path);
void paste_house(Map* map, int y, int x, House* house);
void free_house(House* house);
Interactive** create_intertable(Action** actiontable);
Interactive* load_inter(char* path, Action** actionstable);
void free_inter(Interactive* inter);
void free_intertable(Interactive** intertable);
void add_inst(Map* map, int y, int x, Interactive* inter);
Instance* check_inst(vect2i pos, Map* map);
void destroy_inst(Instance* it, Map* map);
void free_instlist(Instance* it);

void create_map(Map* map, Info* info);
void free_map(Map* map);
Map* load_map(House* house);

Player* create_player(char* name, int y, int x, int hp);
void free_player(Player* pl);

int game(vect3f hue, Map* map, Player* pl, Info* info, int interface_style);
Map* movement(char c, Player* pl, Map* map);
int check_collision(vect2i pos, Map* map);
int check_tp(vect2i pos, Map* map);

void display_map(WINDOW* gwin, Map* map, vect2i pos);
void display_pl(WINDOW* gwin, Player* pl, Map* map);
void display_notice(WINDOW* gwin, vect2i pos, Map* map, int interface_style);

void title_screen(void);
void new_game(vect3f*, Map**, int*, Player**, int);
vect3f hue_selection(int);
Map* mapsize_selection(int);
int choose_difficulty(int);
void set_names(Map*, Player*, int);
