#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    long double phn;
    char name[20];
    struct node *next;
};
typedef struct node Node;
Node *start = NULL;
void swap(char a[20], char b[20])
{
    char temp[20];
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}
void swaps(long double *a, long double *b)
{
    double c;
    c = *a;
    *a = *b;
    *b = c;
}

void sort()
{
    Node *temp, *pretemp;
    char p[20];
    temp = start;
    while (temp != NULL)
    {
        pretemp = temp->next;
        while (pretemp != NULL)
        {
            if (strcmp(temp->name, pretemp->name) > 0)
            {
                swap(temp->name, pretemp->name);
            }

            else if (strcmp(temp->name, pretemp->name) == 0)
            {
                if (strcmp(temp->name, pretemp->name) > 0)
                {
                    swaps(&temp->phn, &pretemp->phn);
                }
            }

            pretemp = pretemp->next;
        }
        temp = temp->next;
    }
}
void add(void)
{
    Node *newnode = (Node *)malloc(sizeof(Node));
    printf(" Enter name of person: ");
    scanf("%s", newnode->name);
    printf(" Enter phone number of the person: ");
    scanf("%lld", &newnode->phn);
    newnode->next = NULL;
    if (start == NULL)
    {
        start = newnode;
    }
    else
    {
        newnode->next = start;
        start = newnode;
    }
    printf("Contact saved successfully. \n");
}

void del(void)
{
    Node *temp;
    Node *prev = NULL;
    if (start == NULL)
    {
        printf("No contacts to delete. \n");
        return;
    }
    long double p;
    printf(" Enter phone number to delete: ");
    scanf("%lld", &p);
    temp = start;

    while (temp != NULL)
    {
        if (temp->phn == p)
        {
            if (prev == NULL)
            {
                start = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }
            free(temp);
            printf("The required contact is deleted.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Contacts not found.\n");
}

void edit(void)
{
    Node *temp;
    temp = start;
    if (start == NULL)
    {
        printf("No contacts to edit.\n");
        return;
    }
    long double p;

    printf(" Enter phone number to edit: ");
    scanf("%lld", &p);
    while (temp != NULL)
    {
        if (temp->phn == p)
        {
            printf(" Enter to change name: ");
            scanf("%s", temp->name);
            printf(" Enter to change phone number: ");
            scanf("%lld", &temp->phn);
            printf("Contact edited successfully. \n");
            return;
        }
        temp = temp->next;
    }
    printf("No contacts found.\n");
}

void display(void)
{
    sort();
    Node *temp;
    temp = start;
    if (temp == NULL)
    {
        printf("No contacts found.\n");
        return;
    }
    while (temp != NULL)
    {
        printf("Name: %s \n", temp->name);
        printf("Number: %lld \n", temp->phn);
        temp = temp->next;
    }
    printf("\n");
}

void search(void)
{
    Node *temp;
    temp = start;

    if (start == NULL)
    {
        printf("No contacts found.\n");
        return;
    }

    long double p;
    printf(" Enter phone number to search: ");
    scanf("%lld", &p);
    while (temp != NULL)
    {
        if (temp->phn == p)
        {
            printf("Name: %s \n", temp->name);
            printf("Number: %lld \n", temp->phn);
            return;
        }

        temp = temp->next;
    }
    printf("No contacts found. \n");
}

int main()
{
    int choice;
    printf("------------MENU-----------\n");
    printf(" 1. Add Contact \n 2. Delete Contact \n 3. Edit Contact \n 4. Display \n 5. Search\n 6. Exit \n");

    do
    {
        printf(" \tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add();
            break;

        case 2:
            del();
            break;

        case 3:
            edit();
            break;

        case 4:
            display();
            break;

        case 5:
            search();
            break;

        case 6:
            exit(0);
        }
    } while (choice != 6);

    return 0;
}