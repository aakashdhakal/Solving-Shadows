#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct story
{
    char description[1000];
    struct story *choice1;
    struct story *choice2;
    struct story *choice3;
    struct story *choice4;
    struct story *choice5;
};
typedef struct story Story;
Story *storyHead = NULL;

Story *createStory(char description[1000])
{
    Story *newStory = (Story *)malloc(sizeof(Story));
    strcpy(newStory->description, description);
    newStory->choice1 = NULL;
    newStory->choice2 = NULL;
    newStory->choice3 = NULL;
    newStory->choice4 = NULL;
    newStory->choice5 = NULL;

    return newStory;
}
