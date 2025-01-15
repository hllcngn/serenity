#include "serenity.h"

Asset* load_asset(char* path){
Asset* ass =malloc(sizeof(Asset));
FILE* f =fopen(path,"r");
ass->h=0; ass->w=0;
char c; while ((c=fgetc(f))!=EOF &&c!='-'){ fseek(f,-1,SEEK_CUR); //?
	int x=0; while ((c=fgetc(f))!='\n' &&c!=EOF) x++; //readline?
	if (x>ass->w) ass->w=x; ass->h++;}	//make fread_line return len
rewind(f);
ass->map =fread_map(f,ass->h,ass->w);
fseek(f,2,SEEK_CUR);
ass->info =fread_map(f,ass->h,ass->w);
fclose(f);	return ass;}

void free_asset(Asset* ass){
for (int y=0;y<ass->h;y++) free(ass->map[y]);
free(ass->map);
for (int y=0;y<ass->h;y++) free(ass->info[y]);
free(ass->info);
free(ass);	return;}


void paste_asset(Map* map, int y, int x, Asset* ass){
for (int yy=0;yy<ass->h;yy++){
	if (y+yy<0 ||y+yy>=map->h) continue;
	for (int xx=0;xx<ass->w;xx++){
		if (x+xx<0 ||x+xx>=map->w) continue;
		switch (ass->info[yy][xx]){
		case ' ':					    break;
		case 'b': map->bg[y+yy][x+xx]   =ass->map[yy][xx];  break;
		case 'X': map->clsn[y+yy][x+xx] =ass->map[yy][xx];  break;
		case 'f': map->fg[y+yy][x+xx]   =ass->map[yy][xx];  break;
		default:					    break;}}}
return;}



Interactive** create_intertable(Action** actionstable){
Interactive** inters =malloc(sizeof(Interactive*)*NB_INTER);
//mvprintw(0,0,"loading tree2.txt\n"); getch();
inters[0] =load_inter("ass/tree2.txt",	  actionstable);
//mvprintw(0,0,"loading fruittree.txt\n"); getch();
inters[1] =load_inter("ass/fruittree.txt",actionstable);
//mvprintw(0,0,"loading stump.txt\n"); getch();
inters[2] =load_inter("ass/stump.txt",	  actionstable);
//mvprintw(0,0,"done loading intertable\n"); getch();
return inters;}

void free_intertable(Interactive** inters){
for (int i=0;i<NB_INTER;i++) free_inter(inters[i]);
free(inters);	return;}


Interactive* load_inter(char* path, Action** actiontable){
Interactive* inter =malloc(sizeof(Interactive));
FILE* f =fopen(path,"r"); while (fgetc(f)!='\n'); //fread_line?
inter->h=0; inter->w=0;		//improve like in load_asset
char c; int i =0; while ((c=fgetc(f))!='-'&&c!=EOF){ fseek(f,-1,SEEK_CUR);
	int x=0; while ((c=fgetc(f))!='\n'&&c!=EOF){ x++; i++;}
	if (c!= EOF) i++;
	if (x>inter->w) inter->w=x;
	inter->h++;} if (c!= EOF) i++;
//mvprintw(0,0,"reading maps\n"); getch();
fseek(f,-i,SEEK_CUR);inter->map   =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->info  =fread_map(f,inter->h,inter->w);
fseek(f,2,SEEK_CUR); inter->inter =fread_map(f,inter->h,inter->w);

//mvprintw(0,0,"loading actions\n"); getch();
inter->actionlist = NULL; fseek(f,2,SEEK_CUR);
while ((c=fgetc(f))!='\n'&&c!=EOF){ fseek(f,-1,SEEK_CUR);
	char* act =fread_line(f);
	//mvprintw(0,0,"act %s\n", act); getch();
	for (int i=0;i<NB_ACTION;i++)
	if (!strcmp(act,actiontable[i]->label))
		add_action(&(inter->actionlist),actiontable[i]);
	free(act);
	fgetc(f);}
fclose(f);	return inter;}

void free_inter(Interactive* inter){
for (int y=0;y<inter->h;y++) free(inter->map[y]);
for (int y=0;y<inter->h;y++) free(inter->info[y]);
for (int y=0;y<inter->h;y++) free(inter->inter[y]);
free(inter->map);free(inter->info);free(inter->inter);
free_actionlist(inter->actionlist);
free(inter);	return;}


void add_inst(Map* map, int y, int x, Interactive* inter){
Instance* inst =malloc(sizeof(Instance));
inst->y =y; inst->x =x;
inst->inter =inter;
if (!map->inst)	inst->id =1;
else {		inst->id =map->inst->id+1;
		map->inst->previous =inst;}
inst->previous =NULL; inst->next =map->inst; map->inst =inst;
for (int yy=0;yy<inst->inter->h;yy++)	//TODO edge cases
for (int xx=0;xx<inst->inter->w;xx++)  //copy arr tool funct
	map->it[y+yy][x+xx]=inst->id;
return;}

void destroy_inst(Instance* it, Map* map){
if (!it)	return;
for (int yy=0;yy<it->inter->h;yy++)	//TODO edge cases
for (int xx=0;xx<it->inter->w;xx++)  //reset copy arr tool funct
	map->it[it->y+yy][it->x+xx]=0;	//TODO superposition
if (it->previous) it->previous->next =it->next;
if (it->next) it->next->previous =it->previous;
if (map->inst==it) map->inst =it->next;
free(it);	return;}

void free_instlist(Instance* it){
if (!it)	return;
free_instlist(it->next);
free(it);	return;}
