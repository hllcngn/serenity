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


// background ASSETS
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
		default:  //map->tp[y+yy][x+xx]	=house->info[yy][xx];
			  map->bg[y+yy][x+xx]	=house->ascii[yy][xx];  break;}}}}

void free_house(House* house){
for (int y=0;y<house->h;y++) free(house->ascii[y]);
free(house->ascii);
for (int y=0;y<house->h;y++) free(house->info[y]);
free(house->info);
free(house);}


// INSTANCES
Inst* inst_new(int type, int y, int x){
Inst* inst =malloc(sizeof(Inst));
inst->y =y; inst->x =x;
inst->actlist =NULL;}
