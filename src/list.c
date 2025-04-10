#include "serenity.h"

List* list_new(int type, void* item, void* hints){
List* new =malloc(sizeof(List));
new->type =type;
new->item =item;
new->hints =hints;
new->prev =new->next =NULL;}

void insert_before(List** list, List* new){
if (!(*list)){	*list =new;	return;}
new->prev =(*list)->prev;
if ((*list)->prev)	(*list)->prev->next =new;
(*list)->prev =new;
new->next =(*list);}

void insert_after(List** list, List* new){
if (!(*list)){	*list =new;	return;}
new->prev =(*list);	new->next =(*list)->next;
if ((*list)->next)	(*list)->next->prev =new;
(*list)->next =new;}

	//i can't actually make such a generic function work for all types of items
	//except if i added a generic part to the list struct with things like position
void insert_inst_sorted_y(List** list, List* new){
if (!(*list)){	*list =new;	return;}
if (((Instance*)(new->hints))->y < ((Instance*)((*list)->hints))->y){
	if ((*list)->next =NULL){	insert_after(list, new);	return;}
	else	insert_inst_sorted_y(&((*list)->next), new);	return;}
else	insert_before(list, new);}
