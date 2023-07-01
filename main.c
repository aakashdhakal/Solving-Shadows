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

void consoleOutput(void (*func)())
{
    printf("\033[2J\033[1;1H");
    gameDetails();
    func();
}
void firstLevel()
{
    gameStory(1);
    int choice;
    do
    {
        printf(" \n 1. Search the room\n");
        printf(" 2. Open the door\n");
        printf("%s", message);
        printf("\n What do you want to do?:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            strcpy(message, "");
            strcpy(currentRoom, "firstLevel");
            searchRoom();
            break;
        }
        while (exitRoom == 0)
            ;
    }

    int main()
    {
        consoleOutput(firstLevel);
        return 0;
    }