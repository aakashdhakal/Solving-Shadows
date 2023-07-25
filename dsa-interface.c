#include <stdio.h>

void vline(char ch, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%c", ch);
    }
    printf("\n");
}

int main()
{

    printf(R"EOF(

                                 _____ _           _             _____                      
                                / ____| |         (_)           / ____|                     
                               | |    | |__   ___  _  ___ ___  | |  __  __ _ _ __ ___   ___ 
                               | |    | '_ \ / _ \| |/ __/ _ \ | | |_ |/ _` | '_ ` _ \ / _ \
                               | |____| | | | (_) | | (_|  __/ | |__| | (_| | | | | | |  __/
                                \_____|_| |_|\___/|_|\___\___|  \_____|\__,_|_| |_| |_|\___|
                                                              
)EOF");
    vline('-', 120);

    printf(" 1. Play Game\n 2. Help\n 3. Quit Game\n");
}