#include "serenity.h"

// REFERENCE tables
Ref* load_ref(void){
Ref *ref =malloc(sizeof(Ref));
ref->action =create_actiontable();
ref->interactive =create_intertable(ref->action);
ref->anim =create_animtable();
return ref;}

void free_ref(Ref* ref){
free_actiontable(ref->action);
free_intertable(ref->interactive);
free_animtable(ref->anim);
free(ref);}


// simple ASSETS
Asset* load_asset(char* path){
Asset* ass =malloc(sizeof(Asset));
FILE* f =fopen(path,"r");
int h,w; fsize_map(f,&h,&w);
rewind(f);	     ass->ascii =fread_map(f,h,w);
fseek(f,2,SEEK_CUR); ass->info =fread_map(f,h,w);
ass->h =h; ass->w =w;
fclose(f);	return ass;}

void paste_asset(Map* map, Asset* ass, int y, int x){
int yy =y<0? -y: 0;
for (; yy<ass->h &&y+yy<map->h; yy++){
	int xx =x<0? -x: 0;
	for (; xx<ass->w &&x+xx<map->w; xx++){
		switch (ass->info[yy][xx]){
		case 'b': map->bg[y+yy][x+xx] =ass->ascii[yy][xx];   break;
		case 'f': map->fg[y+yy][x+xx] =ass->ascii[yy][xx];   break;
		case 'X': map->clsn[y+yy][x+xx] =ass->ascii[yy][xx]; break;}}}}

void free_asset(Asset* ass){
for (int y=0;y<ass->h;y++) free(ass->ascii[y]);
free(ass->ascii);
for (int y=0;y<ass->h;y++) free(ass->info[y]);
free(ass->info);
free(ass);}


// HOUSES (assets w/ tp)
House* load_house(char* path){
House* house =malloc(sizeof(House));
FILE* f =fopen(path,"r");
fsize_map(f,&(house->h),&(house->w));
rewind(f);	     house->ascii =fread_map(f,house->h,house->w);
fseek(f,2,SEEK_CUR); house->info =fread_map(f,house->h,house->w);
fclose(f);	return house;}

void paste_house(Map* map, House* house, int y, int x){
int yy =y<0? -y: 0;
for (; yy<house->h &&y+yy<map->h; yy++){
	int xx =x<0? -x: 0;
	for (; xx<house->w &&x+xx<map->w; xx++){
		switch (house->info[yy][xx]){
		case ' ':					      break;
		case 'b': map->bg[y+yy][x+xx]   =house->ascii[yy][xx];  break;
		case 'f': map->fg[y+yy][x+xx]   =house->ascii[yy][xx];  break;
		case 'X': map->clsn[y+yy][x+xx] =house->ascii[yy][xx];  break;
		default:  map->tp[y+yy][x+xx]	=house->info[yy][xx];
			  map->bg[y+yy][x+xx]	=house->ascii[yy][xx];  break;}}}}

void free_house(House* house){
for (int y=0;y<house->h;y++) free(house->ascii[y]);
free(house->ascii);
for (int y=0;y<house->h;y++) free(house->info[y]);
free(house->info);
free(house);}


// INTERACTIVES (instance models)
Interactive** create_intertable(Action** actiontable){
Interactive** inter =malloc(sizeof(Interactive*)*nb_inter);
inter[tree2] =load_inter("ass/inter/tree2.txt", actiontable);
inter[fruittree] =load_inter("ass/inter/fruittree.txt", actiontable);
inter[stump] =load_inter("ass/inter/stump.txt", actiontable);
inter[umbrella] =load_inter("ass/inter/umbrella.txt", actiontable);
return inter;}

Interactive* load_inter(char* path, Action** actiontable){
Interactive* inter=malloc(sizeof(Interactive));
FILE* f=fopen(path,"r");
fsize_map(f,&(inter->h),&(inter->w));
rewind(f);	     inter->ascii =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->info  =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->inter =fread_map(f,inter->h,inter->w);
inter->actionlist = NULL; fseek(f,2,SEEK_CUR);
char c; while ((c=getc(f))!='\n'&&c!=EOF){ fseek(f,-1,SEEK_CUR);
	char* act =fread_line(f);	//TODO read actions in reverse order
	for (int i=0;i<nb_action;i++)
	if (!strcmp(act,actiontable[i]->label))
		add_action(&(inter->actionlist),actiontable[i], ABLE);
	free(act);}
fclose(f);	return inter;}

void free_inter(Interactive* inter){
for (int y=0;y<inter->h;y++){	free(inter->ascii[y]);
				free(inter->info[y]);
				free(inter->inter[y]);}
free(inter->ascii);free(inter->info);free(inter->inter);
free_actionlist(inter->actionlist);
free(inter);}

void free_intertable(Interactive** inter){
for (int i=0;i<nb_inter;i++) free_inter(inter[i]);
free(inter);}


// INSTANCES
Instance* create_inst_from_inter(Interactive* inter){
Instance* inst =malloc(sizeof(Instance));
inst->inter =inter; inst->actionlist =NULL;
inst->type =LOADED; inst->ascii =NULL;
return inst;}

Instance* add_inst(Map* map, int y, int x, Instance* inst){
inst->y =y; inst->x =x;
insert_inst(&(map->inst), inst);
return inst;}


void insert_inst_before(Instance** list, Instance* new){
if (!(*list)){	*list =new; new->previous =new->next =NULL;	return;}
new->previous =(*list)->previous;	new->next =*list;
if (new->previous)	new->previous->next =new;
(*list)->previous =new;
if (!(new->previous))	*list =new;}

void insert_inst_after(Instance** list, Instance* new){
if (!(*list)){	*list =new; new->previous =new->next =NULL;	return;}
new->previous =*list;	new->next =(*list)->next;
if (new->next)	new->next->previous =new;
(*list)->next =new;}

Instance* insert_inst(Instance** list, Instance* inst){
/*Instance* in=*list; if (!in){
	*list =inst;
	inst->previous =inst->next =NULL;}
else {	Instance* i2; for (; in &&in->y<inst->y; in=in->next) i2=in;
	if(!in) { inst->previous =i2;
		inst->next =NULL;
		i2->next =inst;}
	else { inst->previous =in->previous;
		inst->next =in;
		if (in->previous)
			in->previous->next =inst;
		else *list =inst;
		in->previous =inst;}}
*/
	/**/
Instance* in =*list;
if (!in){ *list =inst; inst->previous =inst->next =NULL;}
else {	Instance* i2; for (; in &&in->y<inst->y; in=in->next) i2=in;
	if(!in) insert_inst_after(&i2, inst);
	else	insert_inst_before(&in, inst);}}

	/**/
/*
if (!(*list)){	*list =inst; inst->previous =inst->next =NULL; return inst;}
if ((*list)->y > inst->y){	insert_inst_before(list, inst); return inst;} //< BUG
if (!((*list)->next)){	insert_inst_after(list, inst); return inst;}
return insert_inst(&((*list)->next), inst);}
*/



Instance* get_inst(Map* map, int y, int x){
for (Instance* i=map->inst; i; i=i->next)
	if (y>=i->y &&y<i->y+i->inter->h &&x>=i->x &&x<i->x+i->inter->w)
		return i;
return NULL;}
/*
Instance* find_inst_id(Map* map, int id){
for (Instance* it=map->inst; it; it=it->next)
	if (it->id==id) return it;
return NULL;}
if (!list) return NULL;
if (list->id==id) return list;
else return func(list, id);

Instance* find_inst_inter(Ref* ref, Map* map, Interactive* inter){
for (Instance* inst=map->inst; inst; inst=inst->next)
	if (inst->inter==inter) return inst;
return NULL;}
*/

void destroy_inst(Instance* it, Map* map){
if (!it)	return;
if (it->previous) it->previous->next =it->next;
if (it->next) it->next->previous =it->previous;
if (map->inst==it) map->inst =it->next;
if (it->actionlist) free_actionlist(it->actionlist);
free(it);}

void free_instlist(Instance* it){
if (!it)	return;
free_instlist(it->next);
free(it);}
