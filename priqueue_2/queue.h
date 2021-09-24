#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"
#include "assert.h"
#include "string.h"

#define ASSERT(x) assert(x)
#define PRI_MAX 10
#define BUF_POOL_SIZE 1024
#define uint32_t int

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
    char *key;
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

void create(ptable*);
void get_data(ptable*, char**, int*);
void put_data(ptable*, char* key, int priority);
void destroy(ptable*);
void display(ptable*);
void put_buf(ptable* p, void* buf);
void create_pool(ptable** p, uint32_t num);
void* get_buf(ptable* p);
void display_buf_pool(ptable* p);

/*
 * Helper functions
 */

void add_a_node(ptable* p, node** last, node** m, char* key, int priority);
