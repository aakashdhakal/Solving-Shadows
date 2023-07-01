#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gameStory.h"

// Global variables
int lives = 3;
char currentRoom[20];
int exitRoom = 0;
char message[500];

struct choice
{

    char choice[100];
    struct choice *nextChoice;
    struct choice *nextStory;
};
typedef struct choice Choice;
Choice *choiceHead = NULL;

struct checkpoint
{
    void (*func)();
    char checkpointType[5];
    struct checkpoint *next;
};
typedef struct checkpoint Checkpoint;
Checkpoint *top = NULL;

struct inventory
{
    char itemName[20];
    struct inventory *next;
};
typedef struct inventory Inventory;
Inventory *inventoryHead = NULL;

// Funcion to display the choices
void displayChoice()
{
    Choice *temp = choiceHead;
    int i = 1;
    while (temp != NULL)
    {
        printf("%d. %s\n", i, temp->choice);
        temp = temp->nextChoice;
        i++;
    }
}

// Function to add choices
void addChoice(char choice[100])
{
    Choice *newChoice = (Choice *)malloc(sizeof(Choice));
    strcpy(newChoice->choice, choice);
    newChoice->nextChoice = NULL;
    if (choiceHead == NULL)
    {
        choiceHead = newChoice;
    }
    else
    {
        Choice *temp = choiceHead;
        while (temp->nextChoice != NULL)
        {
            temp = temp->nextChoice;
        }
        temp->nextChoice = newChoice;
    }
}

// Function to delete choices
void deleteChoice(char *choice)
{
    Choice *temp = choiceHead;
    Choice *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->choice, choice) == 0)
        {
            if (prev == NULL)
            {
                choiceHead = temp->nextChoice;
            }
            else
            {
                prev->nextChoice = temp->nextChoice;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->nextChoice;
    }
}

// Function to add items in inventory
void addItem(char item[10])
{
    Inventory *newItem = (Inventory *)malloc(sizeof(Inventory));
    strcpy(newItem->itemName, item);
    newItem->next = NULL;
    if (inventoryHead == NULL)
    {
        inventoryHead = newItem;
    }
    else
    {
        Inventory *temp = inventoryHead;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newItem;
    }
}

// Function to delete items from inventory
void deleteItem(char *item)
{
    Inventory *temp = inventoryHead;
    Inventory *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->itemName, item) == 0)
        {
            if (prev == NULL)
            {
                inventoryHead = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }
}

// Function to display inventory
void displayInventory()
{
    Inventory *temp = inventoryHead;
    while (temp != NULL)
    {
        printf("%s\n", temp->itemName);
        temp = temp->next;
    }
}

// Function to add checkpoint
void pushCheckpoint(void (*func)(), char checkpointType[5])
{
    Checkpoint *newCheckpoint = (Checkpoint *)malloc(sizeof(Checkpoint));
    newCheckpoint->func = func;
    strcpy(newCheckpoint->checkpointType, checkpointType);
    newCheckpoint->next = NULL;
    if (top == NULL)
    {
        top = newCheckpoint;
    }
    else
    {
        Checkpoint *temp = top;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newCheckpoint;
    }
}
void popCheckpoint()
{
    Checkpoint *temp = top;
    Checkpoint *prev = NULL;

    while (temp && strcmp(temp->checkpointType, "main") != 0)
    {
        if (strcmp(temp->checkpointType, "temp") == 0)
        {
            if (!prev)
                top = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            temp = (prev ? prev->next : top);
        }
        else
        {
            prev = temp;
            temp = temp->next;
        }
    }
}

// Funcion to display horizontal line
void vline(char ch, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%c", ch);
    }
    printf("\n");
}

// Function to update the status bar
void updateConsole()
{
    printf("\033[2J\033[1;1H");
    vline('-', 120);
    printf("Inventory: ");
    displayInventory();
    printf("\t\t\t\t\t\t\t\t\t\t\t       Lives: %d", lives);
    printf("\n");
    vline('-', 120);
}
