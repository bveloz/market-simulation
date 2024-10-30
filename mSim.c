//tentative version, maybe c++ is better than struct for classes


#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "time.h"
#include "mSim.h"

// customer 
struct Customer 
{
    char *name;
    double budget;
    /* if the number of products expands, the structure of preference may change, to a list of prefereneces with a ranking
    so the json would look something like this
    [
        {
            "name": Alice,
            "budget": 150.0,
            "preference": 
            [
                {
                    "product": "Laptop"
                    "preference": ".54"
                },
                {
                    "product": "Fruit"
                    "preference" ".19"
                }
            ]
        }
    ]
    where the value of preference is a percentage 0-1 or rating 1-10
    //*/
    char *preference; 
};

// product 
struct Product
{
    char *name;
    double price;
    int quantity;
};

struct Wallet
{
    Customer* owner;
    Product* product;
};


// Function to parse the customers.json file
Customer* parse_customers(const char *filename, int *customer_count) 
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        fprintf(stderr, "Error parsing JSON\n");
        free(data);
        return NULL;
    }
    
    *customer_count = cJSON_GetArraySize(json);
    Customer *customers = (Customer *)malloc(*customer_count * sizeof(Customer));
    
    for (int i = 0; i < *customer_count; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        customers[i].name = strdup(cJSON_GetObjectItem(item, "name")->valuestring);
        customers[i].budget = cJSON_GetObjectItem(item, "budget")->valuedouble;
        customers[i].preference = strdup(cJSON_GetObjectItem(item, "preference")->valuestring);
    }
    
    cJSON_Delete(json);
    free(data);
    return customers;
}

// Function to parse the products.json file 
Product *parse_products(const char *filename, int *product_count) 
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open file %s\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *data = (char *)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        fprintf(stderr, "Error parsing JSON\n");
        free(data);
        return NULL;
    }
    
    *product_count = cJSON_GetArraySize(json);
    Product *products = (Product *)malloc(*product_count * sizeof(Product));
    
    for (int i = 0; i < *product_count; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);
        products[i].name = strdup(cJSON_GetObjectItem(item, "name")->valuestring);
        products[i].price = cJSON_GetObjectItem(item, "price")->valuedouble;
        products[i].quantity = cJSON_GetObjectItem(item, "quantity")->valueint;
    }
    
    cJSON_Delete(json);
    free(data);
    return products;
}

//function to create a wallet, customer begins with 0 items
Wallet* create_wallet(Customer* c, Product* p)
{
    Wallet* returnWallet = (Wallet*)malloc(sizeof(Wallet));
    returnWallet->owner = c;
    returnWallet->product = (Product*)malloc(sizeof(Product));
    memcpy(returnWallet->product, p, sizeof(Product));
    // by default 0 items
    returnWallet->product->quantity = 0;
    return returnWallet;
}

void free_wallet(Wallet* w)
{
    free(w->product);
    w->product = NULL;
    free(w);
    w = NULL;
}

void consume_product(Wallet* consumer, int quantity)
{
    if (consumer->product->quantity < quantity)
    {
        printf("Attempting to consume more than is available\n");
    }
    else
    {
        consumer->product->quantity -= quantity;
    }

}

void purchase_product(Product* Product, int quantity)
{
    return;
}

void restock_product(Product* product, int quantity)
{
    product->quantity += quantity;
}

// Main function to execute the program
int main(int argc, char *argv[]) 
{
    int i;
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s customers.json products.json\n", argv[0]);
        return 1;
    }
    
    int customer_count, product_count;
    Customer *customers = parse_customers(argv[1], &customer_count);
    Product *products = parse_products(argv[2], &product_count);
    
    if (!customers || !products) {
        fprintf(stderr, "Failed to parse JSON files\n");
        return 1;
    }

    
    //create a new time object
    Time* t = initialize_default_time();
    print_time(t);

    // Print parsed data
    printf("Customers:\n");
    for (i = 0; i < customer_count; i++) {
        printf("Name: %s, Budget: %.2f, Preference: %s\n",
               customers[i].name, customers[i].budget, customers[i].preference);
    }
    
    printf("\nProducts:\n");
    for (i = 0; i < product_count; i++) {
        printf("Name: %s, Price: %.2f, Quantity: %d\n",
               products[i].name, products[i].price, products[i].quantity);
    }
    
    Wallet* customer_wallet = create_wallet(customers, products);
    printf("\nNew Wallet: %s, %s, %d\n", customer_wallet->owner->name, customer_wallet->product->name, customer_wallet->product->quantity);
    
    //pass through a 24 hour cycle
    for (i = 0; i < 24; i++)
    {

        increment_time(t, 1, HOUR);
        print_time(t);
    }

    printf("Crash\n");
    // Free allocated memory
    for (int i = 0; i < customer_count; i++) {
        free(customers[i].name);
        free(customers[i].preference);
    }
    for (int i = 0; i < product_count; i++) {
        free(products[i].name);
    }
    free(customers);
    free(products);

    return 0;
}
