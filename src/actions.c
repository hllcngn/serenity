#include "serenity.h"

Action** create_actionstable(void){
Action** actions =malloc(sizeof(Action*)*NB_ACTIONS);
for (int i=0;i<NB_ACTIONS;i++) actions[i] =malloc(sizeof(Action));
actions[0]->label =strdup("Fall tree");
actions[0]->labellen =9;
actions[0]->c =0;
actions[0]->action =&fall_tree;
actions[1]->label =strdup("Pull stump");
actions[1]->labellen =10;
actions[1]->c =0;
actions[1]->action =&pull_stump;
actions[2]->label =strdup("Harvest fruits");
actions[2]->labellen =14;
actions[2]->c =0;
actions[2]->action =&harvest_fruits;
	return actions;}

void free_actionstable(Action** actions){
for (int i=0;i<NB_ACTIONS;i++){
	free(actions[i]->label); free(actions[i]);}
free(actions);		return;}

void add_action(Actionlist** actionlist, Action* action){
Actionlist* al =malloc(sizeof(Actionlist));
al->action =action; al->previous =NULL;
if (!*actionlist){ al->next =NULL; *actionlist =al;}
else { al->next =(*actionlist)->next; (*actionlist)->previous =al;}
return;}

void destroy_action(Actionlist* al){
if (!al) return;
if (al->previous) al->previous->next =al->next;
if (al->next) al->next->previous =al->previous;
free(al);	return;}

void free_actionlist(Actionlist* al){
if (al==NULL)	return;
free_actionlist(al->next);
free(al);	return;}


void fall_tree(Instance* inst, Map* map, Info* info){
add_inst(map,inst->y+2,inst->x+rand()%2+1,info->interactives[2]);
destroy_inst(inst);
return;}

void pull_stump(Instance* inst, Map* map, Info* info){
	return;}

void harvest_fruits(Instance* inst, Map* map, Info* info){
	return;}
