#include "serenity.h"
//lists of actions are useful when i want to add a particular action
//to a specific asset, upon a special condition
//otherwise i would need to have conditions per inter
//in addition to conditions on the player

Action** create_actiontable(){
Action** action =malloc(sizeof(Action*)*nb_action);
for (int i=0;i<nb_action;i++)
	action[i] =malloc(sizeof(Action));
action[fall_tree]->id=fall_tree;
action[fall_tree]->label =strdup("Fall tree");
action[fall_tree]->c =0;
action[fall_tree]->key ='f';
action[fall_tree]->action =&act_fall_tree;
action[pull_stump]->id=pull_stump;
action[pull_stump]->label =strdup("Pull stump");
action[pull_stump]->c =0;
action[pull_stump]->key ='p';
action[pull_stump]->action =&act_pull_stump;
action[harvest_fruits]->id=harvest_fruits;
action[harvest_fruits]->label =strdup("Harvest fruits");
action[harvest_fruits]->c =0;
action[harvest_fruits]->key ='h';
action[harvest_fruits]->action =&act_harvest_fruits;
action[light_fire]->id=light_fire;
action[light_fire]->label =strdup("Light on fire");
action[light_fire]->c =0;
action[light_fire]->key ='l';
action[light_fire]->action =&act_light_fire;
for (int i=0;i<nb_action;i++)
	action[i]->labellen =strlen(action[i]->label);
return action;}

void free_actiontable(Action** action){
for (int i=0;i<nb_action;i++){
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


Action* find_action_list(char* label, Actionlist* al){
for (Actionlist* all=al; all; all=all->next)
	if (!strcmp(label,all->action->label))
		return all->action;
return NULL;}

Action* find_action_table(char* label, Action** at){
for (int i=0; i<nb_action; i++)
	if (!strcmp(label,at[i]->label))
		return at[i];
return NULL;}


void free_actionlist(Actionlist* al){
if (al==NULL)	return;
free_actionlist(al->next);
free(al);}



void act_fall_tree(Instance* inst, Map* map, Ref* ref){
int y =inst->y, x =inst->x;
destroy_inst(inst,map);
Interactive* stump =find_inter(ref, "stump");
if (stump) add_inst(map,y+2,x+rand()%2+1,stump);}

void act_pull_stump(Instance* inst, Map* map, Ref* ref){
destroy_inst(inst,map);}

void act_harvest_fruits(Instance* inst, Map* map, Ref* ref){
}

void act_light_fire(Instance* inst, Map* map, Ref* ref){
destroy_inst(inst,map);}
