#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct story
{
    char description[1000];
    struct story *choice1;
    struct story *choice2;
};
typedef struct story Story;
Story *storyHead = NULL;

Story *createStory(char description[1000])
{
    Story *newStory = (Story *)malloc(sizeof(Story));
    strcpy(newStory->description, description);
    newStory->choice1 = NULL;
    newStory->choice2 = NULL;

    return newStory;
}

void addStory(char description[1000], int choice, Story *story)
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

void displayStory(Story *story)
{
    printf("%s\n", story->description);
    if (story->choice1 != NULL)
    {
        printf("1. %s\n", story->choice1->description);
    }
    if (story->choice2 != NULL)
    {
        printf("2. %s\n", story->choice2->description);
    }
}
