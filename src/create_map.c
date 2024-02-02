#include "serenity.h"

void create_map(Map* map){
map->map =malloc(sizeof(int*)*map->h);
for (int y=0;y<map->h;y++)
	map->map[y] =malloc(sizeof(int)*map->w);
for (int y=0;y<map->h-1;y+=2){
	for (int x=0;x<map->w-1;x+=2){
		map->map[y][x]='='; map->map[y][x+1]=' ';}
	for (int x=0;x<map->w-1;x+=2){
		map->map[y+1][x]=' '; map->map[y+1][x+1]='=';}}
Asset* atree =load_asset("ass/tree.txt");
paste_asset(map,10,10,atree);
	return;}

void free_map(Map* map){
for (int i=0;i<map->h;i++)
	free(map->map[i]);
free(map->name);
free(map);	return;}


void paste_asset(Map* map, int y, int x, Asset* ass){
for (int yy=0;yy<ass->h;yy++)	//TODO edge cases
	for (int xx=0;xx<ass->w;xx++)
		if (ass->map[yy][xx]!=' ')
			map->map[y+yy][x+xx] =ass->map[yy][xx];
return;}
