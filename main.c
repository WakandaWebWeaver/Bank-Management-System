/*
   A C program to simulate a banking experience - Built by Esvin Joshua
    This program uses the cJSON library to store data in JSON format.
    The program uses a JSON file to store the data.

   Features:
    1. Create new account
    2. Check balance
    3. Deposit
    4. Withdraw
    5. Change pin
    6. Logout
    7. View details
    8. Delete account

    Security features:
    1. Account number is randomly generated
    2. Account number is unique
    3. Pin is required to withdraw money > 1000
    4. Pin is required to change pin
    5. Pin is required to delete account
    6. Account cannot be deleted if balance is greater than 0

    Functions and explanations:
    1. welcome() - Displays a welcome message
    2. login() - Logs in the user
    3. menu() - Displays the menu
    4. newAccount() - Creates a new account
    5. checkBalance() - Checks the balance of the user
    6. deposit() - Deposits money into the user's account
    7. withdraw() - Withdraws money from the user's account
    8. changePin() - Changes the pin of the user's account
    9. viewDetails() - Displays the details of the user's account
    10. deleteAccount() - Deletes the user's account
    11. saveToFile() - Saves the JSON object to a file
    12. loadFromFile() - Loads the JSON object from a file

    Highlights:
    1. Uses cJSON library and JSON files to store data unlike traditional text files
    2. Uses a random number generator to generate account numbers
    4. Uses a function to check if an account number exists in the array
    5. User login is based on account number, not name
   */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.c"
#include <time.h>
#include "cJSON.h"


#define MAX_NAME_LENGTH 40
#define MAX_ADDRESS_LENGTH 50
#define MAX_PHONE_LENGTH 15
#define MAX_PIN_LENGTH 6
#define ACCOUNT_NUMBER_LENGTH 9
#define JSON_FILE "accounts.json"


typedef struct {
    char name[MAX_NAME_LENGTH];
    char country[MAX_ADDRESS_LENGTH];
    char state[MAX_ADDRESS_LENGTH];
    char city[MAX_ADDRESS_LENGTH];
    char street[MAX_ADDRESS_LENGTH];
    char houseNumber[MAX_ADDRESS_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char pin[MAX_PIN_LENGTH];
    char accountNumber[ACCOUNT_NUMBER_LENGTH];
    double balance;
} Account;

void welcome();
void login(Account *user, cJSON *json);
void menu(Account *user, cJSON *json);
void newAccount(cJSON *json);
void checkBalance(const Account *user, cJSON *json);
void deposit(Account *user, cJSON *json);
void withdraw(Account *user, cJSON *json);
void changePin(Account *user, cJSON *json);
void viewDetails(const Account *user, cJSON *json);
void deleteAccount(Account *user, cJSON *json);
void saveToFile(const cJSON *json, const char *filename);
void delay(int number_of_seconds);
int randomNumber(cJSON *json);
int accountNumberExists(int num, cJSON *accounts, int size);
cJSON *loadFromFile(const char *filename);

int main() {
    welcome();
    cJSON *json = loadFromFile("accounts.json");
    if (json == NULL) {
        json = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "accounts", cJSON_CreateArray());
    }

    Account currentUser;
    login(&currentUser, json);
    menu(&currentUser, json);

    cJSON_Delete(json);

    return 0;
}

void welcome() {
    printf("Welcome to The Bank\n");
    // system("clear"); // For Windows, use "cls".
}

void login(Account *user, cJSON *json) {
    char* currentUser;

    printf("Enter your account number: ");
    scanf("%s", user->accountNumber);

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");

    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const int accNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;
            const char *securityQuestion = cJSON_GetObjectItem(account, "securityQuestion")->valuestring;
            const char *securityAnswer = cJSON_GetObjectItem(account, "securityAnswer")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;

            if (accNumber == strtol(user->accountNumber, &currentUser, 10)) {
                strcpy(user->name, name);
                printf("Enter your password or type 'forgot' to recover it: ");
                scanf("%s", user->pin);

                if (strcmp(user->pin, "forgot") == 0) {
                    printf("Security question: %s\n", securityQuestion);
                    printf("Answer: ");
                    char secuAnswer[MAX_NAME_LENGTH];
                    scanf("%s", secuAnswer);
                    if (strcmp(secuAnswer, securityAnswer) == 0) {
                        printf("Your password is: %s\n", pin);
                        delay(1);
                        system("clear"); // For Windows, use "cls".
                        login(user, json);
                    } else {
                        printf("Incorrect answer\n");
                        delay(1);
                        system("clear"); // For Windows, use "cls".
                        login(user, json);
                    }
                }
                if (strcmp(user->pin, pin) == 0) {
                    printf("Login successful\n");
                    delay(1);
                    // system("clear"); // For Windows, use "cls".
                    return;
                } else {
                    printf("Incorrect pin\n");
                    delay(1);
                    system("clear"); // For Windows, use "cls"
                    login(user, json);
                }
            }

        }
    }

    printf("User does not exist. Do you want to create a new account? (yes or no): ");
    char *createNewAccount = malloc(sizeof(char) * 3);
    scanf("%s", createNewAccount);

    if (strcmp((const char *) createNewAccount, "yes") == 0) {
        printf("Creating new account...\n");
        newAccount(json);
        saveToFile(json, "accounts.json");
        login(user, json); // Reattempt login after creating the account
    } else {
        printf("Login failed\n");
        delay(1);
        // system("clear"); // "cls".
        login(user, json);
    }
}

void menu(Account *user, cJSON *json) {
    int choice;
    do {
        printf("\n---------------------\n");
        printf("1. Create new account\n");
        printf("2. Check balance\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Change pin\n");
        printf("6. Logout\n");
        printf("7. View details\n");
        printf("8. Delete account\n");
        printf("---------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                newAccount(json);
                break;
            case 2:
                checkBalance(user, json);
                break;
            case 3:
                deposit(user, json);
                break;
            case 4:
                withdraw(user, json);
                break;
            case 5:
                changePin(user, json);
                break;
            case 6:
                printf("Logged out successfully\n");
                delay(1);
                system("clear"); // For Windows, use "cls".
                login(user, json);
                break;
            case 7:
                viewDetails(user, json);
                break;
            case 8:
                deleteAccount(user, json);
                saveToFile(json, "accounts.json");
                login(user, json); // Reattempt login after deleting the account
                break;
            default:
                printf("Invalid choice\n");
                delay(1);
                // system("clear"); // For Windows, use "cls".
                break;
        }
    } while (choice != 9);
}

void newAccount(cJSON *json) {
    Account newAccount;
    int accountNumber = randomNumber(json);
    char securityAnswer[MAX_NAME_LENGTH];
    char securityQuestion[MAX_NAME_LENGTH];


    printf("\n---------------------\n");
    printf("Enter your name: ");
    scanf("%s", newAccount.name);
    printf("Enter your country: ");
    scanf("%s", newAccount.country);
    printf("Enter your state: ");
    scanf("%s", newAccount.state);
    printf("Enter your city: ");
    scanf("%s", newAccount.city);
    printf("Enter your street: ");
    scanf("%s", newAccount.street);
    printf("Enter your house number (without spaces): ");
    scanf("%s", newAccount.houseNumber);
    printf("Enter your phone number: ");
    scanf("%s", newAccount.phone);
    printf("Enter a pin for your account: ");
    scanf("%s", newAccount.pin);
    printf("Enter your balance: ");
    scanf("%lf", &newAccount.balance);
    getchar();
    printf("Please enter a security question that you will be able to answer in case you forget your pin: ");
    fgets(securityQuestion, MAX_NAME_LENGTH, stdin);
    printf("Please answer your security question: %s: ", securityQuestion);
    fgets(securityAnswer, MAX_NAME_LENGTH, stdin);
    securityAnswer[strcspn(securityAnswer, "\n")] = '\0';
    printf("Your account number is: %d\nPlease copy or remember it.", accountNumber);
    printf("\n---------------------\n");

    cJSON *accountObject = cJSON_CreateObject();
    cJSON_AddStringToObject(accountObject, "name", newAccount.name);
    cJSON_AddStringToObject(accountObject, "country", newAccount.country);
    cJSON_AddStringToObject(accountObject, "state", newAccount.state);
    cJSON_AddStringToObject(accountObject, "city", newAccount.city);
    cJSON_AddStringToObject(accountObject, "street", newAccount.street);
    cJSON_AddStringToObject(accountObject, "houseNumber", newAccount.houseNumber);
    cJSON_AddStringToObject(accountObject, "phone", newAccount.phone);
    cJSON_AddStringToObject(accountObject, "pin", newAccount.pin);
    cJSON_AddStringToObject(accountObject, "securityQuestion", securityQuestion);
    cJSON_AddStringToObject(accountObject, "securityAnswer", securityAnswer);
    cJSON_AddNumberToObject(accountObject, "accountNumber", accountNumber);
    cJSON_AddNumberToObject(accountObject, "balance", newAccount.balance);

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (accounts == NULL) {
        accounts = cJSON_CreateArray();
        cJSON_AddItemToObject(json, "accounts", accounts);
    }

    cJSON_AddItemToArray(accounts, accountObject);

    printf("\nAccount created successfully\n");
    delay(1);
    // system("clear"); // For Windows, use "cls".
}

void checkBalance(const Account *user, cJSON *json) {
    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const int accNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->pin, pin) == 0 && accNumber == strtol(user->accountNumber, NULL, 10)) {
                printf("\n---------------------\n");
                printf("Your balance is %.2lf\n", balance);
                printf("---------------------\n");
                delay(1);
                system("clear"); // For Windows, use "cls".
                return;
            }
        }
    }

    printf("Account not found\n");
    delay(1);
    // system("clear"); // For Windows, use "cls".
}

void deposit(Account *user, cJSON *json) {
    double amount;
    printf("Enter the amount you want to deposit: ");
    scanf("%lf", &amount);

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const int accNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->pin, pin) == 0 && accNumber == strtol(user->accountNumber, NULL, 10)) {
                cJSON_SetNumberValue(cJSON_GetObjectItem(account, "balance"), balance + amount);
                saveToFile(json, JSON_FILE);
                printf("Amount deposited successfully\n");
                return;
            }
        }
    }

    printf("Account not found\n");
}

void withdraw(Account *user, cJSON *json) {
    double amount;
    printf("Enter the amount you want to withdraw: ");
    scanf("%lf", &amount);

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const int accNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->pin, pin) == 0 && accNumber == strtol(user->accountNumber, NULL, 10)) {
                if (balance < amount) {
                    printf("Insufficient balance\n");
                    return;
                }

                if(amount > 1000) {
                    printf("Enter pin to continue: ");
                    char conPin[MAX_PIN_LENGTH];
                    scanf("%s", conPin);
                    if(strcmp(conPin, pin) != 0) {
                        printf("Incorrect pin\n");
                        return;
                    }
                    cJSON_SetNumberValue(cJSON_GetObjectItem(account, "balance"), balance - amount);
                    saveToFile(json, JSON_FILE);
                    printf("Amount withdrawn successfully\n");
                }
                cJSON_SetNumberValue(cJSON_GetObjectItem(account, "balance"), balance - amount);
                saveToFile(json, JSON_FILE);
                printf("Amount withdrawn successfully\n");
                return;
            }
        }
    }

    printf("Account not found\n");
}

void changePin(Account *user, cJSON *json) {
    char newPin[MAX_NAME_LENGTH];

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const int accNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;

            if (strcmp(user->pin, pin) == 0 && accNumber == strtol(user->accountNumber, NULL, 10)) {
                printf("Enter old pin to continue: ");
                char oldPin[MAX_PIN_LENGTH];
                scanf("%s", oldPin);
                if(strcmp(oldPin, pin) != 0) {
                    printf("Incorrect pin\n");
                    return;
                }
                printf("Enter new pin: ");
                scanf("%s", newPin);
                cJSON_SetValuestring(cJSON_GetObjectItem(account, "pin"), newPin);
                saveToFile(json, JSON_FILE);
                printf("Pin changed successfully\n");
                printf("Please login again\n");
                delay(1);
                // system("clear"); // For Windows, use "cls".
                login(user, json);
                return;
            }
        }
    }
    printf("Account not found\n");
    printf("The user had to be logged out due to a technical glitch.\nPlease login again\n");
    delay(1);
    login(user, json);
}

void viewDetails(const Account *user, cJSON *json) {
    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const char *country = cJSON_GetObjectItem(account, "country")->valuestring;
            const char *state = cJSON_GetObjectItem(account, "state")->valuestring;
            const char *city = cJSON_GetObjectItem(account, "city")->valuestring;
            const char *street = cJSON_GetObjectItem(account, "street")->valuestring;
            const char *houseNumber = cJSON_GetObjectItem(account, "houseNumber")->valuestring;
            const char *phone = cJSON_GetObjectItem(account, "phone")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            int accountNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->pin, pin) == 0 && accountNumber == strtol(user->accountNumber, NULL, 10)) {
                printf("\n---------------------\n");
                printf("Name: %s\n", name);
                printf("Address: \nCountry: %s\nState: %s\nCity: %s\nStreet: %s\nHouse number: %s\n", country, state, city, street, houseNumber);
                printf("Phone number: %s\n", phone);
                printf("Pin: %s\n", pin);
                printf("Account number: %d\n", accountNumber);
                printf("Balance: %.2lf\n", balance);
                printf("---------------------\n");
                delay(1);
                // system("clear"); // For Windows, use "cls".
                return;
            }
        }
    }

    printf("Account not found\n");
    delay(1);
    // system("clear"); // For Windows, use "cls".
}

void deleteAccount(Account *user, cJSON *json) {
    char *confirm = malloc(sizeof(char) * 3);
    char conPin[MAX_PIN_LENGTH];


    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        cJSON *newAccounts = cJSON_CreateArray();
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const int accNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            int balance = cJSON_GetObjectItem(account, "balance")->valueint;

            if (strcmp(user->pin, pin) == 0 && accNumber == strtol(user->accountNumber, NULL, 10)) {
                if(balance > 0) {
                    printf("You have a balance of %d in your account. Please withdraw the amount to continue\n"
                           "You were logged out for security reasons.\n"
                           "Please login again.\n", balance);
                    return;
                }
                continue;
            }

            cJSON_AddItemToArray(newAccounts, cJSON_Duplicate(account, 1));
        }


        printf("Enter your pin to continue: ");
        scanf("%s", conPin);
        if(strcmp(conPin, user->pin) != 0) {
            printf("Incorrect pin\n");
            printf("Login again\n");
            return;
        }

        printf("Are you sure you want to delete your account? This action is not reversible. (yes or no): ");
        scanf("%s", confirm);
        if (strcmp((const char *) confirm, "yes") != 0) {
            printf("Account not deleted\n");
            return;
        }

        cJSON_DeleteItemFromObject(json, "accounts");
        cJSON_AddItemToObject(json, "accounts", newAccounts);
        printf("Account deleted successfully\n");
        delay(1);

        // system("clear"); // For Windows, use "cls".
    } else {
        printf("No accounts to delete\n");
        delay(1);
        // system("clear"); // For Windows, use "cls".
    }
}

void saveToFile(const cJSON *json, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file. Function saveToFile()");
        exit(EXIT_FAILURE);
    }

    char *jsonStr = cJSON_Print(json);
    if (jsonStr == NULL) {
        perror("Error creating JSON string. Function saveToFile()");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s", jsonStr);

    fclose(file);
    cJSON_free(jsonStr);
}

cJSON *loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // If the file doesn't exist, create it
        file = fopen(filename, "w");
        if (file == NULL) {
            perror("Error creating file. Function loadFromFile()");
            exit(EXIT_FAILURE);
        }
        fclose(file);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    if (buffer == NULL) {
        perror("Error allocating memory. Function loadFromFile()");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
//    if (bytesRead < fileSize) {
//        perror("Error reading file. Function loadFromFile()");
//        fclose(file);
//        free(buffer);
//        exit(EXIT_FAILURE);
//    }
// Problem when running on windows, windows cache system does not get cleared which means the program will always
// read the same file size even if the file size has changed, the buffer will not be big enough to
// store the file contents

    buffer[fileSize] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        perror("Error parsing JSON. Function loadFromFile()");
        fclose(file);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    free(buffer);

    return json;
}

void delay(int number_of_seconds) {
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();

    while (clock() < start_time + milli_seconds);
}

int randomNumber(cJSON *json) {
    int num;
    int lower = 10000000, upper = 99999999;

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        do {
            num = (rand() % (upper - lower + 1)) + lower;
        } while (accountNumberExists(num, accounts, arraySize));
    } else {
        num = (rand() % (upper - lower + 1)) + lower;
    }

    return num;
}

int accountNumberExists(int num, cJSON *accounts, int size) {
    for (int i = 0; i < size; i++) {
        cJSON *account = cJSON_GetArrayItem(accounts, i);
        int accountNumber = cJSON_GetObjectItem(account, "accountNumber")->valueint;

        if (accountNumber == num) {
            return 1;
        }
    }
    return 0;
}