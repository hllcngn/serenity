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


void fall_tree(Instance* inst, Map* map, Info* info){
add_inst(map,inst->y+2,inst->x+rand()%2+1,info->interactives[2]);
destroy_inst(inst);
return;}

void pull_stump(Instance* inst, Map* map, Info* info){
	return;}

void harvest_fruits(Instance* inst, Map* map, Info* info){
	return;}
