#include "serenity.h"

void create_map(Map* map){
map->bg =malloc(sizeof(int*)*map->h);
for (int y=0;y<map->h;y++)
	map->bg[y] =malloc(sizeof(int)*map->w);
map->clsn =malloc(sizeof(int*)*map->h);
for (int y=0;y<map->h;y++)
	map->clsn[y] =calloc(map->w,sizeof(int));
map->fg =malloc(sizeof(int*)*map->h);
for (int y=0;y<map->h;y++)
	map->fg[y] =calloc(map->w,sizeof(int));
map->inter =NULL;
map->it =malloc(sizeof(int*)*map->h);
for (int y=0;y<map->h;y++)
	map->it[y] =calloc(map->w,sizeof(int));

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
//free_inter(atree2);
	return;}

void free_map(Map* map){   //TODO free inter list
for (int i=0;i<map->h;i++){
	free(map->bg[i]);
	free(map->clsn[i]);
	free(map->fg[i]);
	free(map->it[i]);}
free(map->bg); free(map->clsn); free(map->fg); free(map->it);
free(map->name); free(map);	return;}
