#include "serenity.h"

Asset* load_asset(char* path){
Asset* ass =malloc(sizeof(Asset));
FILE* f =fopen(path,"r");
fsize_map(f,&(ass->h),&(ass->w));
rewind(f);	     ass->map =fread_map(f,ass->h,ass->w);
fseek(f,2,SEEK_CUR); ass->info =fread_map(f,ass->h,ass->w);
fclose(f);	return ass;}

void paste_asset(Map* map, int y, int x, Asset* ass){
for (int yy=0;yy<ass->h;yy++){
	if (y+yy<0 ||y+yy>=map->h) continue;
	for (int xx=0;xx<ass->w;xx++){
		if (x+xx<0 ||x+xx>=map->w) continue;
		switch (ass->info[yy][xx]){
		case ' ':					    break;
		case 'b': map->bg[y+yy][x+xx]   =ass->map[yy][xx];  break;
		case 'f': map->fg[y+yy][x+xx]   =ass->map[yy][xx];  break;
		case 'X': map->clsn[y+yy][x+xx] =ass->map[yy][xx];  break;
		default:					    break;}}}}

void free_asset(Asset* ass){
for (int y=0;y<ass->h;y++) free(ass->map[y]);
free(ass->map);
for (int y=0;y<ass->h;y++) free(ass->info[y]);
free(ass->info);
free(ass);}

HAsset* load_hasset(char* path){
HAsset* hass =malloc(sizeof(HAsset));
FILE* f =fopen(path,"r");
fsize_map(f,&(hass->h),&(hass->w));
rewind(f);	     hass->map =fread_map(f,hass->h,hass->w);
fseek(f,2,SEEK_CUR); hass->info =fread_map(f,hass->h,hass->w);
fseek(f,2,SEEK_CUR); hass->path = fread_line(f);
fclose(f);	return hass;}

void paste_hasset(Map* map, int y, int x, HAsset* hass){
for (int yy=0;yy<hass->h;yy++){
	if (y+yy<0 ||y+yy>=map->h) continue;
	for (int xx=0;xx<hass->w;xx++){
		if (x+xx<0 ||x+xx>=map->w) continue;
		switch (hass->info[yy][xx]){
		case ' ':					    break;
		case 'b': map->bg[y+yy][x+xx]   =hass->map[yy][xx];  break;
		case 'f': map->fg[y+yy][x+xx]   =hass->map[yy][xx];  break;
		case 'X': map->clsn[y+yy][x+xx] =hass->map[yy][xx];  break;
		default:  map->tp[y+yy][x+xx]	=hass->info[yy][xx]; break;}}}}

void free_hasset(HAsset* hass){
for (int y=0;y<hass->h;y++) free(hass->map[y]);
free(hass->map);
for (int y=0;y<hass->h;y++) free(hass->info[y]);
free(hass->info);
free(hass->path);
free(hass);}



Interactive** create_intertable(Action** actiontable){
Interactive** inter =malloc(sizeof(Interactive*)*NB_INTER);
inter[0] =load_inter("ass/tree2.txt",	 actiontable);
inter[1] =load_inter("ass/fruittree.txt",actiontable);
inter[2] =load_inter("ass/stump.txt",	 actiontable);
return inter;}

Interactive* load_inter(char* path, Action** actiontable){
Interactive* inter=malloc(sizeof(Interactive));
FILE* f=fopen(path,"r");
fsize_map(f,&(inter->h),&(inter->w));
rewind(f);	     inter->map   =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->info  =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->inter =fread_map(f,inter->h,inter->w);
inter->actionlist = NULL; fseek(f,2,SEEK_CUR);
char c; while ((c=getc(f))!='\n'&&c!=EOF){ fseek(f,-1,SEEK_CUR);
	char* act =fread_line(f);
	for (int i=0;i<NB_ACTION;i++)
	if (!strcmp(act,actiontable[i]->label))
		add_action(&(inter->actionlist),actiontable[i]);
	free(act);}
fclose(f);	return inter;}

void free_inter(Interactive* inter){
for (int y=0;y<inter->h;y++){	free(inter->map[y]);
				free(inter->info[y]);
				free(inter->inter[y]);}
free(inter->map);free(inter->info);free(inter->inter);
free_actionlist(inter->actionlist);
free(inter);}

void free_intertable(Interactive** inter){
for (int i=0;i<NB_INTER;i++) free_inter(inter[i]);
free(inter);}



void add_inst(Map* map, int y, int x, Interactive* inter){
Instance* inst =malloc(sizeof(Instance));
inst->y =y; inst->x =x; inst->inter =inter;
if (!map->inst)	inst->id =1;
else {		inst->id =map->inst->id+1;
		map->inst->previous =inst;}
inst->previous =NULL; inst->next =map->inst; map->inst =inst;
for (int yy=0;yy<inst->inter->h;yy++)	//TODO edge cases
for (int xx=0;xx<inst->inter->w;xx++)  //copy arr tool funct
	map->it[y+yy][x+xx]=inst->id;}

Instance* check_inst(vect2i pos, Map* map){
if (map->it[pos.y][pos.x]){
Instance* it; for (it=map->inst;
it &&it->id!=map->it[pos.y][pos.x];
it=it->next);	return it;}	return NULL;}

void destroy_inst(Instance* it, Map* map){
if (!it)	return;
for (int yy=0;yy<it->inter->h;yy++)	//TODO edge cases
for (int xx=0;xx<it->inter->w;xx++)  //reset copy arr tool funct
	map->it[it->y+yy][it->x+xx]=0;	//TODO superposition
if (it->previous) it->previous->next =it->next;
if (it->next) it->next->previous =it->previous;
if (map->inst==it) map->inst =it->next;
free(it);}

void free_instlist(Instance* it){
if (!it)	return;
free_instlist(it->next);
free(it);}
