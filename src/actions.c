#include "serenity.h"

Action** create_actiontable(void){
Action** action =malloc(sizeof(Action*)*NB_ACTION);
for (int i=0;i<NB_ACTION;i++)
	action[i] =malloc(sizeof(Action));
action[0]->label =strdup("Fall tree");
action[0]->c =0; action[0]->k ='f';
action[0]->action =&fall_tree;
action[1]->label =strdup("Pull stump");
action[1]->c =0; action[1]->k ='p';
action[1]->action =&pull_stump;
action[2]->label =strdup("Harvest fruits");
action[2]->c =0; action[2]->k ='h';
action[2]->action =&harvest_fruits;
for (int i=0;i<NB_ACTION;i++)
	action[i]->labellen =strlen(action[i]->label);
return action;}

void free_actiontable(Action** action){
for (int i=0;i<NB_ACTION;i++){
	free(action[i]->label);
	free(action[i]);}
free(action);}


void add_action(Actionlist** actionlist, Action* action){
Actionlist* al =malloc(sizeof(Actionlist));
al->action =action; al->previous =NULL;
if (*actionlist) (*actionlist)->previous =al;
al->next =*actionlist; *actionlist =al;}

void destroy_action(Actionlist* al){
if (!al)	return;
if (al->previous) al->previous->next =al->next;
if (al->next) al->next->previous =al->previous;
free(al);}

void free_actionlist(Actionlist* al){
if (al==NULL)	return;
free_actionlist(al->next);
free(al);}



void fall_tree(Instance* inst, Map* map, Info* info){
int y =inst->y, x =inst->x;
destroy_inst(inst,map);
add_inst(map,y+2,x+rand()%2+1,info->interactive[2]);
return;}

void pull_stump(Instance* inst, Map* map, Info* info){
destroy_inst(inst,map);
	return;}

void harvest_fruits(Instance* inst, Map* map, Info* info){
	return;}
