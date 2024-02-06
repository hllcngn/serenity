#include "serenity.h"

Asset* load_asset(char* path){
Asset* ass =malloc(sizeof(Asset));
FILE* f =fopen(path,"r");
ass->h=0; ass->w=0;
char c; while ((c=fgetc(f))!=EOF &&c!='-'){ fseek(f,-1,SEEK_CUR);
	int x=0; while ((c=fgetc(f))!='\n' &&c!=EOF) x++;
	if (x>ass->w) ass->w=x;
	ass->h++;}
rewind(f);
ass->map =fread_map(f,ass->h,ass->w);
fseek(f,2,SEEK_CUR);
ass->info =fread_map(f,ass->h,ass->w);
fclose(f);	return ass;}

void free_asset(Asset* ass){
for (int y=0;y<ass->h;y++){
	free(ass->map[y]);
	free(ass->info[y]);}
free(ass->map); free(ass->info);
free(ass);	return;}


Interactive* load_inter(char* path){
Interactive* inter =malloc(sizeof(Interactive));
FILE* f =fopen(path,"r"); while (fgetc(f)!='\n');
inter->h=0; inter->w=0;
char c; while ((c=fgetc(f))!=EOF &&c!='-'){ fseek(f,-1,SEEK_CUR);
	int x=0; while ((c=fgetc(f))!='\n' &&c!=EOF) x++;
	if (x>inter->w) inter->w=x;
	inter->h++;}
rewind(f); while (fgetc(f)!='\n');
inter->map =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR);
inter->info =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR);
inter->inter =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR);
int len=0; while ((c=fgetc(f))!='\n' &&c!=EOF) len++;
inter->label =malloc(len+1);
if (c==EOF)	fseek(f,-(len),SEEK_CUR);
else		fseek(f,-(len+1),SEEK_CUR);
for (int i=0;i<len;i++) inter->label[i] =fgetc(f);
inter->label[len] ='\0';
fclose(f);	return inter;}

void free_inter(Interactive* inter){
for (int y=0;y<inter->h;y++){
	free(inter->map[y]);
	free(inter->info[y]);
	free(inter->inter[y]);}
free(inter->map); free(inter->info); free(inter->inter);
free(inter->label); free(inter);	return;}


int** fread_map(FILE* f, int h, int w){
int** map =malloc(sizeof(int*)*h);
for (int y=0;y<h;y++)
	map[y] =malloc(sizeof(int)*w);
for (int y=0;y<h;y++){ for (int x=0;x<w;x++){
	map[y][x]=fgetc(f);
	if (map[y][x]=='\n'){
		for (x;x<w;x++) map[y][x]=' '; break;}
	else if (x==w-1) fgetc(f);}}
return map;}



void paste_asset(Map* map, int y, int x, Asset* ass){
for (int yy=0;yy<ass->h;yy++)	//TODO edge cases
for (int xx=0;xx<ass->w;xx++)
	switch (ass->info[yy][xx]){
	case ' ':					    break;
	case 'b': map->bg[y+yy][x+xx]   =ass->map[yy][xx];  break;
	case 'X': map->clsn[y+yy][x+xx] =ass->map[yy][xx];  break;
	case 'f': map->fg[y+yy][x+xx]   =ass->map[yy][xx];  break;
	default:					    break;}
return;}


void add_inst(Map* map, int y, int x, Interactive* inter){
Instance* inst =malloc(sizeof(Instance));
if (!map->inst)	inst->id =1;
else		inst->id =map->inst->id+1;
inst->previous =NULL; inst->next =map->inst;
if (map->inst) map->inst->previous =inst; map->inst =inst;
inst->y =y;	   inst->x =x;
inst->h =inter->h; inst->w =inter->w;
inst->map   =duplicate_arrayint2(inter->map,inst->h,inst->w);
inst->info  =duplicate_arrayint2(inter->info,inst->h,inst->w);
inst->inter =duplicate_arrayint2(inter->inter,inst->h,inst->w);
inst->label =strdup(inter->label);
for (int yy=0;yy<inst->h;yy++)	//TODO edge cases
for (int xx=0;xx<inst->w;xx++)
	map->it[y+yy][x+xx]=inst->id;	return;}
