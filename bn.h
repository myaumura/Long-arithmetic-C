#ifndef bn_h
#define bn_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 2500

// MARK: - Структура большого числа

typedef struct {
    int digits[MAX_DIGITS];
    short int length;
    short int sign;
} bn;

// MARK: - Стуктура стека

struct Stack {
    char data[100][MAX_DIGITS];
    int top;
};

// MARK: - Stack operators

void push(char*);
char pop(void);
int priority(char*);
void initializeStack(struct Stack*);
void push_stack(struct Stack*, const char*);
char* pop_stack(struct Stack*);

// MARK: - Check

int is_number(char*);
int is_operator(char*);

// MARK: - Init

bn bn_create(char*);
void bn_init(bn*);
void bn_print(bn*);

// MARK: - BigNumber Operations

bn bn_add(bn*, bn*);
bn bn_sub(bn*, bn*);
bn bn_mul(bn*, bn*);
void bn_div_by_2(bn*);
bn bn_fact(bn*);
bn bn_pow(bn*, bn*);
bn bn_sum_from_to(bn*, bn*);

// MARK: - Support

int  bn_is_zero(bn*);
void bn_dec(bn*);
void bn_inc(bn*);
int  bn_cmp(bn*, bn*);
void bn_clear_nulls(bn*);
void bn_copy(bn*, bn*);
char* bn_to_string(bn*);

// MARK: - Expression

void parse_expression(void);
void execute_expression(char*);
void convert(char(*)[], int, char(*)[]);
bn execute(char(*)[], int);
int string_to_tokens(char *expression, char(*)[]);
void add_space(char *expression, char *result);
bn execute_bn(char*);
void execute_sum_range(char*, char*);

#endif /* bn_h */
