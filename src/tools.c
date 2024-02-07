#include "serenity.h"

int** malloc_arrayint2(int h,int w){
int** arr =malloc(sizeof(int*)*h);
for (int i=0;i<h;i++) arr[i] =malloc(sizeof(int)*w);
	return arr;}

int** calloc_arrayint2(int h,int w){
int** arr =malloc(sizeof(int*)*h);
for (int i=0;i<h;i++) arr[i] =calloc(w, sizeof(int));
	return arr;}

int** duplicate_arrayint2(int** arr,int h,int w){
int** new =malloc_arrayint2(h,w);
for (int y=0;y<h;y++) for (int x=0;x<w;x++)
	new[y][x] =arr[y][x];	return new;}

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

void clear_screen(int cp){
move(0,0); attron(COLOR_PAIR(cp));
for (int y=0;y<LINES;y++)
	for (int x=0;x<COLS;x++)
		addch(' ');	return;}
