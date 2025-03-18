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
#define ABLE		2
#define SUPERABLE	3

typedef struct{ int	y,x;	} v2i;
typedef struct{	float	i,j,k;	} v3f;

typedef struct ui Ui;
typedef struct game Game;
typedef struct settings Settings;
typedef struct ref Ref;
typedef struct player Player;
typedef struct world World;
typedef struct map Map;
typedef struct asset Asset;
typedef struct house House;
typedef struct interactive Interactive;
typedef struct instance	Instance;
typedef struct action Action;
typedef struct actionlist Actionlist;
typedef struct anim Anim;

// = internals =
struct ui{
	WINDOW		*gamw,*guiw;
	int		style;
};
struct game{
	v3f	hue;
	int	difficulty;
};
struct settings{
};

// = game objects =
struct player{
	int		y,x;
	int		hp;
	char*		name;
	Actionlist*	actionlist;
};
struct world	//TODO implement this
{
	Map*	maps;
};
struct map{
	int		type;
	int		h,w;
	char		**bg,**clsn,**fg,**tp;
	int		**it;
	char*		name;
	//Map*		oldmap;
	Instance*	inst;	//TODO add a max n of instances
	House*		house;
	Map		*previous,*next;
};			//TODO multiple houses per map

// = all assets =
struct ref{
	Interactive**	interactive;
	Action**	action;
	Anim**		anim;
};
struct asset{
	int	h,w;
	int	**ascii,**info;
};
struct house{
	int	y,x;
	int	h,w;
	int	**ascii,**info;
	//char*	path;
	Map*	map;
};
struct houselist{
	House		*house;
	Houselist	*previous,*next;
};
struct interactive{
	int		h,w;
	int		**ascii,**info,**inter;
	//char*		name;
	Actionlist*	actionlist;
};
enum inter_id{
	tree2,
	fruittree,
	stump,
	umbrella,
	nb_inter
};
struct instance{
	int		id;
	int		y,x;
	Interactive*	inter;
	Actionlist*	actionlist;
	Instance	*previous,*next;
};
struct anim{
	int		n;
	char*		chars;
};
enum anim_id{
	fire,
	nb_anim
};

// = actions system =
struct action{
	int		id;
	int		key;
	int		labellen, c;
	char*		label;
	void	(*action)(Instance* inst,Map* map,Ref* ref);
};
struct actionlist{
	int		condition;
	Action		*action;
	Actionlist	*previous,*next; //TODO maybe i don't need
};					//it to be double linked
enum action_id{
	fall_tree,
	pull_stump,
	harvest_fruits,
	light_fire,
	nb_action
};

// = start.c =
void title_screen(void);

// = menus.c =
Ui* create_ui(void);
void free_ui(Ui* ui);
Game* new_game(Ref* ref, Player** pl, Map** map, int random);
void free_game(Game* game);
v3f hue_selection(int);
Map* mapsize_selection(int);
int choose_difficulty(int);
void set_names(Map*, Player*, int);

// = game.c =
int run_game(Game* game, Ui* ui, Ref* ref, Player* pl, World* world);
Map* movement(Player* pl, World* world, Map* map, char c);
int check_collision(Map* map, int y, int x);
int check_tp(Map* map, int y, int x);

// = player.c =
Player* create_player(Ref* ref, char* name, int y, int x, int hp);
void free_player(Player* pl);

// = map.c =
void create_map(Ref* ref, Map* map);
Map* load_map(House* house,Map* oldmap);
void save_map(Map* map);
void free_map(Map* map);

// = assets.c =
Ref* load_ref(void);
void free_ref(Ref* ref);
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
Instance* get_inst(Map* map, int x, int y);
Instance* find_inst_id(Map* map, int id);
Instance* find_inst_inter(Ref* ref, Map* map, Interactive* inter);
void destroy_inst(Instance* it, Map* map);
void free_instlist(Instance* it);

// = actions.c =
Action** create_actiontable(void);
void free_actiontable(Action** actions);
void add_action(Actionlist** actionlist, Action* action, int condition);
void destroy_action(Actionlist* al);
Actionlist* find_action(char* label, Actionlist* al);
Actionlist* find_action_key(char key, Actionlist* al);
void free_actionlist(Actionlist* al);
//
void act(Ref* ref, Map* map, Player* pl, char c);
void act_fall_tree(Instance* inst, Map* map, Ref* ref);
void act_pull_stump(Instance* inst, Map* map, Ref* ref);
void act_harvest_fruits(Instance* inst, Map* map, Ref* ref);
void act_light_fire(Instance* inst, Map* map, Ref* ref);

// = display.c =
void display(Ui* ui, Player* pl, Map* map);
void display_map(WINDOW* gwin, Map* map, v2i pos);
void display_pl(WINDOW* gwin, Player* pl, Map* map);
void display_notice(WINDOW* gwin, Player* pl, Instance* in, Map* map, int interface_style);
void display_gui(WINDOW* guiwin, Player* pl, Map* map);

// = anim.c =
Anim** create_animtable(void);
void free_animtable(Anim** at);

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
int flen_line(FILE* f);
char* fread_line(FILE* f);
void fsize_map(FILE* f, int* h, int* w);
int** fread_map(FILE* f, int h, int w);
//
void clear_screen(int cp);
void debug_msg(char* str);
char* path_cat(char* path, char* file);
