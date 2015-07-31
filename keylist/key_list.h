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
#ifndef _KEY_LIST_H_
#define _KEY_LIST_H_

#include <stdlib.h>

/**
 *  You can use the void *p point to your own data, \
 *  Or define your own struct to replace the void *p
 *  Notice: if used point like void *p, you need manage the buffer manually
 */
typedef struct _value {
    void * p;
}value_t;

/**
 * You can define your own key type, replace the define and \
 * implement the key_compare(a,b)
 */
typedef long lkey_t;

#define key_compare(a,b) ((a==b)?0:-1)

typedef struct key_list_node { 
    lkey_t key;
    value_t value;
    struct key_list_node *prev;
    struct key_list_node *next;
}key_list_node_t;

typedef struct key_list {
    int count;
    key_list_node_t *header;      
}key_list_t;

/**
 *  Create the key list
 */
key_list_t *key_list_create();

/**
 *  Destroy the key list
 *  The point in value_t must be freed manually
 */
void key_list_destroy(key_list_t *list);

/**
 *  Get current list elements number
 */
int key_list_count(key_list_t *list);

/**
 *  Get the key value by index
 *  The index must > 0 and < key_list_count
 *  @return -1 error or the key value
 */
lkey_t key_list_get_key(key_list_t *list,int index);

/**
 *  Check if the given key is exist 
 *  @return 1 exist, 0 not exist
 */
int key_list_find_key(key_list_t *list,lkey_t key);

/**
 *  Add a new value to the key list 
 *  If the key exist, the add will fail
 *  @return 0 success, -1 failed
 */
int key_list_add(key_list_t *list,lkey_t key,value_t value);

/**
 *  Get the value with the given key
 *  @return 0 success, -1 failed
 */
int key_list_get(key_list_t *list,lkey_t key,value_t *value);

/**
 *  Update the value with the given key
 *  @return 0 success, -1 failed
 */
int key_list_update(key_list_t *list,lkey_t key,value_t value);

/**
 *  Delete the value with the given key
 *  @return 0 success, -1 failed
 */
int key_list_delete(key_list_t *list,lkey_t key);

#endif
