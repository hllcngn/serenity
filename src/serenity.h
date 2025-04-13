#pragma once
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
//
#include "settings.h"


#define NORANDOM	0
#define RANDOM		1
//
#define CP_NORMAL	1
#define CP_BASE		2
//
#define OUTDOORS	0
#define INDOORS		1
//
#define UNABLE		0
#define ABLE		2
#define SUPERABLE	3
//
#define LOADED		0
#define GENERATED	1


#define TREEBASE_N 8
#define FRUITBASE_N 9
static char treebase[TREEBASE_N] ={'G','D','C','u','Y','k','v','i'};
static char fruitbase[FRUITBASE_N] ={'p','b','B','d','q','9','o','6','8'};


enum inter_id{
	tree2,
	fruittree,
	stump,
	umbrella,
	nb_inter
};
enum action_id{
	fall_tree,
	pull_stump,
	harvest_fruits,
	light_fire,
	nb_action
};
enum anim_id{
	fire,
	nb_anim
};


typedef struct{ int	y,x;		} v2i;
typedef struct{ int	y,x,h,w;	} v4i;
typedef struct{	float	i,j,k;		} v3f;
typedef struct list List;
struct list{ //NB perhaps interestingly, you can have lists with different types of items
	int	type;
	void*	item;
	void*	inst;
	List	*prev,*next;
};
enum node_type{
	t_inst,
	t_action,
	t_item,
	t_map,
	t_house
};


typedef struct settings Settings; //general settings
typedef struct game Game; //game settings
typedef struct ui Ui;
typedef struct ref Ref; //assets reference
typedef struct player Player;
typedef struct world World;
typedef struct map Map;
typedef struct interactive Interactive;
typedef struct asset Asset;
typedef struct house House;
typedef struct action Action;
typedef struct item Item;
typedef struct anim Anim;
//lists:
typedef struct instance	Instance;
typedef struct maplist Maplist;
typedef struct houselist Houselist;
typedef struct actionlist Actionlist;
typedef struct itemlist Itemlist;

// - game internals -
struct settings{
};
struct game{
	v3f	hue;
	int	difficulty;
};
struct ui{
	WINDOW		*gamw,*guiw;
};

// - game objects -
struct player{
	int		y,x;
	int		hp;
	char*		name;
	Actionlist*	actionlist;
	Itemlist*	inventory;
};
struct world{
	Map*		curr;
	Maplist*	maplist;
//	Tp*		tplist;
};
/*
struct tp{
	int	srcmapid,dstmapid;
	int	srcy,srcx,dsty,dstx;
};
*/
struct map{
	int		id; //TODO use map ids
	int		type;
	int		h,w;
	char*		name;
	char		**bg,**clsn,**fg,**tp;
	//Instance*	inst;	//TODO add a max n of instances
	List*		inst;
	Houselist*	houselist;
	Maplist*	maplist;
};

// - assets -
struct ref{
	Interactive**	interactive;
	Action**	action;
	Anim**		anim;
};
struct asset{
	int	h,w;
	char	**ascii,**info;
};
struct house{			//TODO in order to make things work
	int		id;	// make houses into regular inter/inst
	int		y,x;	// but then i won't be able to delete
	int		h,w;	// all the inner rooms at once easily
	char		**ascii,**info; //so i still need some sort of house struct
};				// with inst and maplist
/*				//and actually a list of instances across all outdoors maps
struct house{			//which is where it becomes useful to have
	int		id;	//the list struct distinct from the instances
	Instance*	inst;	// then we can always have a list of house instances
	Maplist		*maplist;	//on the map if needed
};				//but we also need to connect the instance to the house struct
*/				//else just look for house in new house struct list by id
struct interactive{		//which kinda works if we just have many different lists of stuff
	int		h,w;
	char		**ascii,**info,**inter;
	Actionlist*	actionlist;
	int		type; // = LOADED/GENERATED
};			//TODO migrate to using this type instead of the instance's
struct anim{
	int		n;
	char*		chars;
};

// - others -
struct action{
	int		id;
	int		key;
	int		labellen, c;
	char*		label;
	void	(*action)(List* inst,Map* map,Ref* ref);
};
struct item{
	char*	name;
};

// - lists -
struct instance{
	int		type;
	int		y,x;
	Interactive*	inter;
	Interactive*	ascii; //for generated ones TODO change this so that
	Actionlist*	actionlist; //the 'inter' is always where to look
	// tp (or external tp list)    //just check at freeing to delete it if generated
	Instance	*previous,*next;
};
struct maplist{
	Map*		map;
	Maplist		*previous,*next;
};
struct houselist{
	House*		house;
	Houselist	*previous,*next;
};
struct actionlist{
	int		condition;
	Action*		action;
	Actionlist	*previous,*next;
};
struct itemlist{
	int		n;
	Item*		item;
	Itemlist	*previous, *next;
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
// = display.c =
void display(Ui* ui, Player* pl, Map* map);
void display_map(WINDOW* gwin, Map* map, int ply, int plx);
void display_pl(WINDOW* gwin, Player* pl, Map* map, List* inst);
void display_notice(WINDOW* gwin, Player* pl, Map* map, List* in);
void display_gui(WINDOW* guiwin, Player* pl, Map* map);
// = anim.c =
Anim** create_animtable(void);
void free_animtable(Anim** at);

// = player.c =
Player* create_player(Ref* ref, char* name, int y, int x, int hp);
void free_player(Player* pl);
// = map.c =
World* create_world(void);
void create_map(Ref* ref, World* world, Map* map);
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
Instance* create_inst_from_inter(Interactive* inter);
Instance* add_inst(Map* map, int y, int x, Instance* inst);
Instance* insert_inst(Instance** list, Instance* inst);
Instance* get_inst(Map* map, int x, int y);
//Instance* find_inst_inter(Ref* ref, Map* map, Interactive* inter);
void destroy_inst(Instance* it, Map* map);
void free_instlist(Instance* it);
// = inter.c =
Instance* create_tree(Ref* ref);
Instance* create_fruittree(Ref* ref);

// = actions.c =
Action** create_actiontable(void);
void free_actiontable(Action** actions);
void add_action(Actionlist** actionlist, Action* action, int condition);
void destroy_action(Actionlist* al);
void al_remove_duplicates(Actionlist* al);
Actionlist* find_action(char* label, Actionlist* al);
Actionlist* find_action_key(char key, Actionlist* al);
void free_actionlist(Actionlist* al);
Actionlist* generate_complete_al(Player* pl, List* in);
//
void act(Ref* ref, Map* map, Player* pl, char c);
void act_fall_tree(List* inst, Map* map, Ref* ref);
void act_pull_stump(List* inst, Map* map, Ref* ref);
void act_harvest_fruits(List* inst, Map* map, Ref* ref);
void act_light_fire(List* inst, Map* map, Ref* ref);

// = tool functions =
int** malloc_arrayint2(int h,int w);
int** calloc_arrayint2(int h,int w);
int** duplicate_arrayint2(int** arr,int h,int w);
int** spacoc_arrayint2(int h,int w);
void** fill_rectangle_arrayint2(int** arr, int y, int x, int h,int w, int c);
void fput_arrayint2(FILE* f,int** arr,int h,int w);
void free_arrayint2(int** arr,int h,int w);
char** malloc_arraychar2(int h,int w);
char** calloc_arraychar2(int h,int w);
char** duplicate_arraychar2(char** arr,int h,int w);
char** spacoc_arraychar2(int h,int w);
void** fill_rectangle_arraychar2(char** arr, int y, int x, int h,int w, char c);
void fput_arraychar2(FILE* f,char** arr,int h,int w);
void free_arraychar2(char** arr,int h,int w);
//
int flen_line(FILE* f);
char* fread_line(FILE* f);
void fsize_map(FILE* f, int* h, int* w);
char** fread_map(FILE* f, int h, int w);
//
void clear_screen(int cp);
void debug_msg(char* str);
char* path_cat(char* path, char* file);

// = list functions =
List* list_new(int type, void* item, void* hints);
void insert_before(List** list, List* new);
void insert_after(List** list, List* new);
void list_free(List* list);
void node_free(List* tf);
void node_remove(List** list, List* trm);
//
void list_inst_insert(List** list, List* new);
List* list_inst_get(List* list, int y, int x);
