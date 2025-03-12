#pragma once
#include "settings.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#define NORANDOM	0
#define RANDOM		1

#define OLDSCHOOL	0
#define MODERN		1

#define CP_NORMAL	1
#define CP_BASE		2

#define OUTDOORS	0
#define INDOORS		1

#define UNABLE		0
#define ABLE		1
#define SUPERABLE	2
typedef struct{ int	y,x;	} v2i;
typedef struct{	float	i,j,k;	} v3f;

typedef struct ref Ref;
typedef struct asset Asset;
typedef struct house House;
typedef struct player Player;
typedef struct interactive Interactive;
typedef struct instance	Instance;
typedef struct action Action;
typedef struct actionlist Actionlist;
typedef struct map Map;
typedef struct game World;

// = game objects =
   struct ref
{	Action**	action;
	Interactive**	interactive;
}; struct player
{	int		y,x;
	int		hp;
	char*		name;
	Actionlist*	actionlist;
}; struct world		//TODO implement this
{	Map**	maps;
}; struct map
{	int		type;
	int		h,w;
	char		**bg,**clsn,**fg,**tp;
	int		**it;
	char*		name;
	Map*		oldmap;
	Instance*	inst;	//TODO add a max n of instances
	House*		house;
};			//TODO multiple houses per map

// = all assets =
   struct asset
{	int	h,w;
	int	**map,**info;
}; struct house
{	int	y,x;
	int	h,w;
	int	**map,**info;
	char*	path;
	Map*	oldmap;
}; struct interactive
{	int		h,w;
	int		**map,**info,**inter;
	char*		name;
	Actionlist*	actionlist;
}; struct instance
{	int		id;
	int		y,x;
	Interactive*	inter;
	Actionlist*	actionlist;
	Instance	*previous,*next; //TODO maybe i don't need
};					//it to be double linked
					//no that's useful to erase items easily
typedef enum int_id{	tree2,
			fruittree,
			stump,
			umbrella,
			nb_inter	}IID;

// = actions system =
   struct action
{	int		id;
	int		key;
	int		labellen, c;
	char*		label;
	void	(*action)(Instance* inst,Map* map,Ref* ref);
}; struct actionlist
{	int		condition;
	Action		*action;
	Actionlist	*previous,*next; //TODO maybe i don't need
};					//it to be double linked
typedef enum act_id{	fall_tree,
			pull_stump,
			harvest_fruits,
			light_fire,
			nb_action	}AID;

// = game.c =
int game(v3f hue, Map* map, Player* pl, Ref* ref, int interface_style);
Map* movement(char c, Player* pl, Map* map, Map* oldmap);
int check_collision(v2i pos, Map* map);
int check_tp(v2i pos, Map* map);
// = player.c =
Player* create_player(Ref* ref, char* name, int y, int x, int hp);
void free_player(Player* pl);

// = map.c =
void create_map(Map* map, Ref* ref);
Map* load_map(House* house,Map* oldmap);
void save_map(Map* map);
void free_map(Map* map);

// = assets.c =
Asset* load_asset(char* path);
void paste_asset(Map* map, Asset* ass, int y, int x);
void free_asset(Asset* ass);
House* load_house(char* path);
void paste_house(Map* map, House* house, int y, int x);
void free_house(House* house);
Interactive** create_intertable(Action** actiontable);
Interactive* load_inter(char* path, Action** actiontable);
void free_inter(Interactive* inter);
void free_intertable(Interactive** intertable);
Instance* add_inst(Map* map, int y, int x, Interactive* inter);
Instance* check_inst(v2i pos, Map* map);
Instance* find_inst(Ref* ref, Map* map, Interactive* inter);
void destroy_inst(Instance* it, Map* map);
void free_instlist(Instance* it);

// = actions.c =
Action** create_actiontable(void);
void free_actiontable(Action** actions);
void add_action(Actionlist** actionlist, Action* action, int condition);
void destroy_action(Actionlist* al);
Actionlist* find_action(char* label, Actionlist* al);
void free_actionlist(Actionlist* al);
//
void act_fall_tree(Instance* inst, Map* map, Ref* ref);
void act_pull_stump(Instance* inst, Map* map, Ref* ref);
void act_harvest_fruits(Instance* inst, Map* map, Ref* ref);
void act_light_fire(Instance* inst, Map* map, Ref* ref);


// = display.c =
void display_map(WINDOW* gwin, Map* map, v2i pos);
void display_pl(WINDOW* gwin, Player* pl, Map* map);
void display_notice(WINDOW* gwin, Player* pl, Instance* in, v2i pos, Map* map, int interface_style);
void display_gui(WINDOW* guiwin, Player* pl, Map* map);

// = start.c =
void title_screen(void);
// = menus.c =
void new_game(Ref* ref, v3f* hue, Map** map, int* diff, Player** pl, int random);
v3f hue_selection(int);
Map* mapsize_selection(int);
int choose_difficulty(int);
void set_names(Map*, Player*, int);


// = tool functions =
int** malloc_arrayint2(int h,int w);
int** calloc_arrayint2(int h,int w);
int** duplicate_arrayint2(int** arr,int h,int w);
int** spacoc_arrayint2(int h,int w);
void fput_arrayint2(FILE* f,int** arr,int h,int w);
void free_arrayint2(int** arr,int h,int w);
char** malloc_arraychar2(int h,int w);
char** calloc_arraychar2(int h,int w);
char** spacoc_arraychar2(int h,int w);
void fput_arraychar2(FILE* f,char** arr,int h,int w);
void free_arraychar2(char** arr,int h,int w);
//
char* fread_line(FILE* f);
int flen_line(FILE* f);
void fsize_map(FILE* f, int* h, int* w);
int** fread_map(FILE* f, int h, int w);
//
void clear_screen(int cp);
void debug_msg(const char* str);
char* path_cat(const char* path, const char* file);
