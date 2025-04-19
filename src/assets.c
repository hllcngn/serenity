#include "serenity.h"

// REFERENCE tables
Ref* load_ref(void){
Ref *ref =malloc(sizeof(Ref));
ref->action =create_actiontable();
ref->inter =create_intertable(ref->action);
ref->anim =create_animtable();
return ref;}

void free_ref(Ref* ref){
free_actiontable(ref->action);
free_intertable(ref->inter);
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
Inter** create_intertable(Action** actiontable){
Inter** inter =malloc(sizeof(Inter*)*nb_inter);
inter[tree2] =load_inter("ass/inter/tree2.txt", actiontable);
inter[fruittree] =load_inter("ass/inter/fruittree.txt", actiontable);
inter[stump] =load_inter("ass/inter/stump.txt", actiontable);
inter[umbrella] =load_inter("ass/inter/umbrella.txt", actiontable);
return inter;}

Inter* load_inter(char* path, Action** actiontable){
Inter* inter=malloc(sizeof(Inter));
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

void free_inter(Inter* inter){
for (int y=0;y<inter->h;y++){	free(inter->ascii[y]);
				free(inter->info[y]);
				free(inter->inter[y]);}
free(inter->ascii);free(inter->info);free(inter->inter);
free_actionlist(inter->actionlist);
free(inter);}

void free_intertable(Inter** inter){
for (int i=0;i<nb_inter;i++) free_inter(inter[i]);
free(inter);}


// INSTANCES
Inst* create_inst_from_inter(Inter* inter){
Inst* inst =malloc(sizeof(Inst));
inst->inter =inter; inst->actionlist =NULL;
inst->type =LOADED; inst->ascii =NULL;//
return inst;}


void insert_inst_before(Inst** list, Inst* new){
if (!(*list)){	*list =new; new->previous =new->next =NULL;	return;}
new->previous =(*list)->previous;	new->next =*list;
if (new->previous)	new->previous->next =new;
(*list)->previous =new;
if (!(new->previous))	*list =new;}

void insert_inst_after(Inst** list, Inst* new){
if (!(*list)){	*list =new; new->previous =new->next =NULL;	return;}
new->previous =*list;	new->next =(*list)->next;
if (new->next)	new->next->previous =new;
(*list)->next =new;}

Inst* insert_inst(Inst** list, Inst* inst){
Inst* in =*list;
if (!in){ *list =inst; inst->previous =inst->next =NULL;}
else {	Inst* i2; for (; in &&in->y<inst->y; in=in->next) i2=in;
	if(!in) insert_inst_after(&i2, inst);
	else	insert_inst_before(&in, inst);}}

/*
Inst* find_inst_inter(Ref* ref, Map* map, Inter* inter){
for (Inst* inst=map->inst; inst; inst=inst->next)
	if (inst->inter==inter) return inst;
return NULL;}
*/


void destroy_inst(Inst* it, Map* map){
if (!it)	return;
if (it->previous) it->previous->next =it->next;
if (it->next) it->next->previous =it->previous;
//if (map->inst==it) map->inst =it->next;
if (it->actionlist) free_actionlist(it->actionlist);
free(it);}

void free_instlist(Inst* it){
if (!it)	return;
free_instlist(it->next);
free(it);}
