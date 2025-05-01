#include "serenity.h"

Anim** create_animtable(void){
Anim **at= malloc(sizeof(Anim*)*nb_anim);
for (int i=0; i<nb_anim; i++)
	at[i]= malloc(sizeof(Anim));
at[fire]->n= 5;
at[fire]->chars= strdup("(),`,;");
return at;}

void free_animtable(Anim **at){
for (int i=0; i<nb_anim; i++){
	free(at[i]->chars);
	free(at[i]);}
free(at);}
