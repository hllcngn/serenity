#include "serenity.h"

void create_map(Map* map){
map->bg =malloc_arrayint2(map->h,map->w);
map->clsn =calloc_arrayint2(map->h,map->w);
map->fg =calloc_arrayint2(map->h,map->w);
map->it =calloc_arrayint2(map->h,map->w);
map->inter =NULL;

for (int y=0;y<map->h-1;y+=2){
	for (int x=0;x<map->w-1;x+=2){
		map->bg[y][x]='='; map->bg[y][x+1]=' ';}
	for (int x=0;x<map->w-1;x+=2){
		map->bg[y+1][x]=' '; map->bg[y+1][x+1]='=';}}

Asset* atree1 =load_asset("ass/tree1.txt");
paste_asset(map,10,10,atree1);
free_asset(atree1);

Interactive* itree2 =load_inter("ass/tree2.txt");
add_inter(map,20,20,itree2);
	return;}

void free_map(Map* map){   //TODO free inter list
for (int y=0;y<map->h;y++){
	free(map->bg[y]);
	free(map->clsn[y]);
	free(map->fg[y]);
	free(map->it[y]);}
free(map->bg); free(map->clsn); free(map->fg); free(map->it);
free_interlist(map->inter);
free(map->name); free(map);	return;}

void free_interlist(Interactive* inter){
if (inter==NULL)	return;
if (inter->map){
for (int y=0;y<inter->h;y++){
	free(inter->map[y]);
	free(inter->info[y]);
	free(inter->inter[y]);}
free(inter->map); free(inter->info); free(inter->inter);
inter->map =NULL; inter->info =NULL; inter->inter =NULL;
free(inter->label);}
free_interlist(inter->next);
free(inter);	return;}
