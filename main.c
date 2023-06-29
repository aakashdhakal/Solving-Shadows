#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

char inventoryObject[20];
int lives = 3;
char currentRoom[20];

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
    printf("\n You awaken in a dimly lit room, disoriented and unsure of how you ended up here.");
    printf(" The air is heavy with an eerie silence broken only by the faint sound of whispers echoing through the mansion's halls.");
    printf(" As you try to gather your thoughts, you realize that you are trapped inside a mysterious mansion, its secrets shrouded  in darkness.");
    printf(" The room you find yourself in is adorned with faded wallpaper and antique furniture, a mirror on the wall reflecting your bewildered expression.");
    printf(" \n Your first instinct is to escape this eerie place, to uncover the truth behind its enigmatic presence.\n");
    printf("\n");
}

int main()
{
    gameDetails();
    mainGame();
    return 0;
}
