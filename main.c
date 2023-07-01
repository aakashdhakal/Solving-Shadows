#include "checkpoints.h"
#include "inventory.h"
#include "gameStory.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

int lives = 3;
char currentRoom[20];
int exitRoom = 0;
char message[500];
int hasKey = 0;

void firstLevel();
void wrapperFunction(int n);

void vline(char ch, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%c", ch);
    }
    printf("\n");
}

void gameDetails()
{
    Inventory *items = inventoryHead;
    vline('-', 120);
    printf(" Inventory: ");
    if (inventoryHead == NULL)
    {
        printf("Empty");
    }
    while (items != NULL)
    {
        printf(" %s ", items->itemName);
        items = items->next;
    }
    printf("\t\t\t\t\t\t\t\t\t\t\t       Lives: %d", lives);
    printf("\n");
    vline('-', 120);
}

void consoleOutput(void (*func)(int), int value)
{
    printf("\033[2J\033[1;1H");
    gameDetails();
    func(value);
}
void firstLevel()
{
    int choice;
    printf("%s", message);
    printf(" \n 1. Search the room\n");
    printf(" 2. Open the door\n");
    printf("\n What do you want to do?: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        if (hasKey == 0)
        {
            strcpy(message, " You carefully search the room, examining every nook and cranny.\n After a thorough search, you notice a small painting on the wall that seems out of place.\n You take it off the wall and find a key hidden behind it!\n");
            hasKey = 1;
            strcpy(inventoryObject, "Key");
            addToInventory();
        }
        else
        {
            strcpy(message, " You search the room again, but find nothing of interest.\n");
        }
        break;
    case 2:
        if (hasKey == 0)
        {
            strcpy(message, " The door is locked. You need to find a key.\n");
        }
        else
        {
            strcpy(message, " You use the key to unlock the door and escape the room.\n");
            deleteFromInventory();
            exitRoom = 1;
        }
        break;
    default:
        strcpy(message, " You have no choice to perform this action\n");
        break;
    }
    consoleOutput(wrapperFunction, 1);
}
void wrapperFunction(int n)
{
    if (n == 1)
    {
        firstLevel();
    }
}
int main()
{
    consoleOutput(gameStory, 1);
    firstLevel();
    return 0;
}