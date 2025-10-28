/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct task{
    char name[50];
    struct task *next;
} task;

task *front = NULL;
task *rear  = NULL;
void enqueue(char *name);
void dequeue();
int  isEmpty();
void printQueue();
void clearQueue();

static task *makeNode(const char *s){
    task *t = (task*)malloc(sizeof(task));
    if(!t){
        fprintf(stderr,"alloc failed\n");
        exit(1);
    }
    //* copy name */
    strncpy(t->name, s, sizeof(t->name)-1);
    t->name[sizeof(t->name)-1] = '\0';
    t->next = NULL;
    return t;
}

void enqueue(char *name){
    /* add to back */
    task *n = makeNode(name);
    if (rear == NULL){           // empty queue
        front = n;
        rear  = n;
    } else {
        /* link old rear to new */
        rear->next = n;
        rear = n;                // move rear
    }
    /* nothing to return */
}

void dequeue(){
    // remove from front, if any
    if (front == NULL){
    } else {
        task *tmp = front;
        front = front->next;
        if (front == NULL){
            rear = NULL;
        }
        /* free it */
        free(tmp);
    }
}

int isEmpty(){
    if (front == NULL) return 1;
    return 0;
}

void printQueue(){
    task *p = front;
    while (p != NULL){
        /* print the name then newline */
        printf("%s\n", p->name);
        p = p->next;
    }
}

void clearQueue(){
    while (!isEmpty()){
        dequeue();
    }
}
int main(void){
    enqueue("Knight Parade");
    enqueue("Pegasus Showcase");
    enqueue("Spirit Splash");
    dequeue(); // removes Knight Parade
    printQueue();

    if (!isEmpty()){
        clearQueue();
    } else {
    }


    enqueue("Fireworks Finale");
    printQueue();
    clearQueue();

    return 0;
}