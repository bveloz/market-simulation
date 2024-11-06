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
    char *preference; 
};

// product 
struct Product
{
    int productId;
    char *name;
    char* seller;
    double price;
    int quantity;
};

struct Wallet
{
    Customer* owner;
    Product* product;
    int capacity;
};

double price_factors[MAX_HOURS] = {
    1.15, 1.15, 1.10, 1.10, 1.05, 1.05, 0.95, 0.9, 0.85, 0.8,   // Morning Prices
    0.75, 0.7,  0.8,  0.9,  1.0,  1.1,  1.2,  1.25, 1.2,  1.15, // Afternoon Prices
    1.1,  1.05, 1.0,  0.95                                      // Evening Prices
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
        products[i].productId = cJSON_GetObjectItem(item, "ProductId")->valueint;
        products[i].name = strdup(cJSON_GetObjectItem(item, "name")->valuestring);
        products[i].seller = strdup(cJSON_GetObjectItem(item, "seller")->valuestring);
        products[i].price = cJSON_GetObjectItem(item, "price")->valuedouble;
        products[i].quantity = cJSON_GetObjectItem(item, "quantity")->valueint;
    }
    
    cJSON_Delete(json);
    free(data);
    return products;
}

//function to create a wallet, customer begins with 0 items
Wallet* create_wallet(Customer* c, Product* p, int cap)
{
    Wallet* returnWallet = (Wallet*)malloc(sizeof(Wallet));
    returnWallet->owner = c;
    returnWallet->product = (Product*)malloc(sizeof(Product));
    memcpy(returnWallet->product, p, sizeof(Product));
    // by default 0 items
    returnWallet->product->quantity = 0;
    //set capacity
    returnWallet->capacity = cap;
    return returnWallet;
}

void free_wallet(Wallet* w)
{
    free(w->product);
    w->product = NULL;
    free(w);
    w = NULL;
}

float calculate_purchase_probability(Wallet* wallet, Product* product)
{
    if (wallet->product->productId != product->productId)
    {
        printf("The product does not match the wallet\n");
        return 1;
    }
    float probability;

    
}

int consume_product(Wallet* consumer, int quantity)
{
    if (consumer->product->quantity < quantity)
    {
        printf("Attempting to consume more than is available\n");
        return 1;
    }
    else
    {
        consumer->product->quantity -= quantity;
        printf("%s consumed %d of %s\n", consumer->owner->name, quantity, consumer->product->name);
        return 0;
    }
}

int purchase_product(Wallet* wallet, Product* product, int quantity, int hour)
{
    if (wallet->product->productId != product->productId)
    {
        printf("The product does not match the wallet\n");
        return 1;
    }
    int cost = wallet->product->price * quantity * price_factors[hour];
    if (cost > wallet->owner->budget)
    {
        printf("The amount requested is out of budget\n");
        return 1;
    }
    else
    {
        wallet->owner->budget -= cost;
        wallet->product->quantity += quantity;
        return 0;
    }
}

void restock_product(Product* product, int quantity)
{
    product->quantity += quantity;
}

// Main function to execute the program
int main(int argc, char *argv[]) 
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s customers.json products.json\n", argv[0]);
        return 1;
    }
    int i;
    srand(1);
    int customer_count, product_count;
    Customer *customers = parse_customers(argv[1], &customer_count);
    Product *products = parse_products(argv[2], &product_count);
    
    if (!customers || !products) {
        fprintf(stderr, "Failed to parse JSON files\n");
        return 1;
    }

    
    

    // Print parsed data
    printf("Customers:\n");
    for (i = 0; i < customer_count; i++) {
        printf("Name: %s, Budget: %.2f, Preference: %s\n",
               customers[i].name, customers[i].budget, customers[i].preference);
    }
    
    printf("\nProducts:\n");
    for (i = 0; i < product_count; i++) {
        printf("Name: %s, Price: %.2f, Quantity: %d, ProductId: %d, Sold at: %s\n",
               products[i].name, products[i].price, products[i].quantity, products[i].productId, products[i].seller);
    }
    
    Wallet* customer_wallet = create_wallet(customers, products, 15);
    printf("\nNew Wallet: %s, %s, %d\n", customer_wallet->owner->name, customer_wallet->product->name, customer_wallet->product->quantity);
    //create a new time object
    Time* t = initialize_default_time();
    printf("Start of day:\n");
    //pass through a 24 hour cycle
    for (i = 0; i < MAX_HOURS; i++)
    {
        print_time(t);
        increment_time(t, 1, HOUR);
        if (i >= 9 && i <= 17)
        {
            consume_product(customer_wallet, 1);
        }
    }

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
