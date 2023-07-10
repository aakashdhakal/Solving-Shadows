#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000

// Global variables
int lives = 3;
char currentRoom[MAX];
int exitRoom = 0;
char message[MAX];

struct choice
{

    char choice[MAX];
    struct choice *nextChoice;
    struct choice *nextStory;
};
typedef struct choice Choice;
Choice *choiceHead = NULL;

struct inventory
{
    char itemName[MAX];
    struct inventory *next;
};
typedef struct inventory Inventory;
Inventory *inventoryHead = NULL;

struct story
{
    char description[MAX];
    struct story *choice1;
    struct story *choice2;
    Choice *choiceListHead;
};
typedef struct story Story;
Story *rootNode = NULL;

// Funcion to display the choices
void displayChoice(Story *node)
{
    Choice *temp = node->choiceListHead;
    int i = 1;
    while (temp != NULL)
    {
        printf("%d. %s\n", i, temp->choice);
        temp = temp->nextChoice;
        i++;
    }
}

// Function to add choices
void addChoice(Choice *listHead, char choice[])
{
    Choice *newChoice = (Choice *)malloc(sizeof(Choice));
    strcpy(newChoice->choice, choice);
    newChoice->nextChoice = NULL;
    if (listHead == NULL)
    {
        listHead = newChoice;
    }
    else
    {
        Choice *temp = listHead;
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
void addItem(char item[])
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

Story *addTreeNode(char description[])
{
    Story *node = (Story *)malloc(sizeof(Story));
    strcpy(node->description, description);
    node->choiceListHead = NULL;
    node->choice1 = NULL;
    node->choice2 = NULL;

    return node;
}

void createStory()
{
    Story *rootNode = addTreeNode("You are in a dark room. There is a door to your left and right. Which one do you take?");

    addChoice(&(rootNode->choiceListHead), "Left");
    addChoice(&(rootNode->choiceListHead), "Right");

    Story *node1 = addTreeNode("You find yourself in a forest. There is a wolf in front of you. Behind you is another door. What do you do?");
    addChoice(node1->choiceListHead, "Fight the wolf");
    addChoice(node1->choiceListHead, "Run towards the door");
    rootNode->choice1 = node1;

    Story *node2 = addTreeNode("You enter a room with a mirror. You see a shadow behind you. What do you do?");
    addChoice(node2->choiceListHead, "Face the shadow");
    addChoice(node2->choiceListHead, "Run away from the room");
    rootNode->choice2 = node2;
}

void playGame(Story *currentStory)
{
    printf("%s\n", currentStory->description);
    displayChoice(currentStory->choiceListHead);

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 2)
    {
        if (choice == 1 && currentStory->choice1 != NULL)
        {
            playGame(currentStory->choice1);
        }
        else if (choice == 2 && currentStory->choice2 != NULL)
        {
            playGame(currentStory->choice2);
        }
    }
    else
    {
        printf("Invalid choice. Please try again.\n\n");
        playGame(currentStory);
    }
}

int main()
{
    createStory();
    playGame(rootNode);

    return 0;
}