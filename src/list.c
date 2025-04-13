#include "serenity.h"

List* list_new(int type, void* item, void* hints){
List* new =malloc(sizeof(List));
new->type =type;
new->item =item;
new->hints =hints;
new->prev =new->next =NULL;}

void insert_before(List** list, List* new){
if (!(*list)){	*list =new; new->prev =new->next =NULL;	return;}
new->prev =(*list)->prev;	new->next =*list;
if (new->prev)	  new->prev->next =new;
(*list)->prev =new;
if (!(new->prev)) *list =new;}

void insert_after(List** list, List* new){
if (!(*list)){	*list =new; new->prev =new->next =NULL;	return;}
new->prev =*list;	new->next =(*list)->next;
if (new->next)	new->next->prev =new;
(*list)->next =new;}

void node_remove(List** list, List* trm){
if (!trm)	return;
if (*list ==trm)	*list =(*list)->next;
if (trm->prev)	trm->prev->next =trm->next;
if (trm->next)	trm->next->prev =trm->prev;
node_free(trm);}

void node_free(List* tf){
// for instances check if the item (inter) is loaded or generated
// free generated items
free(tf->hints);
free(tf);}

void list_free(List* list){
if (!list)	return;
list_free(list->next);
free(list);}

	//i can't actually make such a generic function work for all types of items
	//except if i added a generic part to the list struct with things like position
	//but then again maybe there is no need
// SPECIFIC TO INSTANCES
void list_inst_insert(List** list, List* inst){
List* in =*list;
if (!in){ *list =inst; inst->prev =inst->next =NULL;}
else {	List* i2; for (;in &&((Instance*)(in->hints))->y<((Instance*)(inst->hints))->y;
			in=in->next) i2=in;
	if(!in) insert_after(&i2, inst);
	else	insert_before(&in, inst);}}

//TODO change type names Instance > Inst and Interactive > Inter
List* list_inst_get(List** list, int y, int x){
for (List* in=*list; in; in=in->next)
	/*
	insty =((Instance*)(in->hints))->y;
	instx =((Instance*)(in->hints))->x;
	interh =((Interactive*)(in->item))->h;
	interw =((Interactive*)(in->item))->w;
	*/
	if (	y>=((Instance*)(in->hints))->y
		&&y<((Instance*)(in->hints))->y+((Interactive*)(in->item))->h
		&&x>=((Instance*)(in->hints))->x
		&&x<((Instance*)(in->hints))->x+((Interactive*)(in->item))->w)
			return in;
return NULL;}
/*
Instance* find_inst_inter(Ref* ref, Map* map, Interactive* inter){
for (Instance* inst=map->inst; inst; inst=inst->next)
	if (inst->inter==inter) return inst;
return NULL;}
*/
