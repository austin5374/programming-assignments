/*
COP3502C | Fall 2025 | Section 0004
Name: Austin Vodrazka
UCF ID: 5524103
*/

#include <stdio.h>

#define SIZE 10
#define EMPTY -1

struct stack {
    int items[SIZE];
    int top;
};

void initialize(struct stack *stackPtr);
int  full(struct stack *stackPtr);
int  push(struct stack *stackPtr, int value);
int  empty(struct stack *stackPtr);
int  pop(struct stack *stackPtr);
int  peek(struct stack *stackPtr);
void display(struct stack *stackPtr);
void clearStack(struct stack *stackPtr);

void initialize(struct stack *stackPtr) {
    stackPtr->top = -1;
}

int full(struct stack *stackPtr){
    return (stackPtr->top == SIZE - 1);
}

int push(struct stack *stackPtr, int value) {
    if (full(stackPtr)) {
        printf("Stack is full! Cannot push %d", value);
        return 0;
    }
    stackPtr->items[++(stackPtr->top)] = value;
    return 1;
}

int empty(struct stack *stackPtr){
    return stackPtr->top == -1;
}

int pop(struct stack *stackPtr){
    if (empty(stackPtr)){
        printf("Stack is empty! Cannot pop");
        return EMPTY; // -1
    }
    return stackPtr->items[(stackPtr->top)--];
}

int peek(struct stack *stackPtr) {
    if (empty(stackPtr)) {
        return EMPTY; // -1
    }
    return stackPtr->items[stackPtr->top];
}

void display(struct stack *stackPtr) {
    printf("\nCurrent stack: ");
    if (empty(stackPtr)) {
        printf("EMPTY");
        return;
    }
    for (int i = 0; i <= stackPtr->top; i++) {
        printf("%d", stackPtr->items[i]);
    }
    printf("\n");
}

void clearStack(struct stack *stackPtr) {
    if (empty(stackPtr)) {
        printf("There is nothing to ");
        return;
    }
    stackPtr->top = -1;
}

int main() {
    struct stack stack1, stack2;
    int ele, ch;

    initialize(&stack1);
    initialize(&stack2);

    while (1) {
        printf("\nMenu:\n"
               " 1: Stack 1 PUSH\n"
               " 2: Stack 2 PUSH\n"
               " 3: Stack 1 POP\n"
               " 4: Stack 2 POP\n"
               " 5: Stack 1 DISPLAY\n"
               " 6: Stack 2 DISPLAY\n"
               " 7: DISPLAY Both Stacks\n"
               " 8: EXIT\n"
               "Enter your choice: ");
        if (scanf("%d", &ch) != 1) return 0;

        if (ch == 8) {
            printf("<<EXIT>>\n");
            break;
        } else if (ch == 1) {
            printf("Enter an element to PUSH in Stack 1: ");
            scanf("%d", &ele);
            push(&stack1, ele);
            display(&stack1);
        } else if (ch == 2) {
            printf("Enter an element to PUSH in Stack 2: ");
            scanf("%d", &ele);
            push(&stack2, ele);
            display(&stack2);
        } else if (ch == 3) {
            ele = pop(&stack1);
            if (ele != EMPTY) {
                printf("ITEM POPPED from Stack 1: %d\n", ele);
            }
            display(&stack1);
        } else if (ch == 4) {
            ele = pop(&stack2);
            if (ele != EMPTY) {
                printf("ITEM POPPED from Stack 2: %d\n", ele);
            }
            display(&stack2);
        } else if (ch == 5) {
            display(&stack1);
        } else if (ch == 6) {
            display(&stack2);
        } else if (ch == 7) {
            printf("Stack 1:");
            display(&stack1);
            printf("\n");
            printf("Stack 2:");
            display(&stack2);
            printf("\n");
        } else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}