#include "serenity.h"

Asset* load_asset(char* path){
Asset* ass =malloc(sizeof(Asset));
FILE* f =fopen(path,"r");
ass->h=0; ass->w=0;
char c; while ((c=fgetc(f))!=EOF){ fseek(f,-1,SEEK_CUR);
	int x=0; while((c=fgetc(f))!='\n'&&c!=EOF) x++;
	if (x>ass->w) ass->w=x;
	ass->h++;}
fseek(f,0,SEEK_SET);
ass->map =malloc(sizeof(int*)*ass->h);
for (int y=0;y<ass->h;y++)
	ass->map[y] =malloc(sizeof(int)*ass->w);
for (int y=0;y<ass->h;y++){
	int x; for (x=0;x<ass->w;x++){
		ass->map[y][x]=fgetc(f);
		if (ass->map[y][x]=='\n')
			break;}
	if (ass->map[y][x]=='\n')
		for (;x<ass->w;x++)
			ass->map[y][x]=' ';}
fclose(f);	return ass;}

void free_asset(Asset* ass){
	return;}
