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
#define MAX_ACCOUNT_LENGTH 10
#define JSON_FILE "accounts.json"


typedef struct {
    char name[MAX_NAME_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char pin[MAX_PIN_LENGTH];
    char accountNumber[MAX_ACCOUNT_LENGTH];
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
int randomNumber();
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
    printf("Welcome to the Bank Management System\n");
    printf("Press Enter to continue\n");
    getchar();
    system("clear"); // For Windows, use "cls".
}

void login(Account *user, cJSON *json) {
    printf("Enter your username: ");
    scanf("%s", user->name);
    printf("Enter your password: ");
    scanf("%s", user->pin);

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;

            if (strcmp(user->name, name) == 0 && strcmp(user->pin, pin) == 0) {
                printf("Login successful\n");
                delay(1);
                system("clear"); // For Windows, use "cls".
                return;
            }
        }
    }

    // Account creation intent. Invoked when username not detected.
    // Now, it only gets called when an incorrect pin has been entered. Need to fix.
    printf("Username does not exist. Do you want to create a new account? (1 for yes, 0 for no): ");
    int createNewAccount;
    scanf("%d", &createNewAccount);

    if (createNewAccount) {
        newAccount(json);
        saveToFile(json, "accounts.json");
        login(user, json); // Reattempt login after creating the account
    } else {
        printf("Login failed\n");
        delay(1);
        system("clear"); // For Windows, use "cls".
        login(user, json);
    }
}

void menu(Account *user, cJSON *json) {
    int choice;
    do {
        printf("1. Create new account\n");
        printf("2. Check balance\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Change pin\n");
        printf("6. Logout\n");
        printf("7. View details\n");
        printf("8. Delete account\n");
        printf("9. Exit\n");
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
            case 9:
                printf("Goodbye\n");
                break;
            default:
                printf("Invalid choice\n");
                delay(1);
                system("clear"); // For Windows, use "cls".
                break;
        }
    } while (choice != 9);
}

void newAccount(cJSON *json) {
    Account newAccount;

    printf("Enter your name: ");
    scanf("%s", newAccount.name);
    printf("Enter your address: ");
    scanf("%s", newAccount.address);
    printf("Enter your phone number: ");
    scanf("%s", newAccount.phone);
    printf("Enter your pin: ");
    scanf("%s", newAccount.pin);
    printf("Enter your balance: ");
    scanf("%lf", &newAccount.balance);

    cJSON *accountObject = cJSON_CreateObject();
    cJSON_AddStringToObject(accountObject, "name", newAccount.name);
    cJSON_AddStringToObject(accountObject, "address", newAccount.address);
    cJSON_AddStringToObject(accountObject, "phone", newAccount.phone);
    cJSON_AddStringToObject(accountObject, "pin", newAccount.pin);
    double accountNumber = randomNumber();
    printf("Your account number is: %.2lf\n", accountNumber);
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
    system("clear"); // For Windows, use "cls".
}

void checkBalance(const Account *user, cJSON *json) {
    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->name, name) == 0) {
                printf("Your balance is %.2lf\n", balance);
                delay(1);
                system("clear"); // For Windows, use "cls".
                return;
            }
        }
    }

    printf("Account not found\n");
    delay(1);
    system("clear"); // For Windows, use "cls".
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
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->name, name) == 0 && strcmp(user->pin, pin) == 0) {
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
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->name, name) == 0 && strcmp(user->pin, pin) == 0) {
                if (balance < amount) {
                    printf("Insufficient balance\n");
                    return;
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
    printf("Enter your new pin: ");
    scanf("%s", newPin);

    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;

            if (strcmp(user->name, name) == 0 && strcmp(user->pin, pin) == 0) {
                cJSON_SetValuestring(cJSON_GetObjectItem(account, "pin"), newPin);
                saveToFile(json, JSON_FILE);
                printf("Pin changed successfully\n");
                return;
            }
        }
    }

    printf("Account not found\n");
}
void viewDetails(const Account *user, cJSON *json) {
    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const char *address = cJSON_GetObjectItem(account, "address")->valuestring;
            const char *phone = cJSON_GetObjectItem(account, "phone")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;
            double accountNumber = cJSON_GetObjectItem(account, "accountNumber")->valuedouble;
            double balance = cJSON_GetObjectItem(account, "balance")->valuedouble;

            if (strcmp(user->name, name) == 0) {
                printf("Name: %s\n", name);
                printf("Address: %s\n", address);
                printf("Phone number: %s\n", phone);
                printf("Pin: %s\n", pin);
                printf("Account number: %.2lf\n", accountNumber);
                printf("Balance: %.2lf\n", balance);
                delay(1);
                system("clear"); // For Windows, use "cls".
                return;
            }
        }
    }

    printf("Account not found\n");
    delay(1);
    system("clear"); // For Windows, use "cls".
}

void deleteAccount(Account *user, cJSON *json) {
    cJSON *accounts = cJSON_GetObjectItem(json, "accounts");
    if (cJSON_IsArray(accounts)) {
        int arraySize = cJSON_GetArraySize(accounts);
        cJSON *newAccounts = cJSON_CreateArray();
        for (int i = 0; i < arraySize; i++) {
            cJSON *account = cJSON_GetArrayItem(accounts, i);
            const char *name = cJSON_GetObjectItem(account, "name")->valuestring;
            const char *pin = cJSON_GetObjectItem(account, "pin")->valuestring;

            if (strcmp(user->name, name) == 0 && strcmp(user->pin, pin) == 0) {
                continue;
            }

            cJSON_AddItemToArray(newAccounts, cJSON_Duplicate(account, 1));
        }

        cJSON_DeleteItemFromObject(json, "accounts");
        cJSON_AddItemToObject(json, "accounts", newAccounts);

        printf("Account deleted successfully\n");
        delay(1);
        system("clear"); // For Windows, use "cls".
    } else {
        printf("No accounts to delete\n");
        delay(1);
        system("clear"); // For Windows, use "cls".
    }
}

void saveToFile(const cJSON *json, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char *jsonStr = cJSON_Print(json);
    if (jsonStr == NULL) {
        perror("Error creating JSON string");
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
            perror("Error creating file");
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
        perror("Error allocating memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead < fileSize) {
        perror("Error reading file");
        fclose(file);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    buffer[fileSize] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        perror("Error parsing JSON");
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

int randomNumber()
{
    int i, num = 0;
    int lower = 100000, upper = 999999;
    for (i = 0; i < 7; i++) {
        num = (rand() % (upper - lower + 1)) + lower;
    }
    return num;
}