#include "priqueue.h"

/*
 * Adds a node of a given priority to the queue. Since a node is
 * allocated from a fixed size buffer pool, this function blocks
 * if pool has no free buffer object.
 */
void priqueue_add_a_node(ptable* p, node** last, node** m, void* val, int priority)
{
    ASSERT(p);

    LOCK(p->lock);
    node *n = NULL;

    n = (node*)priqueue_get_buf(p);

#ifdef DEBUG
    LOG3("oo-get_data-oo\n");
    priqueue_display_buf_pool(p);
    LOG3("---get_data--\n");
#endif

    if (NULL == n) {
        LOG2("Buf pool is over. Waiting for dequeue\n");
        pthread_cond_wait(&p->cv, &p->lock);
        n = (node*)priqueue_get_buf(p);
        LOG2("Producer: wait over. Got a buffer back\n");
    }

    /*
     * Collided nodes are arranged in a list (queue)
     */
    n->val = val;
    n->priority = priority;
    n->next = NULL;

    if (NULL == *m) {
        *m = n;
    } else {
        (*last)->next = n;
    }

    *last = n;
    LOG("Enqueue: %d\n", p->stats->enqueue++);

    p->is_available = true;
    pthread_cond_signal(&p->cv);
    UNLOCK(p->lock);
}

/*
 * Gets a buffer from the buffer pool
 */
void* priqueue_get_buf(ptable *p)
{
    /*
     * Check if we have at least two nodes
     */
    node* head = p->buf_pool;

    if (p->buf_pool != NULL) {
        p->buf_pool = head->next;
        LOG2("Stealing a buffer %p\n", head);
        return head;
    } else {
        LOG2("\nBuffer overrun\n");
        return NULL;
    }
}

/*
 * Returns a buffer to buffer pool
 */
void priqueue_put_buf(ptable* p, void* buf)
{
    if (p->buf_pool) {
        node* head = (node*)buf;
        head->next = p->buf_pool;
        p->buf_pool = head;
        LOG2("Unstealing a buffer %p\n", buf);
    } else {
        p->buf_pool = buf;
        LOG2("Unstealing the last buffer %p\n", buf);
    }
}

void priqueue_display_buf_pool(ptable* p)
{
    ASSERT(p);

    int i = 1;
    node* temp = p->buf_pool;

    while (temp) {
        LOG2("Buf %d: %p\n", i++, temp);
        temp = temp->next;
    }
}

void priqueue_create_pool(ptable** p, int num)
{
    node* head = NULL;
    node* temp = NULL;

    int i = 0;

    head = malloc(sizeof(node));

    temp = head;

    for (i = 1; i < num; i++) {
        temp->next = malloc(sizeof(node));
        temp = temp->next;
    }

    temp->next = NULL;

    /*
     * Set the buf pool
     */
    if (NULL == (*p)->buf_pool) {
        (*p)->buf_pool = head;
    }

#ifdef DEBUG
    priqueue_display_buf_pool(*p);
#endif

}

/*
 * Create a priority queue object of priority ranging from 0..PRIMAX-1
 */
void priqueue_create(ptable* p)
{
    ASSERT(p);

    int i = 0;

    memset(p, 0x00, sizeof(ptable));

    /*
     * Initialize the entries
     */
    for (i = 0; i < PRI_MAX; i++) {
        p->entry[i].priority = i;
        p->entry[i].n = NULL;
        p->last[i] = NULL;
    }

    priqueue_create_pool(&p, BUF_POOL_SIZE);

    p->stats = malloc(sizeof(queue_stats));

    memset(&(p->lock), 0, sizeof(pthread_mutex_t));
    memset(&(p->cv), 0, sizeof(pthread_cond_t));

    p->is_available = false;
    p->ent_count = PRI_MAX;
}

/*
 * Adds a node to the queue
 */
void priqueue_put_data(ptable* p, void* val, int priority)
{
    ASSERT(p);
    ASSERT(priority < PRI_MAX);

    priqueue_add_a_node(p, &(p->last[priority]), &(p->entry[priority].n),
               val, priority);
}

static int priqueue_available_update(ptable* p)
{
    int i = 0;
    for (i = 0; i < PRI_MAX; i++) {
        if (NULL != p->entry[i].n) {
            return 1;
        }
    }

    p->is_available = false;
    return 0;
}

/*
 * Gets the highest priority node from the queue. If queue is empty,
 * then this routine blocks.
 */
void priqueue_get_data(ptable* p, void** val, int* pri)
{
    ASSERT(p);

    LOCK(p->lock);
    int i = 0;
    node* temp = NULL;

wait_again:

    while (false == p->is_available) {
        /*
         * Else wait for the next element to get in
         */
        LOG2("Nothing in queue; waiting for data\n");
        pthread_cond_wait(&p->cv, &p->lock);
        LOG2("Waiting completed: got data\n");
    }

    for (i = 0; i < PRI_MAX; i++) {
        if (NULL != p->entry[i].n) {
            temp = (p->entry[i].n);

            *val = p->entry[i].n->val;
            *pri = p->entry[i].n->priority;

            p->entry[i].n = temp->next;

            LOG(" Dequeued: %d\n", p->stats->dequeue++);
            priqueue_put_buf(p, temp);
            priqueue_available_update(p);
#ifdef DEBUG
            LOG3("oo-put_data-oo\n");
            priqueue_display_buf_pool(p);
            LOG3("---put_data--\n");
#endif
            pthread_cond_signal(&p->cv);
            UNLOCK(p->lock);
            return;
        }
    }

    p->is_available = false;
    goto wait_again;
}


/*
 * check priqueue is available or not
 */
int priqueue_is_available(ptable* p)
{
    ASSERT(p);

    if (p->is_available) {
        return true;
    } else {
        return false;
    }
}


/*
 * Function to display the queue
 */
void priqueue_display(ptable* p)
{
    ASSERT(p);
    int i = 0;
    node* t = NULL;

    for (i = 0; i < PRI_MAX; i++) {
        t = p->entry[i].n;

        while (t) {
            printf("\nBucket=%d val=%s Priority=%d\n", p->entry[i].priority,
                   (char *)t->val,
                   t->priority);
            t = t->next;
        }
    }
}
