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



Interactive** create_intertable(Action** actionstable){
Interactive** inters =malloc(sizeof(Interactive*)*NB_INTER);
inters[0] =load_inter("ass/tree2.txt",actionstable);
inters[1] =load_inter("ass/fruittree.txt",actionstable);
inters[2] =load_inter("ass/stump.txt",actionstable);
return inters;}

void free_intertable(Interactive** inters){
for (int i=0;i<NB_INTER;i++) free(inters[i]);
free(inters);	return;}

Interactive* load_inter(char* path, Action** actionstable){
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
char* act =fread_line(f);
for (int i=0;i<NB_ACTIONS;i++)	//TODO loop
if (!strcmp(actionstable[i]->label,act)){
	add_action(&(inter->actionlist),actionstable[i]); break;}
free(act);	fclose(f);	return inter;}

void free_inter(Interactive* inter){
for (int y=0;y<inter->h;y++){
	free(inter->map[y]);
	free(inter->info[y]);
	free(inter->inter[y]);}
free(inter->map); free(inter->info); free(inter->inter);
free_actionlist(inter->actionlist);
free(inter);	return;}

void add_inst(Map* map, int y, int x, Interactive* inter){
Instance* inst =malloc(sizeof(Instance));
if (!map->inst)	inst->id =1;
else		inst->id =map->inst->id+1;
inst->previous =NULL; inst->next =map->inst;
if (map->inst) map->inst->previous =inst;
map->inst =inst;
inst->y =y;	   inst->x =x;
inst->inter =inter;
for (int yy=0;yy<inst->inter->h;yy++)	//TODO edge cases
for (int xx=0;xx<inst->inter->w;xx++)
	map->it[y+yy][x+xx]=inst->id;
return;}

void destroy_inst(Instance* it){
if (!it) return;
if (it->previous) it->previous->next =it->next;
if (it->next) it->next->previous =it->previous;
free(it);	return;}

void free_instlist(Instance* it){
if (it==NULL)	return;
free_instlist(it->next);
free(it);	return;}
