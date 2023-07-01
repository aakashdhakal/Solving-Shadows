// stack using linked lists
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct checkpoint
{
    int choiceNumber;
    char checkpointType[20];
    struct checkpoint *next;
};
typedef struct checkpoint Checkpoint;

Checkpoint *top = NULL;

void push(int choiceNumber, char checkpointType[20])
{
    Checkpoint *temp = (Checkpoint *)malloc(sizeof(Checkpoint));
    temp->choiceNumber = choiceNumber;
    strcpy(temp->checkpointType, checkpointType);
    temp->next = top;
    top = temp;
}
void pop()
{
    if (top == NULL)
    {

        return;
    }
    Checkpoint *temp = top;
    top = top->next;
    free(temp);
}