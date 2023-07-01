#include <stdio.h>
#include <stdlib.h>

struct inventory
{
    char itemName[20];
    struct inventory *next;
};
typedef struct inventory Inventory;
Inventory *inventoryHead = NULL;
char inventoryObject[20];

void addToInventory()
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

void deleteFromInventory()
{
    Inventory *temp = inventoryHead;
    Inventory *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->itemName, inventoryObject) == 0)
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
            strcpy(inventoryObject, "");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}