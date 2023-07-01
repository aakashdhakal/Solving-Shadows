#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000

struct choice
{
    char choice[MAX];
    struct choice *nextChoice;
};
typedef struct choice Choice;

struct story
{
    char description[MAX];
    struct story *choice1;
    struct story *choice2;
    Choice *choiceListHead;
};
typedef struct story Story;

Story *createStory(char description[])
{
    Story *newStory = (Story *)malloc(sizeof(Story));
    strcpy(newStory->description, description);
    newStory->choice1 = NULL;
    newStory->choice2 = NULL;
    newStory->choiceListHead = NULL;

    return newStory;
}

void addStory(char description[], int choice, Story *story)
{
    Story *newStory = createStory(description);
    if (choice == 1)
    {
        story->choice1 = newStory;
    }
    else if (choice == 2)
    {
        story->choice2 = newStory;
    }
}

void displayChoice(Choice *choiceListHead)
{
    Choice *temp = choiceListHead;
    int i = 1;
    while (temp != NULL)
    {
        printf("%d. %s\n", i, temp->choice);
        temp = temp->nextChoice;
        i++;
    }
}

void addChoice(char choice[], Story *story)
{
    Choice *newChoice = (Choice *)malloc(sizeof(Choice));
    strcpy(newChoice->choice, choice);
    newChoice->nextChoice = NULL;

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

void deleteChoice(char *choice, Story *story)
{
    Choice *temp = story->choiceListHead;
    Choice *prev = NULL;
    while (temp != NULL)
    {
        if (strcmp(temp->choice, choice) == 0)
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
            break;
        }
        prev = temp;
        temp = temp->nextChoice;
    }
}

void gameStory()
{
    Story *start = createStory("You wake up in a mysterious room. There are two doors in front of you. Which one do you choose?");
    addChoice("Door 1");
    addChoice("Door 2");
    start->choice = choiceHead;
}

int main()
{
    gameStory();
    playGame();

    return 0;
}