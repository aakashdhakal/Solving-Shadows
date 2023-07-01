#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main()
{
    int hasKey = 0; // Variable to track if the player has the key
    char input;

    printf("Welcome to 'Escape from the Enigma Manor'!\n");
    printf("You find yourself trapped in a dimly lit room within the mysterious manor.\n");
    printf("Your first task is to find a key to unlock the door and escape.\n");

    while (1)
    {
        printf("\nWhat would you like to do?\n");
        printf("1. Look around the room\n");
        printf("2. Open the door\n");
        printf("3. Quit the game\n");

        input = getch();
        printf("%c\n", input);

        switch (input)
        {
        case '1':
            printf("You carefully search the room, examining every nook and cranny.\n");
            printf("After a thorough search, you notice a small painting on the wall that seems out of place.\n");
            printf("You take it off the wall and find a key hidden behind it!\n");
            hasKey = 1; // Player found the key
            break;

        case '2':
            if (hasKey)
            {
                printf("You insert the key into the door's lock and turn it. The door creaks open.\n");
                printf("Congratulations! You've escaped from the room!\n");
            }
            else
            {
                printf("The door is locked. You need to find a key first.\n");
            }
            break;

        case '3':
            printf("Thank you for playing 'Escape from the Enigma Manor'!\n");
            exit(0);

        default:
            printf("Invalid input. Please try again.\n");
            break;
        }
    }

    return 0;
}
