#pragma once

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

typedef struct{	float	i,j,k;	} vect3f;

typedef struct{ int	h,w;
		int**	map;	} Asset;

typedef struct{ int	h,w;
		char*	name;
		int**	map;	} Map;

typedef struct{	char*	name;	} Player;

void create_map(Map* map);
void free_map(Map* map);
Asset* load_asset(char* path);
void free_asset(Asset* ass);
void paste_asset(Map* map, int y, int x, Asset* ass);
int game(vect3f hue, Map* map, Player* pl);
