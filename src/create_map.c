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

for (int y=0;y<map->h-1;y+=2){
	for (int x=0;x<map->w-1;x+=2){
		map->bg[y][x]='='; map->bg[y][x+1]=' ';}
	for (int x=0;x<map->w-1;x+=2){
		map->bg[y+1][x]=' '; map->bg[y+1][x+1]='=';}}

Asset* atree1 =load_asset("ass/tree1.txt");
Asset* atree2 =load_asset("ass/tree2.txt");
paste_asset(map,10,10,atree1);
paste_asset(map,20,20,atree2);
free_asset(atree1);
free_asset(atree2);
	return;}

void free_map(Map* map){
for (int i=0;i<map->h;i++)
	free(map->bg[i]);
free(map->name); free(map);	return;}
