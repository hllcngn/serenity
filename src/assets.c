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
ass->map =malloc(sizeof(int*)*ass->h);
for (int y=0;y<ass->h;y++)
	ass->map[y] =malloc(sizeof(int)*ass->w);
for (int y=0;y<ass->h;y++){
	for (int x=0;x<ass->w;x++){
		ass->map[y][x]=fgetc(f);
		if (ass->map[y][x]=='\n'){
			for (;x<ass->w;x++)
				ass->map[y][x]=' ';
			break;}
		else if (x==ass->w-1) fgetc(f);}}

fseek(f,2,SEEK_CUR);
ass->info =malloc(sizeof(int*)*ass->h);
for (int y=0;y<ass->h;y++)
	ass->info[y] =malloc(sizeof(int)*ass->w);
for (int y=0;y<ass->h;y++){
	for (int x=0;x<ass->w;x++){
		ass->info[y][x]=fgetc(f);
		if (ass->info[y][x]=='\n'){
			for (;x<ass->w;x++)
				ass->info[y][x]=' ';
			break;}
		else if (x==ass->w-1) fgetc(f);}}
fclose(f);	return ass;}

void free_asset(Asset* ass){
for (int y=0;y<ass->h;y++)
	free(ass->map[y]);
free(ass->map); free(ass); return;}


void paste_asset(Map* map, int y, int x, Asset* ass){
for (int yy=0;yy<ass->h;yy++)	//TODO edge cases
	for (int xx=0;xx<ass->w;xx++)
		switch (ass->info[yy][xx]){
		case ' ':	break;
		case 'b': map->bg[y+yy][x+xx] =ass->map[yy][xx];   break;
		case 'X': map->clsn[y+yy][x+xx] =ass->map[yy][xx]; break;
		case 'f': map->fg[y+yy][x+xx] =ass->map[yy][xx];   break;
		default:	break;}	return;}
