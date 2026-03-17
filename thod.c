/*Kollia Aggelikh
AM:2022202300057
dit23057@go.uop.gr

Siakwtos Thodwris
AM:2022202200189
dit22189@go.uop.gr
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Structure definitions
typedef struct {
    int year;
    int month;
    int day;
} mydate;

typedef struct {
    int hour;
    int minutes;
} mytime;

typedef struct bilList {
    char name[100];
    char city[50];
    char country[50];
    char activity[100];
    char origin_country[50];
    char gender[10];
    char account_number[15];
    double net_worth;
    int age;
    mydate birthdate;
    mytime birthtime;
    struct bilList *next;
    struct bilList *prev;
} billionaire;

// Helper functions

void formatName(char *name) {
    int i = 0;
    int capitalize_next = 1;
    
    while (name[i]) {
        if (isspace(name[i])) {
            capitalize_next = 1;
        } else if (capitalize_next && isalpha(name[i])) {
            name[i] = toupper(name[i]);
            capitalize_next = 0;
        } else {
            name[i] = tolower(name[i]);
        }
        i++;
    }
}

void formatCityCountry(char *location) {
    char *slash_pos = strchr(location, '/');
    if (slash_pos) {
        if (strncmp(location, "?", 1) == 0) {
            strncpy(location, "Unknown", slash_pos - location);
            location[7] = '\0'; // Ensure null termination
        }
        if (strcmp(slash_pos + 1, "?") == 0) {
            strcpy(slash_pos + 1, "Unknown");
        }
    }
}

void generateRandomDigits(char *buffer, int length) {
    	int i;
	for ( i = 0; i < length; i++) {
        buffer[i] = '0' + rand() % 10;
    }
    buffer[length] = '\0';
}

int calculateChecksum(char *account) {
    int sum = 0;
    int len = strlen(account);
    
   	 int i;
	for ( i = len - 1; i >= 0; i--) {
        int digit = account[i] - '0';
        if ((len - 1 - i) % 2 == 0) {
            digit *= 2;
            if (digit > 9) digit -= 9;
        }
        sum += digit;
    }
    
    int checksum = (sum * 9) % 10;
    return checksum;
}

void generateAccountNumber(char *account) {
    char part1[6], part2[8];
    generateRandomDigits(part1, 5);
    generateRandomDigits(part2, 7);
    
    sprintf(account, "%s%s", part1, part2);

    int checksum = calculateChecksum(account);
    sprintf(account, "%s-%s-%d", part1, part2, checksum);
}

// Node creation and list insertion

billionaire* createNode(char *name, char *city, char *country, char *activity, char *origin_country, char *gender, double net_worth, int age, mydate birthdate, mytime birthtime) {
    billionaire *newNode = (billionaire*)malloc(sizeof(billionaire));
    if (!newNode) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    strcpy(newNode->name, name);
    strcpy(newNode->city, city);
    strcpy(newNode->country, country);
    strcpy(newNode->activity, activity);
    strcpy(newNode->origin_country, origin_country);
    strcpy(newNode->gender, gender);
    newNode->net_worth = net_worth;
    newNode->age = age;
    newNode->birthdate = birthdate;
    newNode->birthtime = birthtime;
    newNode->next = newNode->prev = newNode;
    return newNode;
}

void insertNode(billionaire **head, billionaire *newNode) {
    if (*head == NULL) {
        *head = newNode;
    } else {
        billionaire *tail = (*head)->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = *head;
        (*head)->prev = newNode;
    }
}

void deleteNode(billionaire **head, char *name) {
    if (*head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    billionaire *current = *head;
    do {
        if (strcmp(current->name, name) == 0) {
            if (current->next == current) {
                *head = NULL;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                if (*head == current) {
                    *head = current->next;
                }
            }
            free(current);
            printf("Billionaire %s deleted\n", name);
            return;
        }
        current = current->next;
    } while (current != *head);
    
    printf("Billionaire %s not found\n", name);
}

// Display and statistics functions

void displayList(billionaire *head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    billionaire *current = head;
    do {
        printf("Name: %s, City: %s, Country: %s, Activity: %s, Origin Country: %s, Gender: %s, Net Worth: %.2f, Age: %d, Account: %s\n", 
                current->name, current->city, current->country, current->activity, current->origin_country, current->gender, current->net_worth, current->age, current->account_number);
        current = current->next;
    } while (current != head);
}

void getBasicStatistics(billionaire *head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    int count = 0;
    double total_net_worth = 0.0;
    billionaire *current = head;
    
    do {
        total_net_worth += current->net_worth;
        count++;
        current = current->next;
    } while (current != head);
    
    double average_net_worth = total_net_worth / count;
    
    printf("Total Billionaires: %d\n", count);
    printf("Total Net Worth: %.2f\n", total_net_worth);
    printf("Average Net Worth: %.2f\n", average_net_worth);
}

void getBillionairesByCountry(billionaire *head, char *country) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    billionaire *current = head;
    int found = 0;
    
    do {
        if (strcmp(current->country, country) == 0) {
            printf("Name: %s, City: %s, Country: %s, Activity: %s, Origin Country: %s, Gender: %s, Net Worth: %.2f, Age: %d, Account: %s\n", 
                    current->name, current->city, current->country, current->activity, current->origin_country, current->gender, current->net_worth, current->age, current->account_number);
            found = 1;
        }
        current = current->next;
    } while (current != head);
    
    if (!found) {
        printf("No billionaires found from %s\n", country);
    }
}

void getBillionairesByIndustry(billionaire *head, char *industry) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    billionaire *current = head;
    int found = 0;
    
    do {
        if (strcmp(current->activity, industry) == 0) {
            printf("Name: %s, City: %s, Country: %s, Activity: %s, Origin Country: %s, Gender: %s, Net Worth: %.2f, Age: %d, Account: %s\n", 
                    current->name, current->city, current->country, current->activity, current->origin_country, current->gender, current->net_worth, current->age, current->account_number);
            found = 1;
        }
        current = current->next;
    } while (current != head);
    
    if (!found) {
        printf("No billionaires found in the %s industry\n", industry);
    }
}

void getAgeDistribution(billionaire *head) {
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    int age_distribution[100] = {0}; // Assuming ages are within a range of 0-100
    
    billionaire *current = head;
    
    do {
        age_distribution[current->age]++;
        current = current->next;
    } while (current != head);
    
    printf("Age Distribution:\n");
    int i;
	for ( i = 0; i < 100; i++) {
        if (age_distribution[i] > 0) {
            printf("Age %d: %d billionaires\n", i, age_distribution[i]);
        }
    }
}

// Insert new billionaire function

void newbil(billionaire **head, char *data) {
    char name[100], city_country[100], activity[100], origin_country[50], gender[10], birth[50];
    double net_worth;
    
    sscanf(data, "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%lf", 
           name, city_country, activity, origin_country, gender, birth, &net_worth);
    
    // Format the name
    formatName(name);

    // Format the city and country
    formatCityCountry(city_country);
    
    // Split city and country
    char city[50], country[50];
    sscanf(city_country, "%[^/]/%s", city, country);
    
    // Parse and format the birthdate and birthtime
    int day, month, year, hour, minutes;
    if (strcmp(birth, "Unknown") == 0) {
        day = 1;
        month = 1;
        year = 1900;
        hour = 0;
        minutes = 0;
    } else {
        sscanf(birth, "%d/%d/%d %d:%d", &day, &month, &year, &hour, &minutes);
    }
    mydate birthdate = {year, month, day};
    mytime birthtime = {hour, minutes};
    
    // Ensure net worth is non-negative
    if (net_worth < 0) {
        net_worth = 0;
    }
    
    // Create a new node
    billionaire *newNode = createNode(name, city, country, activity, origin_country, gender, net_worth, 2024 - year, birthdate, birthtime);

    // Generate and assign a bank account number
    char account_number[15];
    generateAccountNumber(account_number);
    strcpy(newNode->account_number, account_number);

    // Insert the node into the list
    insertNode(head, newNode);
}


void fortosi_apo_arxeio(billionaire **head, char *arxeio) {
    char onoma[200], xora[50], genhthke[50], industry[50], input[256];
    mydate birthdate; 
    mytime birthtime;

    FILE *fp = fopen("project2.txt", "r");
    if (fp == NULL) {
        printf("File could not be opened\n");
        exit(1);
    }

    while (fgets(input, 256, fp)) {
        newbil(head, input);
    }
    fclose(fp);
}


// Main function

int main() {
    srand(time(0)); // Seed the random number generator

    billionaire *head = NULL;
    
    int choice;
    char input[500];  // To handle the newbil command input
    char name[100];
    char country[50];
    char industry[50];
    double net_worth;
    int age;
    mydate birthdate;
    mytime birthtime;
    
    do {
        printf("\nChoose an option:\n");
        printf("1. Insert a new billionaire\n");
        printf("2. Delete a billionaire\n");
        printf("3. View basic statistics\n");
        printf("4. View billionaires by country\n");
        printf("5. View billionaires by industry\n");
        printf("6. View age distribution\n");
        printf("7. Exit\n");
        printf("8. Read from file\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                printf("Enter the data for the new billionaire in the format (name;city/country;activity;origin_country;gender;day/month/year hour:minutes;net_worth): ");
                scanf(" %[^\n]s", input);
                newbil(&head, input);
                break;
            case 2:
                printf("Enter name of billionaire to delete: ");
                scanf(" %[^\n]s", name);
                deleteNode(&head, name);
                break;
            case 3:
                getBasicStatistics(head);
                break;
            case 4:
                printf("Enter country name: ");
                scanf(" %[^\n]s", country);
                getBillionairesByCountry(head, country);
                break;
            case 5:
                printf("Enter industry name: ");
                scanf(" %[^\n]s", industry);
                getBillionairesByIndustry(head, industry);
                break;
            case 6:
                getAgeDistribution(head);
                break;
            case 7:
                printf("Exiting...\n");
                break;
            case 8:
                scanf(" %[^\n]s", input);
                fortosi_apo_arxeio(&head, input);
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 7);
    
    return 0;
}