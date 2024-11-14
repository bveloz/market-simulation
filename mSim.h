#ifndef mSIM_H
#define mSIM_H


typedef enum PREFERENCE_T PREFERENCE_T;
typedef struct Customer Customer;
typedef struct Product Product;
typedef struct Wallet Wallet;
typedef struct ProductAveragePrice ProductAveragePrice;

char* change_to_lower(char*);
PREFERENCE_T string_to_preference(char*);
const char* preference_to_string(PREFERENCE_T);
Customer* parse_customers(const char *, int *);
Product* parse_products(const char *, int *);
Wallet* create_wallet(Customer*, Product*, int);
void free_wallet(Wallet*);
float calculate_purchase_probability(Wallet*, Product*);
int consume_product(Wallet*, int);
int purchase_amount(Wallet*, double);
int purchase_product(Wallet*, Product*, int);
void restock_product(Product*, int);

#endif