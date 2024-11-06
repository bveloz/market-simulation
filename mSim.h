#ifndef mSIM_H
#define mSIM_H

typedef struct Customer Customer;
typedef struct Product Product;
typedef struct Wallet Wallet;

Customer* parse_customers(const char *, int *);
Product* parse_products(const char *, int *);
Wallet* create_wallet(Customer*, Product*);
void free_wallet(Wallet*);
int consume_product(Wallet*, int);
int purchase_product(Wallet*, int, int);
void restock_product(Product*, int);

#endif