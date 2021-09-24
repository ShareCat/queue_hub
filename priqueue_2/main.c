
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "queue.h"

int main(int argc, char** argv)
{

    char *key;
    int priority;

    ptable *p = malloc(sizeof(ptable));
    create(p);

    printf("priqueue \n");

    put_data(p, "this is 1", 1);
    put_data(p, "this is 4", 4);
    put_data(p, "this is 2", 2);
    put_data(p, "this is 51", 5);
    put_data(p, "this is 5", 4);
    put_data(p, "this is 61", 6);
    put_data(p, "this is 3", 3);
    put_data(p, "this is 6", 4);

    display(p);
    printf("--------------------- \n");

    get_data(p, &key, &priority);

    display(p);
    printf("--------------------- \n");

    get_data(p, &key, &priority);

    display(p);
    printf("--------------------- \n");

    put_data(p, "5", 4);
    put_data(p, "5", 4);

    get_data(p, &key, &priority);

    display(p);
    printf("--------------------- \n");

    get_data(p, &key, &priority);

    display(p);
    printf("--------------------- \n");

    get_data(p, &key, &priority);

    display(p);
    printf("--------------------- \n");

}

