#include "serenity.h"

Asset* load_asset(char* path){
Asset* ass =malloc(sizeof(Asset));
FILE* f =fopen(path,"r");
int h,w; fsize_map(f,&h,&w);
rewind(f);	     ass->map =fread_map(f,h,w);
fseek(f,2,SEEK_CUR); ass->info =fread_map(f,h,w);
ass->h =h; ass->w =w;
fclose(f);	return ass;}

void paste_asset(Map* map, Asset* ass, int y, int x){
int yy =y<0? -y: 0;
for (; yy<ass->h &&y+yy<map->h; yy++){
	int xx =x<0? -x: 0;
	for (; xx<ass->w &&x+xx<map->w; xx++){
		switch (ass->info[yy][xx]){
		case 'b': map->bg[y+yy][x+xx] =ass->map[yy][xx];   break;
		case 'f': map->fg[y+yy][x+xx] =ass->map[yy][xx];   break;
		case 'X': map->clsn[y+yy][x+xx] =ass->map[yy][xx]; break;}}}}

void free_asset(Asset* ass){
for (int y=0;y<ass->h;y++) free(ass->map[y]);
free(ass->map);
for (int y=0;y<ass->h;y++) free(ass->info[y]);
free(ass->info);
free(ass);}


House* load_house(char* path){
House* house =malloc(sizeof(House));
FILE* f =fopen(path,"r");
fsize_map(f,&(house->h),&(house->w));
rewind(f);	     house->map =fread_map(f,house->h,house->w);
fseek(f,2,SEEK_CUR); house->info =fread_map(f,house->h,house->w);
fseek(f,2,SEEK_CUR); house->path = fread_line(f);
fclose(f);	return house;}

void paste_house(Map* map, House* house, int y, int x){
int yy =y<0? -y: 0;
for (; yy<house->h &&y+yy<map->h; yy++){
	int xx =x<0? -x: 0;
	for (; xx<house->w &&x+xx<map->w; xx++){
		switch (house->info[yy][xx]){
		case ' ':					      break;
		case 'b': map->bg[y+yy][x+xx]   =house->map[yy][xx];  break;
		case 'f': map->fg[y+yy][x+xx]   =house->map[yy][xx];  break;
		case 'X': map->clsn[y+yy][x+xx] =house->map[yy][xx];  break;
		default:  map->tp[y+yy][x+xx]	=house->info[yy][xx];
			  map->bg[y+yy][x+xx]	=house->map[yy][xx];  break;}}}}

void free_house(House* house){
for (int y=0;y<house->h;y++) free(house->map[y]);
free(house->map);
for (int y=0;y<house->h;y++) free(house->info[y]);
free(house->info);
free(house->path);
free(house);}



Interactive** create_intertable(Action** actiontable){
Interactive** inter =malloc(sizeof(Interactive*)*NB_INTER);
inter[0] =load_inter("ass/tree2.txt", "tree2", actiontable);
inter[1] =load_inter("ass/fruittree.txt", "fruittree", actiontable);
inter[2] =load_inter("ass/stump.txt", "stump", actiontable);
return inter;}

Interactive* load_inter(char* path, char* name, Action** actiontable){
Interactive* inter=malloc(sizeof(Interactive));
inter->name =strdup(name);
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

Interactive* find_inter(Ref* ref, char* name){
for (int i=0; i<NB_INTER; i++)
	if (!strcmp(name, ref->interactive[i]->name))
		return ref->interactive[i];
return NULL;}

void free_inter(Interactive* inter){
for (int y=0;y<inter->h;y++){	free(inter->map[y]);
				free(inter->info[y]);
				free(inter->inter[y]);}
free(inter->map);free(inter->info);free(inter->inter);
free_actionlist(inter->actionlist);
free(inter->name); free(inter);}

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

Instance* check_inst(v2i pos, Map* map){
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
