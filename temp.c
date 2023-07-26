#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define MAX 1000

// Global variables
int lives = 3;
char message[MAX];
int id = 1;
char inventory[MAX];
int numberOfChoices;

struct choice
{
    char choice[MAX];
    int choiceNumber;
    int isStoryNext;
    char inventoryItem[MAX];
    char message[MAX];
    int isLifeDecrease;
    int isCompleted;
    struct choice *nextChoice;
};

typedef struct choice Choice;

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

void playGame(Story *);
void startScreen();
void helpScreen();

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
    int i = 1;
    temp->choiceNumber = 1;
    numberOfChoices = 0;
    printf("\n");
    while (temp != NULL)
    {
        temp->choiceNumber = i;
        printf(" %d. %s\n", temp->choiceNumber, temp->choice);
        temp = temp->nextChoice;
        i++;
        numberOfChoices++;
    }
}

// Function to add choices
void addChoice(Story *story, char choice[], int proceed, char message[], char inventoryItem[], int isLifeDecrease)
{
    Choice *newChoice = (Choice *)malloc(sizeof(Choice));
    strcpy(newChoice->choice, choice);
    newChoice->nextChoice = NULL; // Set the nextChoice pointer to NULL for the last choice
    newChoice->isStoryNext = proceed;
    strcpy(newChoice->message, message); // Initialize the message to an empty string
    strcpy(newChoice->inventoryItem, inventoryItem);
    newChoice->isLifeDecrease = isLifeDecrease;
    newChoice->isCompleted = 0;

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
void deleteChoice(Story *story, int choiceNumber)
{
    Choice *temp = story->choiceListHead;
    Choice *prev = NULL;
    int i = 1;
    while (temp->nextChoice != NULL)
    {
        if (temp->choiceNumber == choiceNumber)
        {
            if (prev == NULL)
            {
                story->choiceListHead = temp->nextChoice;
            }
            else
            {
                prev->nextChoice = temp->nextChoice;
            }

            free(temp);
            return;
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
        printf("%s |", temp->itemName);
        temp = temp->next;
    }

    if (inventoryHead == NULL)
    {
        printf("Empty");
    }
}
char *searchItem(char *item)
{
    Inventory *temp = inventoryHead;
    while (temp != NULL)
    {
        if (temp->itemName == item)
        {
            return item;
        }
    }

    return "";
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

void gameOver()
{
    system("cls");
    printf(R"EOF(

                                 ____                         ___                 
                                / ___| __ _ _ __ ___   ___   / _ \__   _____ _ __ 
                               | |  _ / _  |  _   _ \ / _ \ | | | \ \ / / _ \ '__|
                               | |_| | (_| | | | | | |  __/ | |_| |\ V /  __/ |   
                                \____|\__,_|_| |_| |_|\___|  \___/  \_/ \___|_|   
                                                    
                                             
)EOF");
    vline('-', 120);
    printf(" !!! %s !!!\n", message);
    printf("\n 1. New Game\n 2. Reload last checkpoint\n 3. Quit to Main Menu\n");
    int choice;
    printf("\n Enter your choice: ");
    scanf("%d", &choice);
    switch(choice){
    	case 1: playGame(rootNode);
    			break;
    	case 2: playGame(checkpointHead->node);
    			break;
    	case 3: startScreen();
    			break;
	}
}

// Function to update the status bar
void updateConsole()
{
    //    printf("\033[2J\033[1;1H");
    system("cls");
    if (lives == 0)
    {
        strcpy(message, " You have lost all your lives");
        gameOver();
    }
    vline('-', 120);
    printf("  Inventory: ");
    displayInventory();

    printf("\n\n  Lives: %d", lives);
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

    addChoice(rootNode, " Search the room", 0, " You find a key", "Key", 0);
    addChoice(rootNode, " Open the door", 1, " You need a key to open the door", "Key", 0);

    Story *node1 = addTreeNode(" You find yourself in a forest. There is a wolf in front of you. Behind you is another door. What do you do?");
    addChoice(node1, " Fight the wolf", 0, " You fought the wolf and lost. You lost a life", "", 1);
    addChoice(node1, " Run towards the door", 1, " You need a key to open the door", "", 0);
    rootNode->choice1 = node1;

    Story *node2 = addTreeNode(" You enter a room with a mirror. You see a shadow behind you. What do you do?");
    addChoice(node2, " Smash the mirror", 0, " You found key in the mirror", "Key", 0);
    addChoice(node2, " Run away from the room", 1, "", "Key", 0);
    node1->choice1 = node2;
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

    fprintf(file, "%d\t%d ", currentStory->nodeId, lives);
    Inventory *temp = inventoryHead;
    while (temp != NULL)
    {
        fprintf(file, "\t%s ", temp->itemName);
        temp = temp->next;
    }

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
    char inventoryItem[MAX];

    fscanf(file, "%d\t%d", &targetID, &lives);
    while (fscanf(file, "\t%s", inventoryItem) != EOF)
    {
        addItem(inventoryItem);
    }

    fclose(file);

    return searchNodeByID(rootNode, targetID);
}

void playGame(Story *currentStory)
{
    Choice *userChoice;
    Story *nextStory = NULL;
    int choice;
    int canProceed = 0;
    do
    {
        updateConsole();
        printf("%s\n", currentStory->description);
        userChoice = currentStory->choiceListHead;
        if (message[0] != '\0')
        {
            printf("\n !!! %s !!!\n", message);
            message[0] = '\0';
        }

        displayChoice(userChoice);
        printf("\n Enter your choice: ");
        scanf("%d", &choice);
        if (choice < 1 || choice > numberOfChoices)
        {
            strcpy(message, " Invalid choice");
        }
        else
        {
            while (userChoice != NULL)
            {
                if (userChoice->choiceNumber == choice)
                {
                    break;
                }

                userChoice = userChoice->nextChoice;
            }
            if (userChoice->isCompleted == 1)
            {
                if (userChoice->isLifeDecrease != 1)
                {
                    strcpy(message, "You have already completed this action");
                    continue;
                }
            }

            if (userChoice->isStoryNext == 1 || userChoice->isStoryNext == 2)
            {

                if (userChoice->isStoryNext == 1)
                {
                    nextStory = currentStory->choice1;
                }
                else
                {
                    nextStory = currentStory->choice2;
                }
                deleteItem(userChoice->inventoryItem);
            }
            else
            {

                strcpy(message, userChoice->message);
                if (userChoice->inventoryItem[0] != '\0')
                {
                    addItem(userChoice->inventoryItem);
                }

                if (userChoice->isLifeDecrease == 1)
                {
                    lives--;
                }
                userChoice->isCompleted = 1;
            }
        }
        canProceed = 1;
    } while (userChoice->isStoryNext == 0 || canProceed == 0);
    if (nextStory == NULL)
    {
        strcpy(message, " You have completed the game");
        gameOver();
        saveGameProgress(rootNode);
        return;
    }

    pushCheckpoint(currentStory);
    saveGameProgress(nextStory);
    playGame(nextStory);
}

void helpScreen()
{
    system("cls");
    printf(" Help\n");
    vline('-', 120);
    printf("Use the number keys to select the choices\n");
    printf("Press any key to go back to the main menu\n");
    getch();
    startScreen();
}

void startScreen()
{
    system("cls");
    printf(R"EOF(

                                 _____ _           _             _____                      
                                / ____| |         (_)           / ____|                     
                               | |    | |__   ___  _  ___ ___  | |  __  __ _ _ __ ___   ___ 
                               | |    | '_ \ / _ \| |/ __/ _ \ | | |_ |/ _` | '_ ` _ \ / _ \
                               | |____| | | | (_) | | (_|  __/ | |__| | (_| | | | | | |  __/
                                \_____|_| |_|\___/|_|\___\___|  \_____|\__,_|_| |_| |_|\___|
                                                              
)EOF");
    vline('-', 120);

    printf(" 1. Resume Game\n 2. New Game\n 3. Help\n 4. Quit\n");
    int choice;
    printf("\n Enter your choice: ");
    scanf("%d", &choice);
    FILE *file = fopen("game_progress.txt", "r");
    switch(choice){
    	case 1: 
        		if (file != NULL) {
            		Story *currentStory = loadGameProgress();
            		fclose(file);
            		playGame(currentStory);
        		}
        		else {
            		printf("\n No saved file found");
        		}
        case 2: playGame(rootNode);
        		break;
        		
        case 3: helpScreen();
        		break;
        		
        case 4: exit(0);
	}
}

int main()
{
    createStory();
    startScreen();
    return 0;
}
