#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

char inventoryObject[20];

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

void inventoryAction(int n)
{
    if (n == 1)
    {
        Inventory *items = (Inventory *)malloc(sizeof(Inventory));
        strcpy(items->itemName, "Knife");
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

        printf("%s added to inventory\n", items->itemName);
    }
}
void gameDetails()
{
    Inventory *items = inventoryHead;
    vline('-', 120);
    printf("Inventory: ");
    while (items != NULL)
    {
        printf("%s ", items->itemName);
        items = items->next;
    }
    printf("\n");
    vline('-', 120);
}

int main()
{
    inventoryAction(1);
    gameDetails();
    return 0;
}