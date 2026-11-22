#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contacts.txt", "w");

    if(fp == NULL)
    {
        printf("File not opened!\n");
        return;
    }

    for(int i=0;i<addressBook->contactCount;i++)
    {
        fprintf(fp, "%s,%s,%s\n",
        addressBook->contacts[i].name,
        addressBook->contacts[i].phone,
        addressBook->contacts[i].email);
    }

    fclose(fp);
} 


void loadContactsFromFile(AddressBook *addressBook) 
{
        FILE *fp = fopen("contacts.txt", "r");

    if(fp == NULL)
        return;

    while(fscanf(fp, " %49[^,],%19[^,],%49[^\n]\n",
        addressBook->contacts[addressBook->contactCount].name,
        addressBook->contacts[addressBook->contactCount].phone,
        addressBook->contacts[addressBook->contactCount].email) == 3)
    {
        addressBook->contactCount++;
    }

    fclose(fp);
}
