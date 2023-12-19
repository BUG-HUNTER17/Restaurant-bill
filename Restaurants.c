#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Item {
    char name[20];
    float price;
    int quantity;
};

struct Order {
    char customer[50];
    char date[50];
    int numOfItems;
    struct Item items[50];
};

// Function prototypes
void printBillHeader(char name[50], char date[30]);
void printBillBody(char item[30], int qty, float price);
void printBillFooter(float total);

void generateBill(struct Order ord);
void saveInvoice(struct Order ord);

int main() {
    int option, numOfItems;
    struct Order ord;
    char saveBill = 'y', contFlag = 'y', customerName[50];
    FILE *filePointer;

    while (contFlag == 'y') {
        system("clear");

        printf("\t============RESTAURANT============");
        printf("\n\nPlease select your preferred operation");
        printf("\n\n1. Generate Invoice");
        printf("\n2. Show all Invoices");
        printf("\n3. Search Invoice");
        printf("\n4. Exit");

        printf("\n\nYour choice:\t");
        scanf("%d", &option);
        fflush(stdin);  // Clearing the input buffer

        switch (option) {
            case 1:
                system("clear");
                printf("\nPlease enter the name of the customer:\t");
                scanf("%s", ord.customer);

                strcpy(ord.date, __DATE__);

                printf("\nPlease enter the number of items:\t");
                scanf("%d", &numOfItems);
                ord.numOfItems = numOfItems;

                float total = 0;

                for (int i = 0; i < numOfItems; i++) {
                    printf("\n\n");
                    printf("Please enter the item %d:\t\n", i + 1);
                    scanf("%s", ord.items[i].name);

                    printf("Please enter the quantity:\t");
                    scanf("%d", &ord.items[i].quantity);

                    printf("Please enter the unit price:\t");
                    scanf("%f", &ord.items[i].price);

                    total += ord.items[i].quantity * ord.items[i].price;
                }

                generateBill(ord);

                printf("\nDo you want to save the invoice [y/n]:\t");
                scanf(" %c", &saveBill);

                if (saveBill == 'y') {
                    saveInvoice(ord);
                }
                break;

            case 2:
                system("clear");
                filePointer = fopen("RestaurantBill.dat", "r");
                printf("\n  *****Your Previous Invoices*****\n");

                while (fread(&ord, sizeof(struct Order), 1, filePointer)) {
                    generateBill(ord);
                }
                fclose(filePointer);
                break;

            case 3:
                printf("Enter the name of the customer:\t");
                scanf("%s", customerName);
                system("clear");

                filePointer = fopen("RestaurantBill.dat", "r");
                printf("\t*****Invoice of %s*****", customerName);

                int invoiceFound = 0;

                while (fread(&ord, sizeof(struct Order), 1, filePointer)) {
                    if (!strcmp(ord.customer, customerName)) {
                        generateBill(ord);
                        invoiceFound = 1;
                        break;
                    }
                }

                fclose(filePointer);

                if (!invoiceFound) {
                    printf("Sorry, the invoice for %s does not exist", customerName);
                }
                break;

            case 4:
                printf("\n\t\t Bye Bye :)\n\n");
                exit(0);

            default:
                printf("Sorry, invalid option");
                break;
        }

        printf("\nDo you want to perform another operation?[y/n]:\t");
        scanf(" %c", &contFlag);
    }

    printf("\n\t\t Bye Bye :)\n\n");
    printf("\n\n");

    return 0;
}

void printBillHeader(char name[50], char date[30]) {
    printf("\n\n");
    printf("\t    ADV. Restaurant");
    printf("\n\t   -----------------");
    printf("\nDate:%s", date);
    printf("\nInvoice To: %s", name);
    printf("\n");
    printf("---------------------------------------\n");
    printf("Items\t\t");
    printf("Qty\t\t");
    printf("Total\t\t");
    printf("\n---------------------------------------");
    printf("\n\n");
}

void printBillBody(char item[30], int qty, float price) {
    printf("%s\t\t", item);
    printf("%d\t\t", qty);
    printf("%.2f\t\t", qty * price);
    printf("\n");
}

void printBillFooter(float total) {
    printf("\n");
    float discount = 0.1 * total;
    float netTotal = total - discount;
    float cgst = 0.09 * netTotal;
    float grandTotal = netTotal + 2 * cgst;  // netTotal + cgst + sgst
    printf("---------------------------------------\n");
    printf("Sub Total\t\t\t%.2f", total);
    printf("\nDiscount @10%s\t\t\t%.2f", "%", discount);
    printf("\n\t\t\t\t-------");
    printf("\nNet Total\t\t\t%.2f", netTotal);
    printf("\nCGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\nSGST @9%s\t\t\t%.2f", "%", cgst);
    printf("\n---------------------------------------");
    printf("\nGrand Total\t\t\t%.2f", grandTotal);
    printf("\n---------------------------------------\n");
}

void generateBill(struct Order ord) {
    float total = 0;
    printBillHeader(ord.customer, ord.date);

    for (int i = 0; i < ord.numOfItems; i++) {
        printBillBody(ord.items[i].name, ord.items[i].quantity, ord.items[i].price);
        total += ord.items[i].quantity * ord.items[i].price;
    }

    printBillFooter(total);
}

void saveInvoice(struct Order ord) {
    FILE *filePointer = fopen("RestaurantBill.dat", "a+");

    if (fwrite(&ord, sizeof(struct Order), 1, filePointer) != 0) {
        printf("\nSuccessfully saved");
    } else {
        printf("\nError saving");
    }

    fclose(filePointer);
}
