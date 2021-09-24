
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "priqueue.h"

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

