#ifndef mSIM_H
#define mSIM_H

typedef struct Customer Customer;
typedef struct Product Product;
typedef struct Wallet Wallet;

Customer* parse_customers(const char *, int *);
Product* parse_products(const char *, int *);
Wallet* create_wallet(Customer*, Product*);


#endif