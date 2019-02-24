#include "stdafx.h"
#include "mylist.h"
#include <stdio.h>
#include <stdlib.h>

mylist::mylist(){
	num = 0;
	head = prev_node = curr_node = NULL;
}

mylist::~mylist(){
	move_first();
	while(isEmpty()==false) del();
}

bool mylist::isEmpty(void){
	if(num==0) return true;
	return false;
}

void mylist::add(Point* tpoint){
	node *NewN=(node *)malloc(sizeof(node));
	Point *NewPt=(Point *)malloc(sizeof(Point));
	NewPt->x=tpoint->x;NewPt->y=tpoint->y;
	NewN->point=NewPt;
	NewN->next=NULL;

	if(curr_node==NULL)
	{
		head=NewN;
		curr_node=NewN;
	}
	else{
			curr_node->next=NewN;
			prev_node=curr_node;
			curr_node=NewN;
	}
		num++;
	// 코드필요
}

node* mylist::del(void){
	node* temp;
	if(curr_node==NULL)
		return NULL;
	
	temp = curr_node;

	prev_node->next = curr_node->next; 
	curr_node = curr_node->next;
	free(temp->point); 
	free(temp);
	num--;
	return curr_node;
}

node* mylist::move_first(void){
	curr_node=head;
	prev_node=head;
	return curr_node;
	//코드 필요
}

node* mylist::move_next(void){

	if(curr_node==NULL)
		return NULL;
	prev_node=curr_node;
	curr_node=curr_node->next;
		return curr_node;
	//코드 필요
}