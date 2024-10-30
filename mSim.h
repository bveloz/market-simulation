#ifndef mSIM_H
#define mSIM_H

typedef struct Customer Customer;
typedef struct Product Product;

Customer* parse_customers(const char *, int *);
Product *parse_products(const char *, int *);



#endif