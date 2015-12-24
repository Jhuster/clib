/*
 *  COPYRIGHT NOTICE  
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author:  Jhuster(lujun.hust@gmail.com)
 *  
 *  https://github.com/Jhuster/C-Cpp
 *   
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.  
 */
#ifndef _KEY_LIST_H_
#define _KEY_LIST_H_

#include <stdlib.h>

/**
 *  You can set the *value point to your own structure
 *  You must also provide a value_releaser to release the *value
 */
typedef struct _value {
    void *value;
} value_t;

typedef int key_t;
typedef void (*value_releaser)(value_t value);

#define key_compare(a,b) ((a==b)?1:0)

typedef struct key_list_node { 
    key_t key;
    value_t value;
    struct key_list_node *prev;
    struct key_list_node *next;
} key_list_node_t;

typedef struct key_list {
    int count;
    key_list_node_t *header;      
    value_releaser releaser;
} key_list_t;

/**
 *  Create & Destroy the key list
 */
key_list_t *key_list_create();
int key_list_destroy(key_list_t *list);

/**
 *  Get the elements number in the list
 */
int key_list_count(key_list_t *list);

/**
 *  Get all the keys in the list 
 *  @param list the key_list_t object
 *  @param array an array to copy the keys into
 *  @param array_size the array's size
 *  @return the elements number in the list, -1 if the array_size is not enough
 */
int key_list_keyset(key_list_t *list,key_t* array,int array_size);

/**
 *  Check if the given key is exist 
 *  @return 1 exist, 0 not exist
 */
int key_list_find_key(key_list_t *list,key_t key);

/**
 *  CRUD on the key list
 *  @return 0 success, -1 failed
 */
int key_list_add(key_list_t *list,key_t key,value_t value);
int key_list_get(key_list_t *list,key_t key,value_t *value);
int key_list_edit(key_list_t *list,key_t key,value_t value);
int key_list_delete(key_list_t *list,key_t key);

#define key_list_foreach(L,V) key_list_node_t *_node = NULL;\
    key_list_node_t* V;\
    for(V = _node = L->header; _node != NULL; V = _node = _node->next)

#endif
