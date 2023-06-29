#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "gameStory.h"

char inventoryObject[20];
int lives = 3;
char currentRoom[20];
char message[500];

struct inventory
{
    char itemName[20];
    struct inventory *next;
};
typedef struct inventory Inventory;
Inventory *inventoryHead = NULL;

void vline(char ch, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%c", ch);
    }
    printf("\n");
}

int inventoryAction(int n)
{
    if (n == 1)
    {
        Inventory *items = (Inventory *)malloc(sizeof(Inventory));
        strcpy(items->itemName, inventoryObject);
        items->next = NULL;
        if (inventoryHead == NULL)
        {
            inventoryHead = items;
        }
        else
        {
            Inventory *temp = inventoryHead;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = items;
        }
    }
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
void mainGame()
{

    int input, hasKey = 0;
    system("cls");
    gameDetails();
    firstStory();
    printf("%s", message);

    do
    {
        printf("\n Enter your choice: ");
        scanf("%d", &input);
        switch (input)
        {
        case 1:
            strcpy(message, " You carefully search the room, examining every nook and cranny.\n After a thorough search, you notice a small painting on the wall that seems out of place.\n You take it off the wall and find a key hidden behind it!\n");
            strcpy(inventoryObject, "Key");
            hasKey = 1;
            inventoryAction(1);
            break;
        case 2:
            if (hasKey == 0)
            {
                strcpy(message, "\n The door is locked. You need to find a key first.\n");
                break;
            }
            else
            {
                strcpy(message, "\n You insert the key into the door's lock and turn it. The door creaks open.\n Congratulations! You've escaped from the room!\n");
                break;
            }
        }
    } while (input != 2);
    mainGame();
}

int main()
{
    mainGame();
    return 0;
}