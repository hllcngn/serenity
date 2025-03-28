#include "serenity.h"

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



void add_action(Actionlist** actionlist, Action* action, int condition){
Actionlist* al =malloc(sizeof(Actionlist));
al->action =action; al->condition =condition; al->previous =NULL;
if (*actionlist) (*actionlist)->previous =al;
al->next =*actionlist; *actionlist =al;}

void destroy_action(Actionlist* al){
if (!al)	return;
if (al->previous) al->previous->next =al->next;
if (al->next) al->next->previous =al->previous;
free(al);}

Actionlist* generate_complete_al(Player* pl, Instance* in){
Actionlist* aldisp =NULL;
for (Actionlist *al=pl->actionlist; al; al=al->next)
	if (al &&al->condition==SUPERABLE)
		add_action(&aldisp, al->action, 0);
for (Actionlist *al=in->actionlist; al; al=al->next){
	if (al->condition==SUPERABLE)
		add_action(&aldisp, al->action, 0);
	else {	Actionlist* plal =find_action(al->action->label,pl->actionlist);
		if (plal &&plal->condition)
			add_action(&aldisp, plal->action, 0);}}
for (Actionlist *al=in->inter->actionlist; al; al=al->next){
	if (al->condition==SUPERABLE)
		add_action(&aldisp, al->action, 0);
	else {	Actionlist* plal =find_action(al->action->label,pl->actionlist);
		if (plal &&plal->condition)
			add_action(&aldisp, plal->action, 0);}}
al_remove_duplicates(aldisp);
return aldisp;}

void al_remove_duplicates(Actionlist* list){
for (Actionlist* al=list; al; al=al->next)
	for (Actionlist *al2=al->next, *al3=al->next; al2;){
		al3=al2->next;
		if (al2->action==al->action) destroy_action(al2);
		al2=al3;}}

Actionlist* find_action(char* label, Actionlist* al){
for (Actionlist* all=al; all; all=all->next)
	if (!strcmp(label,all->action->label))
		return all;
return NULL;}

Actionlist* find_action_key(char key, Actionlist* al){
Actionlist* all;
for (all=al; all &&all->action->key!=key; all=all->next);
return all;}

void free_actionlist(Actionlist* al){
if (al==NULL)	return;
free_actionlist(al->next);
free(al);}
/*
void free_actionlist(Actionlist* al){
for (Actionlist *alfree=al; alfree;){
	alfree =al->next;
	free(al);
	al=alfree;}}
*/



void act(Ref* ref, Map* map, Player* pl, char c){
int inst_id =map->it[pl->y][pl->x]; //TODO this is still representing the internals of Map
Instance* inst =NULL;
if (inst_id)	inst =find_inst_id(map, inst_id);
if (!inst)	return;
Actionlist* al =generate_complete_al(pl, inst);
al =find_action_key(c, al);
al->action->action(inst, map, ref);}

void act_fall_tree(Instance* inst, Map* map, Ref* ref){
int y =inst->y, x =inst->x;
destroy_inst(inst,map);
Instance* stump_inst =create_inst_from_inter(ref->interactive[stump]);
add_inst(map, y+2, rand()%2+1, stump_inst);}

void act_pull_stump(Instance* inst, Map* map, Ref* ref){
destroy_inst(inst,map);}

void act_harvest_fruits(Instance* inst, Map* map, Ref* ref){
}

void act_light_fire(Instance* inst, Map* map, Ref* ref){
destroy_inst(inst,map);}
