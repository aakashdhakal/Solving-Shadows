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
void gameTitle();

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
        printf(" %d. %s\n\n", temp->choiceNumber, temp->choice);
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
        printf(" %s |", temp->itemName);
        temp = temp->next;
    }

    if (inventoryHead == NULL)
    {
        printf("Empty");
    }
}
// Function to check if an item is present in the inventory
int isItemPresent(char *item)
{
    Inventory *temp = inventoryHead;
    while (temp != NULL)
    {
        if (strcmp(temp->itemName, item) == 0)
        {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
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

    
                            ___   ___  ___  ___  ____      ___   __ __  ____ ____ 
                           // \\ // \\ ||\\//|| ||        // \\  || || ||    || \\
                          (( ___ ||=|| || \/ || ||==     ((   )) \\ // ||==  ||_//
                           \\_|| || || ||    || ||___     \\_//   \V/  ||___ || \\

                                                         
                                      
)EOF");
    vline('-', 120);
    printf(" !!! %s !!!\n", message);
    printf("\n 1. New Game\n\n 2. Reload last checkpoint\n\n 3. Quit to Main Menu\n");
    int choice;
    lives = 3;
    strcpy(message, "");
    printf("\n Enter your choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        playGame(rootNode);
        break;
    case 2:
        playGame(checkpointHead->node);
        break;
    case 3:
        startScreen();
        break;
    }
}

// Function to update the status bar
void updateConsole()
{
    system("cls");

    if (lives == 0)
    {
        strcpy(message, " You have lost all your lives");
        gameOver();
    }
    gameTitle();
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
    rootNode = addTreeNode(" You hear ticking of clock. You slowly open your eyes and find yourself in a dark room. There is nothing instead of\n broken plasters of wall, a door which is locked and the ticking clock. You are unaware about who you are and where\n you are. ");
    addChoice(rootNode, "Look around the room", 0, "You found a brick lying on the floor", "Brick", 0);
    addChoice(rootNode, "Look under the bed", 0, "You found a gun under the bed", "Gun", 0);
    addChoice(rootNode, "Break the door", 1, "You can't break door with hand", "Brick", 0);

    Story *node1 = addTreeNode(" After breaking the door you exit the room and enter a big hallway. the hallway is mostly empty and a small bulb is\n providing light enough to navigate around the room. At the end of the hallway there are two doors one is rusted and\n another looks like it is just bought and attached. ");
    addChoice(node1, "Open the rusted door", 1, "", "", 0);
    addChoice(node1, "Open the new door", 2, "", "", 0);
    rootNode->choice1 = node1;

    Story *node2 = addTreeNode(" You open the door and you saw a strange shadow standing infront of you. You are afraid and try to run away but the door was already closed and locked. You looked around and see a piece of steel pipe. ");
    addChoice(node2, "Pick up the steel pipe", 0, "You picked up the pipe", "Pipe", 0);
    addChoice(node2, "Beat the shadow", 1, "The shadow had a gun and it shot you", "Pipe", 1);
    node1->choice1 = node2;

    Story *node3 = addTreeNode(" You escaped the shadow but when you were running you saw something familiar about the shadow. While running suddenly\n the floor breaks and you fall down. It looks like you are in someone's bedroom. The only way out was a vent on the\n bottom of the wall. ");
    addChoice(node3, "Look around the room", 0, "You found a key under the bed", "Screwdriver", 0);
    addChoice(node3, "Open the vent", 1, "You cannot open the vent without a screwdriver", "Screwdriver", 0);
    node2->choice1 = node3;

    Story *node4 = addTreeNode(" You open the vent and crawl through it. After crawling for some time you see a light coming from the end of the vent.\n You crawl towards the light and you arrive a room with a rusted door");
    addChoice(node4, "Open the door", 1, "", "", 0);
    node3->choice1 = node4;

    Story *node5 = addTreeNode("When you were going to open the door a person suddenly opens the door ");
    addChoice(node5, "Shoot Him", 1, "You don't have a gun so the person beat you with pipe", "Gun", 1);
    addChoice(node5, "Retry and start again", 2, "", "", 0);
    node4->choice1 = node5;
    node5->choice2 = rootNode;

    Story *node12 = addTreeNode(" Before you shoot him he hit you with a pipe and you fall down. After some time you wake up in the same room where you started");
    addChoice(node12, "Continue", 1, "", "", 0);
    node5->choice1 = node12;

    Story *node6 = addTreeNode(" You open the new door and you found yourself in some futuristic room. There is a computer on the table and a window.\n You look outside the window and you saw a big city. You are confused and afraid. You look around and see a gun on\n the table. ");
    addChoice(node6, "Look at the computer", 1, "", "", 0);
    node1->choice2 = node6;

    Story *node7 = addTreeNode(" You look at the computer and you found a message. The message says \"You are in a simulation. You are the only one who\n can save the world. You have to find the key to exit the simulation. The key is hidden in the room. You have to\n find it and exit the simulation");
    addChoice(node7, "Look around the room", 1, "", "", 0);
    node6->choice1 = node7;

    Story *node8 = addTreeNode(" You found a safe in a corner. There is a message in the safe that says 'If you get any incorrect 5 digits password\n combination then you will die. So you look around the room and found a note on the table. The note says 'If we place an\n extra numeral 1 at the beginning, we get a number three times smaller than if we put that numeral 1 at the end of the\n number'. So what is the password?");
    addChoice(node8, "12345", 0, "You entered the wrong password and you died", "", 1);
    addChoice(node8, "82374", 0, "You entered the wrong password and you died", "", 1);
    addChoice(node8, "42857", 1, "You entered the wrong password and you died", "", 0);
    addChoice(node8, "14285", 0, "You entered the wrong password and you died", "", 1);
    addChoice(node8, "28571", 0, "You entered the wrong password and you died", "", 1);
    addChoice(node8, "57142", 0, "You entered the wrong password and you died", "", 1);
    node7->choice1 = node8;

    Story *node9 = addTreeNode(" The password is correct and the safe opens. You found a key in the safe. You take the key and exit the room. You\n are back in the hallway. You look around and see a door with a lock. You try to open the door with the key and\n the door opens. You enter the room and you see a big computer.");
    addChoice(node9, "Try to exit the simulation", 1, "", "", 0);
    addChoice(node9, "Destroy the computer with gun", 2, "You don't have gun right now", "Gun", 0);
    node8->choice1 = node9;

    Story *node10 = addTreeNode("You try to exit the simulation but you suddenly passed out and you wake up ine same room where you started");
    addChoice(node10, "Continue", 1, "", "", 0);
    node9->choice1 = node10;

    Story *node11 = addTreeNode("You destroy the computer and suddenly you passed out and you wake up in same room where you started");
    addChoice(node11, "Continue", 1, "", "", 0);
    node9->choice2 = node11;
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
    pushCheckpoint(currentStory);

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
            continue;
        }
        while (userChoice != NULL)
        {
            if (userChoice->choiceNumber == choice)
            {
                break;
            }
            userChoice = userChoice->nextChoice;
        }
        if (userChoice->isLifeDecrease == 1)
        {
            strcpy(message, userChoice->message);
            lives--;
        }

        if (userChoice->isCompleted == 1 && userChoice->isLifeDecrease != 1)
        {
            strcpy(message, "You have already completed this action");
            continue;
        }

        if (userChoice->isStoryNext == 1 || userChoice->isStoryNext == 2)
        {
            if (strcmp(userChoice->inventoryItem, "") != 0)
            {
                if (!isItemPresent(userChoice->inventoryItem))
                {
                    sprintf(message, "You need %s to complete this action", userChoice->inventoryItem);
                    continue;
                }
                deleteItem(userChoice->inventoryItem);
            }

            if (userChoice->isStoryNext == 1)
            {
                nextStory = currentStory->choice1;
            }
            else
            {
                nextStory = currentStory->choice2;
            }
        }
        else
        {
            strcpy(message, userChoice->message);
            if (userChoice->inventoryItem[0] != '\0')
            {
                addItem(userChoice->inventoryItem);
            }

            userChoice->isCompleted = 1;
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
void gameTitle()
{
    printf(R"EOF(
                                         ____  __    ___  ___  ____   ____
                                        ||    (( \  //   // \\ || \\ ||   
                                        ||==   \\  ((    ||=|| ||_// ||== 
                                        ||___ \_))  \\__ || || ||    ||___


)EOF");
    vline('-', 120);
}

void startScreen()
{
    system("cls");
    gameTitle();
    printf(" \n 1. Resume Game\n\n 2. New Game\n\n 3. Help\n\n 4. Quit\n\n");
    int choice;
    printf("\n Enter your choice: ");
    scanf("%d", &choice);
    FILE *file = fopen("game_progress.txt", "r");
    switch (choice)
    {
    case 1:
        if (file != NULL)
        {
            fseek(file, 0, SEEK_END); // move to end of file
            if (ftell(file) == 0)     // if position is still at the beginning, the file is empty
            {
                vline('-', 120);
                printf("\nNo saved file found\nPress any key to go back to the main menu");
                getch();
                startScreen();
            }
            else
            {
                rewind(file); // move back to the start of the file
                Story *currentStory = loadGameProgress();
                fclose(file);
                playGame(currentStory);
            }
        }
        else
        {
            printf("\n An error occured while opening the file");
        }
    case 2:
        playGame(rootNode);
        break;

    case 3:
        helpScreen();
        break;

    case 4:
        exit(0);
    }
}

int main()
{
    createStory();
    startScreen();
    return 0;
}
