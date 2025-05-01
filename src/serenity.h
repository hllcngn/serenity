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


#define TREEBASE_N 6
#define FRUITBASE_N 4
static char treebase[TREEBASE_N] ={'C','O','G','c','Y','k'};
static char fruitbase[FRUITBASE_N] ={'9','o','6','8'};


enum inter_id{
	tree2,
	fruittree,
	stump,
	umbrella,
	portal,
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
typedef struct list	List;
struct list{ //NB perhaps interestingly, you can have lists with different types of items
	int	type;
	void*	item;
	void*	inst;
	List	*prev,*next;
};
enum node_type{
	t_inst,
	t_act,
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
typedef struct tp Tp;
typedef struct inter Inter;
typedef struct asset Asset;
typedef struct house House;
typedef struct action Action;
typedef struct item Item;
typedef struct anim Anim;
//lists:
typedef struct inst Inst;
typedef struct houselist Houselist;
typedef struct actinst Actinst;
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
	List*		actlist;
	Itemlist*	inventory;
};
struct world{
	Map*		curr;
	//Maplist*	maplist;
	List*		maplist;
};

struct map{
	int		type;
	int		id; //TODO use map ids
	int		h,w;
	char*		name;
	char		**bg;
	List*		inst; //TODO add a max n of instances
	//List*		tplist;
	Tp*		tp;
	Houselist*	houselist;
};

struct tp{	//looks good but it would be tedious to fill in
		//the other optiom i thought off would be to have relative
		//positions to maps in the world
		//admitting no change in scaling
		//but that's overkill
		//even though somewhat sane
		//but that doesn't account for actual teleporters
		//[this would be relative to the teleporter inst]
		//this is where class inheritance for instances would be useful
		//because i would need to add a couple fields to inter
		//in order for it to encompass tp too
		//[no i would need tp interaction in Inter]
		//[and teleporter info on the Inst]
		//except i can make it a tp struct
		//[and i am going to for simplicity]
		//add it to the inst list as t_tp
		//just for two fields
		//then it would be good to have a world tp list,
		//[it works fine on map/inst so that's useless]
		//based on instances where if it's not a house, look it up
		//also need to add one field to inter for houses
		//if i want to link the facade with the house maps
		//[or i link it in the house struct, that would make sense]
		//or have a world/map list of houses
		//all this is facilitated by the type field in inst
		//if it's anything special, look it up in the appropriate list
	char	srcit,dstit;
	//int	srcy,srcx,
	int	dsty,dstx;
	Map	*srcmap,*dstmap;
	Inst	*srcinst,*dstinst;//would only work if the map is in memory
				  //or tp on the same map
				  //or dstinst is instantiated even if the map doesn't exist
				  //and it's placed on the map while loading it
};


// - assets -
struct ref{
	Inter**		inter;
	Action**	action;
	Anim**		anim;
};
struct asset{
	int	h,w;
	char	**ascii;
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
	Inst*	inst;	// then we can always have a list of house instances
	Maplist		*maplist;	//on the map if needed
};				//but we also need to connect the instance to the house struct
*/				//else just look for house in new house struct list by id
/*
struct building{
	Inst*		facade;
	Maplist*	maplist;
};
*/
struct inter{		//which kinda works if we just have many different lists of stuff
	int		type; // = LOADED/GENERATED
	int		h,w;
	char		**ascii,**info,**inter;
	List*		actlist;
};
struct anim{
	int		n;
	char*		chars;
};

// - others -
struct action{
	int		key;
	int		c, labellen; //c is the position of key in label
	char		*label;
	void	(*action)(Ref* ref, Map* map, List* inst);
};
struct item{
	char*	name;
};

// - lists -
struct inst{
	int		y,x;
	List		*actlist;
	Tp		*tp;// tp (or external tp list) (?)
};
struct actinst{
	int		condition;
};

struct houselist{
	House*		house;
	Houselist	*previous,*next;
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
Game* new_game(Ref* ref, Player** pl, World** world, int random);
void free_game(Game* game);
v3f menu_hue_selection(int);
v2i menu_mapsize_selection(int);
int menu_choose_difficulty(int);
void menu_set_names(Map*, Player*, int);

// = game.c =
int run_game(Game* game, Ui* ui, Ref* ref, Player* pl, World* world);
Map* movement(Player* pl, World* world, Map* map, char c);
int check_collision(Map* map, int y, int x, Tp** tp);
int check_tp(Map* map, int y, int x);
// = display.c =
void display(Ui* ui, Player* pl, Map* map);
void display_map_bg(WINDOW* gwin, Map* map, int ply, int plx);
void display_map_inst(WINDOW* gwin, Map* map, int ply, int plx);
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
void free_world(World* world);
Map* new_map(int type, int h, int w);
Map* create_map1(Ref* ref, int h, int w);
Map* create_further_map();
void free_map(Map* map);
Map* load_map(House* house,Map* oldmap);
void save_map(Map* map);

// = assets.c =
Ref* load_ref(void);
void free_ref(Ref* ref);
Asset* load_asset(char* path);
void paste_asset(Map* map, Asset* ass, int y, int x);
void free_asset(Asset* ass);
House* load_house(char* path);
void paste_house(Map* map, House* house, int y, int x);
void free_house(House* house);
Inst* inst_new(int type, int y, int x);//is it like es in rust if everything is an inst?
Inst* create_inst_from_inter(Inter* inter);
Inst* add_inst(Map* map, int y, int x, Inst* inst);
Inst* insert_inst(Inst** list, Inst* inst);
Inst* get_inst(Map* map, int x, int y);
//Inst* find_inst_inter(Ref* ref, Map* map, Inter* inter);
void destroy_inst(Inst* it, Map* map);
void free_instlist(Inst* it);
// = inter.c =
Inter** create_intertable(Action** actiontable);
Inter* load_inter(char* path, Action** actiontable);
void free_inter(Inter* inter);
void free_intertable(Inter** intertable);
Inter* duplicate_inter(Inter* inter);
Inter* create_tree(Ref* ref);
Inter* create_fruittree(Ref* ref);

// = actions.c =
Action** create_actiontable(void);
void free_actiontable(Action** actions);
//
void act(Ref* ref, Map* map, Player* pl, char c);
void act_fall_tree(Ref* ref,Map* map,List* list);
void act_pull_stump(Ref* ref,Map* map,List* list);
void act_harvest_fruits(Ref* ref,Map* map,List* list);
void act_light_fire(Ref* ref,Map* map,List* list);

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
void list_free_node(List* tf);
void list_free(List* list);
void list_insert_before(List** list, List* new);
void list_insert_after(List** list, List* new);
void list_pop(List** list, List* trm);
List* list_duplicate(List* list);
void* inst_duplicate(void* inst, int type);
void list_remove(List** list, List* trm);
void list_remove_duplicates(List* list);
//
List* list_inst_insert_new(List** list, Inter* inter, int y, int x);
void list_inst_insert(List** list, List* new);
List* list_inst_find(List* list, int y, int x);
//
void list_act_insert_new(List** list, Action* action, int condition);
List* list_act_find_key(List* list, char key);
List* list_act_find_label(List* list, char* label);
List* list_act_generate(Player* pl, List* inst);
