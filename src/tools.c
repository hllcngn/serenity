#include "serenity.h"

int** malloc_arrayint2(int h,int w){
int** arr =malloc(sizeof(int*)*h);
for (int i=0;i<h;i++) arr[i] =malloc(sizeof(int)*w);
	return arr;}

int** calloc_arrayint2(int h,int w){
int** arr =malloc(sizeof(int*)*h);
for (int i=0;i<h;i++) arr[i] =calloc(w, sizeof(int));
	return arr;}
