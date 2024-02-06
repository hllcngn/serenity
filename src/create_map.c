#include "serenity.h"

void create_map(Map* map){
map->bg =malloc_arrayint2(map->h,map->w);
map->clsn =calloc_arrayint2(map->h,map->w);
map->fg =calloc_arrayint2(map->h,map->w);
map->it =calloc_arrayint2(map->h,map->w);
map->inst =NULL;

for (int y=0;y<map->h-1;y+=2){
	for (int x=0;x<map->w-1;x+=2){
		map->bg[y][x]='='; map->bg[y][x+1]=' ';}
	for (int x=0;x<map->w-1;x+=2){
		map->bg[y+1][x]=' '; map->bg[y+1][x+1]='=';}}

Asset* atree1 =load_asset("ass/tree1.txt");
paste_asset(map,10,10,atree1);
free_asset(atree1);

Interactive* itree2 =load_inter("ass/tree2.txt");
add_inst(map,20,20,itree2);
add_inst(map,20,30,itree2);
for (int i=0;i<20;i++)
	add_inst(map,rand()%(map->h-10)+5,rand()%(map->w-20)+10,itree2);
free_inter(itree2);
	return;}

void free_map(Map* map){
for (int y=0;y<map->h;y++){
	free(map->bg[y]);
	free(map->clsn[y]);
	free(map->fg[y]);
	free(map->it[y]);}
free(map->bg); free(map->clsn); free(map->fg); free(map->it);
free_instlist(map->inst);
free(map->name); free(map);	return;}

void free_instlist(Instance* it){
if (it==NULL)	return;
free_instlist(it->next);
for (int y=0;y<it->h;y++){
	free(it->map[y]);
	free(it->info[y]);
	free(it->inter[y]);}
free(it->map); free(it->info); free(it->inter);
free(it->label);
free(it);	return;}
