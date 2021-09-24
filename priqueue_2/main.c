
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "priqueue.h"

#if 0
/*
    single thread test
*/
int main(int argc, char** argv)
{

    char *val;
    int priority;

    ptable *p = malloc(sizeof(ptable));
    priqueue_create(p);

    printf("priqueue \n");

    priqueue_put_data(p, "this is 1", 1);
    priqueue_put_data(p, "this is 4", 4);
    priqueue_put_data(p, "this is 2", 2);
    priqueue_put_data(p, "this is 51", 5);
    priqueue_put_data(p, "this is 5", 4);
    priqueue_put_data(p, "this is 61", 6);
    priqueue_put_data(p, "this is 3", 3);
    priqueue_put_data(p, "this is 6", 4);

    priqueue_display(p);
    printf("--------------------- \n");

    priqueue_get_data(p, &val, &priority);

    priqueue_display(p);
    printf("--------------------- \n");

    priqueue_get_data(p, &val, &priority);

    priqueue_display(p);
    printf("--------------------- \n");

    priqueue_put_data(p, "5", 4);
    priqueue_put_data(p, "5", 4);

    priqueue_get_data(p, &val, &priority);

    priqueue_display(p);
    printf("--------------------- \n");

    priqueue_get_data(p, &val, &priority);

    priqueue_display(p);
    printf("--------------------- \n");

    priqueue_get_data(p, &val, &priority);

    priqueue_display(p);
    printf("--------------------- \n");

}

#else

/*
    multi-threads test
*/

/*
 * Test code
 *   Use two threads, acting as producer and consumer
 */
void* producer(void* p)
{
    ASSERT(p);

    ptable *table = (ptable*)p;

    printf("Thread producer\n");
    int i = 0;

    while (1) {
        /*
         * We break the producer after enqueuing 16 messages
         */
        if (i == 16) {
            break;
        }

        char *val = malloc(32);

        memset(val, 0x00, 32);
        sprintf(val, "number %d", i);
        printf("---> put_data: %s priority: %d \n", val, (i % 9));
        /*
         * Using max bucket as (MAX_PRI - 1)
         */
        priqueue_put_data(p, val, (i % 9));
        i++;

        sleep(1);
    }
}

void* consumer(void* p)
{
    sleep(2);
    ptable *table = (ptable*)p;

    char* val;
    int priority;

    printf("Thread consumer\n");
    int i = 0;

    while (1) {
        printf("<--- get_data\n");
        priqueue_get_data(p, &val, &priority);
        printf("Search-> Priority = %d val = %s\n\n", priority, val);
        free(val);

        /*
         * We break the consumer after dequeuing 16 messages.
         * The next call to get_data will block since there
         * will be no data from the producer
         */
        if (i == 15) {
            break;
        }
    }
}

void cleanup(ptable *p)
{
    node *n = p->buf_pool;

    while (n) {
        node* temp = n;
        n = n->next;
        free(temp);
    }

    free(p);
}

int main()
{
    ptable *p = malloc(sizeof(ptable));
    priqueue_create(p);

    pthread_t thread1, thread2;

    int iret1, iret2;

    iret1 = pthread_create(&thread1, NULL, producer, (void*) p);
    iret2 = pthread_create(&thread2, NULL, consumer, (void*) p);

    priqueue_display(p);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    cleanup(p);
    printf("cleanup \n");
}
#endif
