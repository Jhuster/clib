/*
 *  COPYRIGHT NOTICE  
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author:  Jhuster(lujun.hust@gmail.com)
 *  
 *  https://github.com/Jhuster
 *   
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.  
 */
#include "key_list.h"

key_list_t *key_list_create() 
{
    key_list_t * list = calloc(1, sizeof(key_list_t));
    list->count = 0;
    list->header = NULL;
    return list;
}

void key_list_destroy(key_list_t *list) 
{
    key_list_node_t *current = list->header;    
    while(current != NULL) {
        key_list_node_t *next = current->next;
        free(current);        
        current = next;
    }
    free(list);
}

lkey_t key_list_get_key(key_list_t *list,int index)
{
    int i=0;
    if(index < 0 || index >= list->count) {
        return -1;
    }
    key_list_node_t *current = list->header;
    while(current!=NULL&&i!=index) {
        current = current->next;
        i++;
    }
    return current->key; 
}

int key_list_count(key_list_t *list) 
{
    return list->count;    
}

static key_list_node_t* key_list_get_node(key_list_t *list,lkey_t key) 
{
    key_list_node_t *current = list->header;    
    while(current != NULL) {
        if(key_compare(key,current->key) == 0) {
            return current;
        }       
        current = current->next;
    }
    return NULL;
}

int key_list_find_key(key_list_t *list,lkey_t key) 
{
    return key_list_get_node(list,key)!=NULL;
}

int key_list_add(key_list_t *list,lkey_t key,value_t value)
{
    if(key_list_find_key(list,key) == 1) {
        return -1;
    }

    key_list_node_t* node = calloc(1, sizeof(key_list_node_t));
    if(node == NULL) {
        return -1;
    }

    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    if(list->header != NULL) {
        node->next = list->header;
        list->header->prev = node;                
    }
    list->header = node;      
    list->count++;

    return 0;   
}

int key_list_get(key_list_t *list,lkey_t key,value_t *value) 
{
    key_list_node_t* node = key_list_get_node(list,key);
    if(node == NULL) {
        return -1;
    }
    *value = node->value;
    return 0;
}

int key_list_update(key_list_t *list,lkey_t key,value_t value)
{
    key_list_node_t* node = key_list_get_node(list,key);
    if(node == NULL) {
        return -1;
    }
    node->value = value;
    return 0;
}

static int key_list_remove(key_list_t *list,key_list_node_t *node) 
{
    if( node == list->header ) {
        list->header = node->next;
    }
    else {
        node->prev->next = node->next;
    }

    if( node->next != NULL ) {
        node->next->prev = node->prev;                        
    }            

    free(node);
    list->count--;

    return 0;
}

int key_list_delete(key_list_t *list,lkey_t key) 
{
    key_list_node_t* node = key_list_get_node(list,key);
    if(node == NULL) {
        return -1;
    }

    return key_list_remove(list,node);
}
