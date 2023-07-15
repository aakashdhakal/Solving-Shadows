#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

// Global variables
int lives = 3;
char message[MAX];
int id = 1;
int numberOfChoices;

//--------Data Structure for Story----------//

struct story
{
    char description[MAX];
    int nodeId;
    struct story *choice1;
    struct story *choice2;
    struct choice *choiceListHead;
};

typedef struct story Story;
Story *rootNode = NULL;

// Function to add story nodes
Story *addTreeNode(char description[])
{
    Story *node = (Story *)malloc(sizeof(Story));
    strcpy(node->description, description);
    node->nodeId = id++;
    node->choiceListHead = NULL;
    node->choice1 = NULL;
    node->choice2 = NULL;

    return node;
}

Story *searchNodeByID(Story *currentStory, int targetID)
{
    if (currentStory == NULL)
        return NULL;

    if (currentStory->nodeId == targetID)
        return currentStory;

    Story *foundNode = searchNodeByID(currentStory->choice1, targetID);
    if (foundNode == NULL)
        foundNode = searchNodeByID(currentStory->choice2, targetID);

    return foundNode;
}

void saveGameProgress(Story *currentStory)
{
    FILE *file = fopen("game_progress.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return;
    }
    fprintf(file, "%d\n", currentStory->nodeId);

    fclose(file);
}

Story *loadGameProgress()
{
    FILE *file = fopen("game_progress.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 0;
    }

    int targetID;
    fscanf(file, "%d", &targetID);
    fclose(file);

    return searchNodeByID(rootNode, targetID);
}

//--------Data Structure for Inventory----------//

struct inventory
{
    char itemName[MAX];
    struct inventory *next;
};
typedef struct inventory Inventory;
Inventory *inventoryHead = NULL;

// Function to display the inventory
void displayInventory()
{
    Inventory *temp = inventoryHead;
    while (temp != NULL)
    {
        printf("%s\n", temp->itemName);
        temp = temp->next;
    }
}

// Function to add items to inventory
void addItem(char itemName[])
{
    Inventory *newItem = (Inventory *)malloc(sizeof(Inventory));
    strcpy(newItem->itemName, itemName);
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
void deleteItem(char *itemName)
{
    Inventory *temp = inventoryHead;
    Inventory *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->itemName, itemName) == 0)
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

// -------Data Structure for Choices--------------//

struct choice
{
    char choice[MAX];
    struct choice *nextChoice;
    struct choice *nextStory;
};
typedef struct choice Choice;
Choice *choiceHead = NULL;

// Function to display the choices
void displayChoice(Choice *node)
{
    Choice *temp = node;
    numberOfChoices = 0;
    int i = 1;
    while (temp != NULL)
    {
        printf(" %d. %s\n", i, temp->choice);
        temp = temp->nextChoice;
        i++;
        numberOfChoices++;
    }
}

// Function to add choices
void addChoice(Choice **listHead, char choice[])
{
    Choice *newChoice = (Choice *)malloc(sizeof(Choice));
    strcpy(newChoice->choice, choice);
    newChoice->nextChoice = NULL;
    newChoice->nextStory = NULL;
    if (*listHead == NULL)
    {
        *listHead = newChoice;
    }
    else
    {
        Choice *temp = *listHead;
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

//--------Data Structure for checkpoints----------//

struct checkpoint
{
    Story *node;
    struct checkpoint *next;
};
typedef struct checkpoint Checkpoint;
Checkpoint *checkpointHead = NULL;

// push the node to checkpoint stack
void pushCheckpoint(Story *node)
{
    Checkpoint *newCheckpoint = (Checkpoint *)malloc(sizeof(Checkpoint));
    newCheckpoint->node = node;
    newCheckpoint->next = checkpointHead;
    checkpointHead = newCheckpoint;
}

void popCheckpoint()
{
    Checkpoint *temp = checkpointHead;
    checkpointHead = checkpointHead->next;
    free(temp);
}