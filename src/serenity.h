#pragma once
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "controls.h"
#include "settings.h"

#define NB_ACTION  3 //TODO turn these into lists
#define NB_INTER   3

#define CP_NORMAL  1
#define CP_BASE	   2

#define NORANDOM	0
#define RANDOM		1
#define OLDSCHOOL	0
#define MODERN		1



typedef struct{ int	y,x;	} v2i;
typedef struct{	float	i,j,k;	} v3f;

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



typedef struct ref Ref;
typedef struct interactive Interactive;
typedef struct instance	Instance;
typedef struct action Action;
typedef struct actionlist Actionlist;
typedef struct map Map;

struct ref{
	Action**	action;
	Interactive**	interactive;	};

typedef struct{ int	h,w;
		int	**map,**info;	} Asset;

typedef struct{	int	h,w;
		int	**map,**info;
		char*	path;		} House;

struct interactive{
	int		h,w;
	int		**map,**info,**inter;
	Actionlist*	actionlist;	};

struct instance{
	int		id;
	int		y,x;
	Interactive*	inter;
	Instance	*previous,*next;};

struct action{
	int		key;
	int		labellen, c;
	char*		label;
	void	(*action)(Instance* inst,Map* map,Ref* ref);};

struct actionlist{
	Action		*action;
	Actionlist	*previous,*next;};

struct map{
	int		type;
	int		h,w;
	int		**bg,**clsn,**fg;
	int		**it,**tp;
	char*		name;
	Instance*	inst;
	House*		house;		};

typedef struct{	int	y,x;
		int	hp;
		char*	name;		} Player;

void title_screen(void);
void new_game(v3f*, Map**, int*, Player**, int);
v3f hue_selection(int);
Map* mapsize_selection(int);
int choose_difficulty(int);
void set_names(Map*, Player*, int);

int game(v3f hue, Map* map, Player* pl, Ref* ref, int interface_style);
Map* movement(char c, Player* pl, Map* map);
int check_collision(v2i pos, Map* map);
int check_tp(v2i pos, Map* map);

Player* create_player(char* name, int y, int x, int hp);
void free_player(Player* pl);

void create_map(Map* map, Ref* ref);
void free_map(Map* map);
Map* load_map(House* house);

void display_map(WINDOW* gwin, Map* map, v2i pos);
void display_pl(WINDOW* gwin, Player* pl, Map* map);
void display_notice(WINDOW* gwin, Instance* in, v2i pos, Map* map, int interface_style);
void display_gui(WINDOW* guiwin, Player* pl, Map* map);

Asset* load_asset(char* path);
void paste_asset(Map* map, Asset* ass, int y, int x);
void free_asset(Asset* ass);
House* load_house(char* path);
void paste_house(Map* map, int y, int x, House* house);
void free_house(House* house);
Interactive** create_intertable(Action** actiontable);
Interactive* load_inter(char* path, Action** actionstable);
void free_inter(Interactive* inter);
void free_intertable(Interactive** intertable);
void add_inst(Map* map, int y, int x, Interactive* inter);
Instance* check_inst(v2i pos, Map* map);
void destroy_inst(Instance* it, Map* map);
void free_instlist(Instance* it);

Action** create_actiontable(void);
void free_actiontable(Action** actions);
void add_action(Actionlist** actionlist, Action* action);
void destroy_action(Actionlist* al);
void free_actionlist(Actionlist* al);
void fall_tree(Instance* inst, Map* map, Ref* ref);
void pull_stump(Instance* inst, Map* map, Ref* ref);
void harvest_fruits(Instance* inst, Map* map, Ref* ref);
