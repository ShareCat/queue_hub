#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
#include "assert.h"
#include "string.h"

#ifndef __PRIQUEUE_H__
#define __PRIQUEUE_H__

#define ASSERT(x) assert(x)
#define PRI_MAX 8
#define BUF_POOL_SIZE 32

#if 0
    #define LOG   printf
    #define LOG2  printf
    #define LOG3  printf
#else
    #define LOG   //
    #define LOG2  //
    #define LOG3  //
#endif

#define LOCK(x) pthread_mutex_lock(&x)
#define UNLOCK(x) pthread_mutex_unlock(&x)

typedef enum bool_ {
    false,
    true
} bool;

typedef struct queue_stats_ {
    int enqueue;
    int dequeue;
    int wait_time;
} queue_stats;

int priority[PRI_MAX];

/*
 * List of nodes in a hash bucket
 */
typedef struct node_ {
    void *val;
    int priority;
    struct node_* next;
} node;

/*
 * Define the hash table
 * |p1| ->|a|->|b|->|c|
 * |p2|->|e|->|f|
 */
typedef struct ptable_entry_ {
    int priority;
    node* n;
} ptable_entry;

typedef struct ptable_ {
    ptable_entry entry[PRI_MAX];
    node* last[PRI_MAX];
    node* buf_pool;
    node* free_bbuf_pool;
    int ent_count;
    pthread_mutex_t lock;
    pthread_cond_t cv;
    bool is_available;
    queue_stats *stats;
} ptable;

void priqueue_create(ptable*);
void priqueue_get_data(ptable*, void**, int*);
int priqueue_is_available(ptable* p);
void priqueue_put_data(ptable*, void* val, int priority);
void priqueue_destroy(ptable*);
void priqueue_display(ptable*);
void priqueue_put_buf(ptable* p, void* buf);
void priqueue_create_pool(ptable** p, int num);
void* priqueue_get_buf(ptable* p);
void priqueue_display_buf_pool(ptable* p);

/*
 * Helper functions
 */

void priqueue_add_a_node(ptable* p, node** last, node** m, void* val, int priority);

#endif  /* __PRIQUEUE_H__ */
