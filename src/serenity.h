#pragma once

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

typedef struct{	float	i,j,k;	} vect3f;

typedef struct{ int	h,w;
		char*	name;	} Map;

typedef struct{	char*	name;	} Player;
