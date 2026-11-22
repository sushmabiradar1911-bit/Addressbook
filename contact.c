#include <stdio.h>    // for std I/O  functions
#include <stdlib.h>   // for exit()
#include <string.h>   // for string functions like strlen, strcmp
#include <ctype.h>    // for character checks like isalpha, isdigit
#include "contact.h"  // structure definitions
#include "file.h"     // file handling functions

/*---------------- VALIDATION FUNCTIONS ----------------*/

// Validate name: only alphabets and spaces allowed
int validName(char *name)
{
    while(*name)
    {
        if(!isalpha(*name) && *name != ' ')  // check each character
            return 0;  // invalid
        name++;
    }
    return 1;  // valid
}

// Validate phone number: must be 10 digits and start with 7/8/9
int validPhone(char *phone)
{
    if(strlen(phone) != 10) return 0;  // check length

    for(int i=0;i<10;i++)
        if(!isdigit(phone[i])) return 0;  // only digits allowed

    if(phone[0] < '7') return 0;  // first digit condition

    return 1;
}


// Validate email: only lowercase letters, one '@', at least one '.', no digits
int validEmail(char *email)
{
    int at = 0, dot = 0;

    for(int i = 0; email[i]; i++)
    {
        if(email[i] == '@')
        {
            at++;  // count '@'
            if(i == 0 || email[i+1] == '\0')  // '@' cannot be first/last
                return 0;
        }
        else if(email[i] == '.')
        {
            dot++;  // count '.'
        }
        else if(!islower(email[i]))  // only lowercase letters allowed
        {
            return 0;
        }
    }

    if(at != 1 || dot < 1)  // must have exactly 1 '@' and at least 1 '.'
        return 0;

    // ensure '.' comes after '@'
    char *atPtr = strchr(email, '@');
    char *dotPtr = strrchr(email, '.');

    if(dotPtr < atPtr)
        return 0;

    return 1;
}

// Check if phone number already exists
int isDuplicatePhone(AddressBook *ab, char *phone)
{
    for(int i=0;i<ab->contactCount;i++)
        if(strcmp(ab->contacts[i].phone, phone)==0)
            return 1;  // duplicate found
    return 0;
}

// Check if email already exists
int isDuplicateEmail(AddressBook *ab, char *email)
{
    for(int i=0;i<ab->contactCount;i++)
        if(strcmp(ab->contacts[i].email, email)==0)
            return 1;  // duplicate found
    return 0;
}

/*---------------- HELPER ----------------*/

// Convert string to lowercase (used for case-insensitive search)
void toLowerStr(char *str)
{
    for(int i=0; str[i]; i++)
        str[i] = tolower(str[i]);
}

/*---------------- LIST CONTACTS ----------------*/

// Display all contacts
void listContacts(AddressBook *addressBook) 
{
    if(addressBook->contactCount == 0)
    {
        printf("No contacts available!\n");
        return;
    }

    printf("\n---- Contact List ----\n");

    for(int i=0;i<addressBook->contactCount;i++)
    {
        printf("%d. %s | %s | %s\n", i+1,
        addressBook->contacts[i].name,
        addressBook->contacts[i].phone,
        addressBook->contacts[i].email);
    }
}

/*---------------- INITIALIZE ----------------*/

// Initialize address book and load contacts from file
void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0;
    loadContactsFromFile(addressBook);
}

/*---------------- SAVE AND EXIT ----------------*/

// Save contacts to file and exit program
void saveAndExit(AddressBook *addressBook) 
{
    saveContactsToFile(addressBook);
    exit(0);
}

/*---------------- CREATE CONTACT ----------------*/

// Create a new contact with validation and duplicate checks
void createContact(AddressBook *ab)
{
    if(ab->contactCount >= MAX_CONTACTS)
    {
        printf("Address book is full!\n");
        return;
    }

    Contact c;
    int attempts = 3;

    // Input and validate name
    while(attempts)
    {
        printf("Enter Name: ");
        scanf(" %[^\n]", c.name);

        if(validName(c.name)) break;

        attempts--;
        printf("Invalid name! Attempts left: %d\n", attempts);
    }
    if(attempts == 0) return;

    // Input and validate phone
    attempts = 3;
    while(attempts)
    {
        printf("Enter Phone: ");
        scanf(" %[^\n]", c.phone);

        if(validPhone(c.phone) && !isDuplicatePhone(ab,c.phone)) break;

        attempts--;
        printf("Invalid/Duplicate phone! Attempts left: %d\n", attempts);
    }
    if(attempts == 0) return;

    // Input and validate email
    attempts = 3;
    while(attempts)
    {
        printf("Enter Email: ");
        scanf(" %[^\n]", c.email);

        if(validEmail(c.email) && !isDuplicateEmail(ab,c.email)) break;

        attempts--;
        printf("Invalid/Duplicate email! Attempts left: %d\n", attempts);
    }
    if(attempts == 0) return;

    // Store contact in array
    ab->contacts[ab->contactCount++] = c;

    printf("Contact added successfully!\n");
}

/*---------------- SEARCH CONTACT ----------------*/

// Search contact by name, phone, or email
void searchContact(AddressBook *ab)
{
    int choice;
    char key[50];
    int found = 0;

    printf("1.Name  2.Phone  3.Email\n");
    printf("Enter choice: ");
    scanf("%d",&choice);

    if(choice < 1 || choice > 3)
    {
        printf("Invalid choice!\n");
        return;
    }

    printf("Enter value: ");
    scanf(" %[^\n]", key);

    char tempName[50], tempKey[50];

    strcpy(tempKey, key);
    toLowerStr(tempKey);  // convert search key to lowercase

    for(int i=0;i<ab->contactCount;i++)
    {
        strcpy(tempName, ab->contacts[i].name);
        toLowerStr(tempName);  // convert stored name to lowercase

        // search logic
        if((choice==1 && strstr(tempName,tempKey)!=NULL) ||
           (choice==2 && strcmp(ab->contacts[i].phone,key)==0) ||
           (choice==3 && strstr(ab->contacts[i].email,key)!=NULL))
        {
            printf("%d. %s | %s | %s\n",i+1,
            ab->contacts[i].name,
            ab->contacts[i].phone,
            ab->contacts[i].email);
            found = 1;
        }
    }

    if(!found)
        printf("Contact not found!\n");
}

/*---------------- EDIT CONTACT ----------------*/

// Edit selected contact details
void editContact(AddressBook *ab)
{
    int index;

    listContacts(ab);

    if(ab->contactCount == 0) return;

    printf("Enter serial number: ");
    scanf("%d",&index);
    index--;

    if(index<0 || index>=ab->contactCount)
    {
        printf("Invalid index!\n");
        return;
    }

    int choice;
    printf("1.Name 2.Phone 3.Email\n");
    scanf("%d",&choice);

    // Update selected field
    if(choice==1)
    {
        printf("Enter new name: ");
        scanf(" %[^\n]", ab->contacts[index].name);
    }
    else if(choice==2)
    {
        printf("Enter new phone: ");
        scanf(" %[^\n]", ab->contacts[index].phone);
    }
    else if(choice==3)
    {
        printf("Enter new email: ");
        scanf(" %[^\n]", ab->contacts[index].email);
    }
    else
    {
        printf("Invalid choice!\n");
        return;
    }

    printf("Updated successfully!\n");
}

/*---------------- DELETE CONTACT ----------------*/

// Delete a contact after confirmation
void deleteContact(AddressBook *ab)
{
    int index;

    listContacts(ab);

    if(ab->contactCount == 0) return;

    printf("Enter serial number: ");
    scanf("%d",&index);
    index--;

    if(index<0 || index>=ab->contactCount)
    {
        printf("Invalid index!\n");
        return;
    }

    char confirm;
    printf("Are you sure you want to delete? (y/n): ");
    scanf(" %c",&confirm);

    if(confirm!='y' && confirm!='Y')
    {
        printf("Delete cancelled!\n");
        return;
    }

    // shift contacts to left
    for(int i=index;i<ab->contactCount-1;i++)
    {
        ab->contacts[i] = ab->contacts[i+1];
    }

    ab->contactCount--;  // decrease count

    printf("Deleted successfully!\n");
}
