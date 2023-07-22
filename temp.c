#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

// Global variables
int lives = 3;
char currentRoom[MAX];
int exitRoom = 0;
char message[MAX];
int id = 1;
int choiceId = 1;
char inventory[MAX];
int numberOfChoices;

struct choice
{
    char choice[MAX];
    int choiceId;
    struct choice *nextChoice;
    int proceedNext;

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
    int nodeId;
    struct story *choice1;
    struct story *choice2;
    Choice *choiceListHead;
};
typedef struct story Story;
Story *rootNode = NULL;

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
void addChoice(Story *story, char choice[],int proceed)
{
    Choice *newChoice = (Choice *)malloc(sizeof(Choice));
    strcpy(newChoice->choice, choice);
    newChoice->nextChoice = NULL;
    newChoice->choiceId = choiceId;
    newChoice->proceedNext = proceed;
    choiceId++;

    if (story->choiceListHead == NULL)
    {
        story->choiceListHead = newChoice;
    }
    else
    {
        Choice *temp = story->choiceListHead;
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

// Function to add items to inventory
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

// Function to display a horizontal line
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
    system("cls");
    vline('-', 120);
    printf("  Inventory: ");
    displayInventory();
    printf("\t\t\t\t\t\t\t\t\t\t\t       Lives: %d", lives);
    printf("\n");
    vline('-', 120);
}

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


void createStory()
{
    rootNode = addTreeNode(" You are in a dark room. There is a door to your left and right. What will you do?");

    addChoice(rootNode, " Search the room",0);
    addChoice(rootNode, " Open the door",1);

    Story *node1 = addTreeNode(" You find yourself in a forest. There is a wolf in front of you. Behind you is another door. What do you do?");
    addChoice(node1, " Fight the wolf",0);
    addChoice(node1, " Run towards the door",1);
    rootNode->choice1 = node1;
    

    Story *node2 = addTreeNode(" You enter a room with a mirror. You see a shadow behind you. What do you do?");
    addChoice(node2, " Face the shadow",0);
    addChoice(node2, " Run away from the room",1);
    rootNode->choice2 = node2;
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

void playGame(Story *currentStory)
{
    updateConsole();
    printf("%s\n", currentStory->description);
    Choice *userChoice = currentStory->choiceListHead;
    displayChoice(userChoice);
    Story *nextStory = NULL;
    
    int choice;

    do
    {
        printf("\n Enter your choice: ");
        scanf("%d", &choice);
        if (choice < 1 || choice > numberOfChoices)
        {
            printf("Invalid choice. Please enter again\n");
        }
        else
        {
        	
        	
			while(userChoice != NULL){
        		if(choice = userChoice->choiceId){
        			break;
				}
				userChoice = userChoice->nextChoice;
			}
			
            if (choice == 1)
            {
                nextStory = currentStory->choice1;
            }
            else
            {
                nextStory = currentStory->choice2;
            }
        }
    } while (userChoice->proceedNext == 1);
    if (nextStory == NULL)
    {
        printf("Game Over\n");
        saveGameProgress(rootNode);
        return;
    }
    pushCheckpoint(currentStory);
    saveGameProgress(nextStory);
    playGame(nextStory);
}

int main()
{
    createStory();
    FILE *file = fopen("game_progress.txt", "r");
    if (file != NULL)
    {
        Story *currentStory = loadGameProgress();
        playGame(currentStory);
    }
    else
    {
        playGame(rootNode);
    }

    return 0;
}
