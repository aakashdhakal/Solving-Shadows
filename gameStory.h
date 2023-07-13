#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    rootNode = addTreeNode("You are in a dark room. There is a door to your left and right. Which one do you take?");

    addChoiceToStory(rootNode, "Left");
    addChoiceToStory(rootNode, "Right");

    Story *node1 = addTreeNode("You find yourself in a forest. There is a wolf in front of you. Behind you is another door. What do you do?");
    addChoiceToStory(node1, "Fight the wolf");
    addChoiceToStory(node1, "Run towards the door");
    rootNode->choice1 = node1;

    Story *node2 = addTreeNode("You enter a room with a mirror. You see a shadow behind you. What do you do?");
    addChoiceToStory(node2, "Face the shadow");
    addChoiceToStory(node2, "Run away from the room");
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