//
//  List.c
//  assignment
//
//  Created by mac on 2020/5/19.
//  Copyright © 2020 mac. All rights reserved.
//

#include "list.h"
#include <stdio.h>
static Node array_Nodes[LIST_MAX_NUM_NODES];
static List array_Heads[LIST_MAX_NUM_HEADS];
static int numberOfNodes=LIST_MAX_NUM_NODES;
static int numberOfHeads=LIST_MAX_NUM_HEADS;
List* List_create()
{
    if (numberOfHeads!=0)
    {
        List* new_List=&array_Heads[numberOfHeads--];
        new_List->current=NULL;
        new_List->tail=NULL;
        new_List->head=NULL;
        return new_List;                    // everything is null and return the list
    }
    else
        return NULL;
}
int List_count(List* pList)
{
    if (pList->head==NULL)              // if it is nothing then return null
        return 0;
    int count=0;
    Node* temp_current=pList->current;
    pList->current=pList->head;
    while (pList->current->data!=NULL)
    {
        count++;
        if (pList->current->next==NULL)         //if stay in the tail position
            break;
        pList->current=pList->current->next;
    }
    pList->current=temp_current;
    return count;
}
void * List_first(List* pList)
{
    if (pList->head == NULL)
    {
        pList->current = NULL;
        return NULL;
    }
    else
    {
        pList->current = pList->head;
        pList->current->pos=0;                  // make a tag of staying in head position
        return pList->head->data;
    }

}
void * List_last(List* pList)
{
    if (pList->tail == NULL)
    {
        pList->current = NULL;
        return NULL;
    }
    else
    {
        pList->current=pList->tail;
        pList->current->pos=0;                  // make a tag of staying in tail position
        return pList->tail->data;
    }
}

int List_add(List* pList, void* pItem)  // index of -1 is means head and -2 is means tail and pos is 1 means current item is beyond the tail or before the head
{
    if (numberOfNodes<=0||pItem==NULL||pList==NULL)     // use all nodes or pItem is null
        return -1;
    else if (pList->head==NULL)              // empty list   head->newnode(current)->tail  !
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        pList->head=new_node;
        pList->head->index=-1;
        pList->current=new_node;
        pList->tail=new_node;
        return 0;
    }
    else if (pList->current==pList->head&&pList->current->pos==0)           //at the position of head!
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        pList->head->next=new_node;
        new_node->prev=pList->head;
        pList->current=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        if (pList->current->next==NULL)
        {
            pList->tail=pList->current;
            pList->current->index=-2;
        }
        return 0;
    }
    else if (pList->current==pList->tail&&pList->current->pos==0)      //at the position of tail!
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        pList->tail->next=new_node;
        new_node->prev=pList->current;
        pList->tail=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        pList->current=new_node;
        pList->current->index=-2;
        return 0;
    }
    else if(pList->current->index==-1&&pList->current->pos==1)      // at the above of list
    {
        pList->current=pList->head;
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        new_node->next=pList->head;
        pList->head->prev=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        pList->current->pos=0;
        pList->current=new_node;
        pList->head=new_node;
        pList->current->index=-1;
        if (pList->current->next==NULL)
        {
            pList->tail=pList->current;
            pList->current->index=-2;
        }
        return 0;
    }
    else if(pList->current->index==-2&&pList->current->pos==1)       // at the beyond of list
    {
        pList->current=pList->tail;
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        new_node->prev=pList->current;
        pList->tail->next=new_node;
        pList->tail=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        pList->current=new_node;
        pList->current->index=-2;
        pList->current->pos=0;
        return 0;
    }
    else                                        //normal case
    {
        Node* new_node = &array_Nodes[numberOfNodes--];
        Node* temp=pList->current;
        pList->current=pList->current->next;
        new_node->data=pItem;
        temp->next=new_node;
        new_node->prev=temp;
        new_node->next=pList->current;
        pList->current=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        return 0;
    }
    return -1;

}

void* List_curr(List* pList)
{
    if (pList->current==NULL)
        return NULL;
    return pList->current->data;
}

// Adds item to pList directly before the current item, and makes the new item the current one.
// If the current pointer is before the start of the pList, the item is added at the start.
// If the current pointer is beyond the end of the pList, the item is added at the end.
// Returns 0 on success, -1 on failure.
int List_insert(List* pList, void* pItem)  // index of -1 is means head and -2 is means tail and pos is 1 means current item is beyond the tail or before the head
{
    if (numberOfNodes<=0||pItem==NULL||pList==NULL)     // use all nodes or pItem is null
            return -1;
    else if (pList->head==NULL)              // empty list   head->newnode(current)->tail
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        pList->head=new_node;
        pList->head->index=-1;
        pList->current=new_node;
        pList->tail=new_node;
        return 0;
    }
    else if (pList->current==pList->head&&pList->current->pos==0)      //at the position of head!
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        if (pList->current->index==-1&&pList->current->next==NULL)  // the situation of in head
        {
            pList->current->index=-2;
            new_node->next=pList->current;
            pList->current->prev=new_node;
            pList->head=new_node;
            pList->current=new_node;
            pList->current->index=-1;
            return 0;
        }
        else
        {
            pList->current->prev=new_node;
            new_node->next=pList->current;
            pList->head=new_node;
            pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
            pList->current=new_node;
            pList->current->index=-1;
            return 0;
        }
    }
    else if (pList->current==pList->tail&&pList->current->pos==0)     //at the position of tail!
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        pList->current=pList->tail->prev;
        pList->tail->prev=new_node;
        new_node->next=pList->tail;
        new_node->prev=pList->current;
        pList->current->next=new_node;
        pList->current=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        return 0;
    }
    else if(pList->current==pList->head&&pList->current->pos==1)      // at the above of list
    {
        pList->current=pList->head;
        pList->current->pos=0;
        List_insert(pList, pItem);
        return 0;
    }
    else if(pList->current==pList->tail&&pList->current->pos==1)     // at the beyond of list
    {
        Node* new_node = &array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        new_node->prev=pList->current;
        pList->current->next=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        pList->current->pos=0;
        pList->tail=new_node;
        pList->current=new_node;
        pList->current->index=-2;
        pList->current->pos=0;
        return 0;
    }
    else                                        //normal case
    {
        Node* new_node = &array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        Node* temp=pList->current;
        pList->current=pList->current->prev;
        temp->prev=new_node;
        new_node->next=temp;
        pList->current->next=new_node;
        new_node->prev=pList->current;
        pList->current=new_node;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        return 0;
    }
    return -1;

}

// Adds item to the front of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)  // index of -1 is means head and -2 is means tail
{
    if (numberOfNodes<=0||pItem==NULL||pList==NULL)     // use all nodes or pItem is null
        return -1;
    else
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        if (pList->head==NULL)
        {
             Node* new_node=&array_Nodes[numberOfNodes--];
            new_node->data=pItem;
            pList->head = new_node;
            pList->current=pList->head;
            pList->current=new_node;
            pList->tail = new_node;
            pList->head->index = -1;
            return 0;
        }
        else
        {
            new_node->next=pList->head;
            new_node->prev=NULL;
            pList->head->prev=new_node;
            pList->head->index=(LIST_MAX_NUM_NODES-numberOfNodes);
            pList->head=new_node;
            pList->current=new_node;
            pList->current->index=-1;
            return 0;
        }
    }
    return -1;
}
// Adds item to the end of pList, and makes the new item the current one.
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem) // index of -1 is means head and -2 is means tail
{
    if (numberOfNodes<=0||pItem==NULL||pList==NULL)     // use all nodes or pItem is null
           return -1;
    else if (pList->head==NULL)  // empty
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        pList->head = new_node;
        pList->current=pList->head;
        pList->current=new_node;
        pList->tail = new_node;
        pList->head->index = -1;
        return 0;
    }
    else                    // normal case
    {
        Node* new_node=&array_Nodes[numberOfNodes--];
        new_node->data=pItem;
        pList->current=pList->tail;
        pList->tail->next=new_node;
        new_node->prev=pList->current;
        pList->current->next=new_node;
        pList->tail=new_node;
        pList->tail->index=-2;
        pList->current->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        pList->current=new_node;
        return 0;
    }
    return -1;
}
void* List_prev(List* pList)
// index of -1 is means head and -2 is means tail and pos is 1 means current item is beyond the tail or before the head
{
    if(pList==NULL||pList->head==NULL)    //empty
        return NULL;
    else if (pList->current->pos==1&&pList->current==pList->head)   //at the before of head
    {
        return NULL;
    }
    else if (pList->current->pos==0&&pList->current==pList->head)   //in the position of head
    {
        pList->current->pos=1;
        return NULL;
    }
    else if (pList->current->pos==1&&pList->current==pList->tail)   // at the beyond of tail
    {
        pList->current=pList->tail;
        pList->current->pos=0;
        return pList->current->data;
    }
    else                                                        // normal case
    {
        pList->current=pList->current->prev;
        if (pList->current==pList->head)
            pList->current->pos=0;
        return pList->current->data;
    }
}

void* List_next(List* pList)
// index of -1 is means head and -2 is means tail and pos is 1 means current item is beyond the tail or before the head
{
    if(pList->head==NULL||pList==NULL)                                       // empty
        return NULL;
    if (pList->current->pos==1&&pList->current==pList->head)   // at the before of head
    {
        pList->current=pList->head;
        pList->current->pos=0;
        return pList->current->data;
    }
    else if (pList->current->pos==1&&pList->current==pList->tail)   // at the beyond of tail
    {
        return NULL;
    }
    else if (pList->current->pos==0&&pList->current==pList->tail) // at the tail
    {
        pList->current->pos=1;
        return NULL;
    }
    else                                                // normal case
    {
        pList->current=pList->current->next;
        if (pList->current==pList->tail)
            pList->current->pos=0;
        return pList->current->data;
    }
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
// index of -1 is means head and -2 is means tail and pos is 1 means current item is beyond the tail or before the head
{
    if (pList->head==NULL||pList==NULL)      //empty case
        return NULL;
                                    // if beyond the tail or before the head
    if                               ((pList->current->pos==1&&pList->current==pList->head)||(pList->current->pos==1&&pList->current==pList->tail))
        return NULL;
    else if (pList->current==pList->head&&pList->current->next!=NULL) // ai the top
    {
        Node* temp=pList->current;
        pList->current=pList->current->next;
        pList->current->prev=NULL;
        pList->head=pList->current;
        pList->current->pos=0;
        pList->current->index=-1;
        numberOfNodes++;
        return temp->data;
    }
    else if (pList->current==pList->head&&pList->current->next==NULL)  //only the head
    {
        Node* temp=pList->current;
        numberOfHeads++;
        numberOfNodes++;
        pList->head=NULL;
        pList->tail=NULL;
        pList->current=NULL;
        pList=NULL;
        return temp->data;
    }
    else if (pList->current==pList->tail&&pList->current->prev!=NULL)  // at the end not empty
    {
        Node* temp=pList->current;
        pList->current=pList->current->prev;
        pList->current->next=NULL;
        pList->tail=pList->current;
        pList->current->index=-2;
        pList->current->pos=0;
        numberOfNodes++;
        return temp->data;
    }
    else if (pList->current==pList->tail&&pList->current->prev==NULL)  // at the end
    {
        Node* temp=pList->current;
        numberOfNodes++;
        pList->head=NULL;
        pList->tail=NULL;
        pList->current=NULL;
        pList=NULL;
        return temp->data;
    }                                           // normal case
    else
    {
        Node* temp=pList->current;
        Node* temp1=pList->current->prev;
        pList->current=pList->current->next;
        pList->current->prev=temp1;
        temp1->next=pList->current;
        numberOfNodes++;
        return temp->data;
    }
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
    if (pList->head==NULL||pList==NULL)
        return NULL;
    pList->current=pList->tail;
    Node* temp=pList->current;
    pList->current=pList->tail->prev;
    pList->current->next=NULL;
    pList->tail=pList->current;
    pList->current->index=-2;
    numberOfNodes++;
    return temp->data;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item.
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are
// available for future operations.
// UPDATED: Changed function pointer type, May 19
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn)
{
    void*temp=NULL;
    if (pList->head!=NULL)
    {
        while (pList->head!=pList->tail)
        {
            pList->current=pList->tail;
            temp=List_remove(pList);
            pItemFreeFn(temp);
        }
        if (pList->head==pList->tail)
        {
            pList->current=pList->head;
            temp=List_remove(pList);
            pItemFreeFn(temp);
        }
        pList=NULL;
    }
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1.
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
// index of -1 is means head and -2 is means tail and pos is 1 means current item is beyond the tail or before the head

{
    if (pList2==NULL||pList1==NULL)
        return;
    else
    {
        Node* temp=pList1->current;
        pList1->current=pList1->tail;           // one for tail of pList1
        pList2->current=pList2->head;           // one for head of pList2
        pList1->current->next=pList2->current;
        pList2->current->prev=pList1->current;
        pList1->tail=pList2->tail;
        pList1->tail->index=-2;
        pList2->head->index=(LIST_MAX_NUM_NODES-numberOfNodes);
        pList1->current=temp;
    }
    List* change=List_create();
    int size=List_count(pList1);
    for (int i=0;i<size;i++)
        change[i]=pList1[i];
    for (int i=0;i<size;i++)
        pList1[i]=change[i];
    
    void*temp2=NULL;
    if (pList2->head!=NULL)
    {
        while (pList2->head!=pList2->tail)
        {
            pList2->current=pList2->tail;
            temp2=List_remove(pList2);
        }
        if (pList2->head==pList2->tail)
        {
            pList2->current=pList2->head;
            temp2=List_remove(pList2);
            pList2=NULL;
        }
        
    }
}

// Search pList, starting at the current item, until the end is reached or a match is found.
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match,
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator.
//
// If a match is found, the current pointer is left at the matched item and the pointer to
// that item is returned. If no match is found, the current pointer is left beyond the end of
// the list and a NULL pointer is returned.
//
// UPDATED: Added clarification of behaviour May 19
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg)
// index of -1 is means head and -2 is means tail and pos is 1 means current item is beyond the tail or before the head
{
    if (pList->head==NULL||pList==NULL)          // empty case
        return NULL;
    if (pList->current->pos==1&&pList->current==pList->tail)  //at the beyond of tail
        return NULL;
    if (pList->current->pos==1&&pList->current==pList->head)  // at the before of head
    {
        pList->current=pList->head;
        pList->current->pos=0;
        List_search(pList, pComparator, pComparisonArg);
    }
    else                                // normal case
    {
        while (pList->current!=pList->tail)
        {
            if (pComparator(pList->current->data,pComparisonArg))
                return pList->current->data;
            pList->current=pList->current->next;
        }
        if (pList->current==pList->tail)     // at the tail position
        {
            pList->current=pList->tail;
            if (pComparator(pList->current->data,pComparisonArg))
                return pList->current->data;
            pList->current->pos=1;
            return NULL;
        }
    }
    return NULL;
}
