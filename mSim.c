//tentative version, maybe c++ is better than struct for classes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

// customer 
typedef struct {
    char *name;
    double budget;
    char *preference;
} Customer;

// product 
typedef struct {
    char *name;
    double price;
    int quantity;
} Product;

// Function to parse the customers.json file
Customer *parse_customers(const char *filename, int *customer_count) {
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
Product *parse_products(const char *filename, int *product_count) {
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

// Main function to execute the program
int main(int argc, char *argv[]) {
    if (argc < 3) {
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

    // Print parsed data
    printf("Customers:\n");
    for (int i = 0; i < customer_count; i++) {
        printf("Name: %s, Budget: %.2f, Preference: %s\n",
               customers[i].name, customers[i].budget, customers[i].preference);
    }
    
    printf("\nProducts:\n");
    for (int i = 0; i < product_count; i++) {
        printf("Name: %s, Price: %.2f, Quantity: %d\n",
               products[i].name, products[i].price, products[i].quantity);
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
