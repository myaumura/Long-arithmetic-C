#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DIGITS 2500

// MARK: - Структура большого числа
typedef struct {
    int digits[MAX_DIGITS];
    short int length;
    short int sign;
} bn;

// MARK: - Stack

char stack[25];
int top = -1;

// MARK: - Stack operators

void push(char*);
char pop(void);
int priority(char*);
void convert(char(*)[], int, char(*)[]);
bn execute(char(*)[], int);


// MARK: - Expression

void parse_expression(void);
void execute_expression(char*);
int string_to_tokens(char *expression, char(*)[]);
void add_space(char *expression, char *result);
bn execute_bn(char*);

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

// MARK: - Main

int main(void) {
    parse_expression();
    return EXIT_SUCCESS;
}

// MARK: - Инициализация большого числа

void bn_init(bn * num) {
    memset(num->digits, 0x00, sizeof(int) * MAX_DIGITS);
    num->length = 2500;
}

// MARK: - Создание большого числа

bn bn_create(char *str) {
    bn num;
    bn_init(&num);
    if (str[0] == '-') {
        num.sign = -1;
        str++;
    } else num.sign = 1;
    num.length = strlen(str);
    memset(num.digits, 0x00, sizeof(int) * MAX_DIGITS);
    for (short int i = 0; i < num.length; i++) num.digits[num.length - i - 1] = str[i] - '0';
    return num;
}

// MARK: - Вывод большого числа

void bn_print(bn *num) {
    if (num->sign == -1) { printf("-"); }
    for (short int i = num->length - 1 ; i >= 0; i--) printf("%d", num->digits[i]);  // потому что идем справа налево <--
    printf("\n");
}

// MARK: - Сложение больших чисел

bn bn_add(bn *first_num, bn *second_num) {
    bn result;
    bn_init(&result);
    short int carry_flag = 0,sum = 0;
    
    for (int i = 0; i < MAX_DIGITS; i++) {
        sum = first_num->digits[i] + second_num->digits[i] + carry_flag;
        carry_flag = sum / 10;
        result.digits[i] = sum % 10;
    }
    bn_clear_nulls(&result);
    return result;
}

// MARK: - Вычитание больших чисел

bn bn_sub(bn *first_num, bn *second_num) {
    bn result;
    bn_init(&result);
    
    short int carry_flag = 0;
    
    for(short int i = 0; i < MAX_DIGITS; i++) {
        short int sub = 0;
        
        if (first_num->length > second_num->length) {
            sub = first_num->digits[i] - second_num->digits[i] - carry_flag;
        } else if (second_num->length > first_num->length) {
            sub = second_num->digits[i] - first_num->digits[i] - carry_flag;
        } else {
            switch (bn_cmp(first_num, second_num)) {
                case 1:
                    sub = first_num->digits[i] - second_num->digits[i] - carry_flag;
                    break;
                case -1:
                    sub = second_num->digits[i] - first_num->digits[i] - carry_flag;
                    result.sign = -1;
                    break;
            }
        }
        
        if (sub < 0) {
            sub += 10;
            carry_flag = 1;
        } else {
            carry_flag = 0;
        }
        result.digits[i] = sub;
    }
    bn_clear_nulls(&result);
    return result;
}

// MARK: - Умножение больших чисел

bn bn_mul(bn* first_number, bn* second_num) {
    bn result, temp1, temp2;
    bn_copy(first_number, &temp1);
    bn_copy(second_num, &temp2);
    bn_init(&result);
    
    short int current = 0;
    
    for(short int i = 0; i < temp1.length; i++) {
        for (short int j = 0; j < temp2.length; j++) {
            current = temp1.digits[i] * temp2.digits[j];
            result.digits[i + j] += current;
        }
    }
    for(short int i = 0; i < result.length - 1; i++) {
        result.digits[i + 1] +=  result.digits[i] / 10;
        result.digits[i] %= 10;
    }
    bn_clear_nulls(&result);
    return result;
}

// MARK: - Деление большого числа на 2

void bn_div_by_2(bn* num) {
    int carry = 0;
    for (int i = num->length; i >= 0; i--) {
        int carry_value = carry ? 10 : 0;
        int cur = num->digits[i] + carry_value;
        num->digits[i] = cur / 2;
        carry = cur % 2;
    }
    bn_clear_nulls(num);
}

// MARK: - Факториал большого числа

bn bn_fact(bn* num) {
    bn result, temp;
    bn_copy(num, &temp);
    bn_dec(num);
    
    while (!bn_is_zero(num)) {
        result = bn_mul(&temp, num);
        bn_dec(num);
        bn_copy(&result, &temp);
        bn_init(&result);
    }
    bn_copy(&temp, &result);
    return result;
}

// MARK: - Возведение в степень большого числа

bn bn_pow(bn* num, bn* exp) {
    bn result;
    
    if (bn_is_zero(exp)) {
        result.digits[0] = 1;
        result.length = 1;
    } else if ((num->length == 1) && (num->digits[0] == 1)) {
        bn_copy(num, &result);
    } else {
        bn temp;
        bn_init(&temp);
        bn_copy(num, &temp);
        bn_dec(exp);
        
        while (!bn_is_zero(exp)) {
            result = bn_mul(&temp, num);
            bn_dec(exp);
            bn_copy(&result, &temp);
        }
        bn_copy(&temp, &result);
    }
    return result;
}

// MARK: - Сумма всех чисел от и до

bn bn_sum_from_to(bn* first_num, bn* second_num) {
    bn result, count;
    int flag = 0;
    bn_copy(second_num, &result);
    
    if (first_num->sign == -1) {
        result = bn_sub(second_num, first_num);
        if (bn_cmp(first_num, second_num) == 1) {
            flag = 1;
        }
    } else {
        result = bn_add(second_num, first_num);
    }
    
    if (first_num->sign == -1) {
        count = bn_add(second_num, first_num);
    } else {
        count = bn_sub(second_num, first_num);
    }
    
    bn_inc(&count);
    result = bn_mul(&result, &count);
    bn_div_by_2(&result);
    if (flag) { result.sign = -1; }
    return result;
}

// MARK: - Проверка числа на ноль

int bn_is_zero(bn* num) {
    for (int i = 0; i < num->length; i++) {
        if (num->digits[i]) {
            return 0;
        }
    }
    return 1;
}

// MARK: - Уменьшение числа на единицу

void bn_dec(bn *num) {
    short int carry_flag = 1;
    
    for (short int i = 0; i < num->length; i++) {
        short int sub = 0;
        
        sub = num->digits[i] - carry_flag;
        
        if (sub < 0) {
            sub += 10;
            carry_flag = 1;
        } else {
            carry_flag = 0;
        }
        num->digits[i] = sub;
    }
    bn_clear_nulls(num);
}

// MARK: - Увеличение числа на единицу

void bn_inc(bn* num) {
    int sum, carry_flag = 1;
    
    for (int i = 0; i <= num->length; i++) {
        sum = num->digits[i] + carry_flag;
        if (sum >= 10) {
            num->length++;
        }
        carry_flag = sum / 10;
        num->digits[i] = sum % 10;
    }
}

// MARK: - Сравнение больших чисел

int bn_cmp(bn* first_num, bn* second_num) {
    int i = MAX_DIGITS;
    do {
        i -= 1;
        if (first_num->digits[i] > second_num->digits[i]) {
            return 1;
        } else if (first_num->digits[i] < second_num->digits[i]) {
            return -1;
        }
    } while (i != 0);
    
    return 0;
}

// MARK: - Удаление нулей перед числом

void bn_clear_nulls(bn* num) {
    while (num->digits[num->length - 1] == 0 && num->length > 1) num->length--;
}

// MARK: - Функция копирования структуры

void bn_copy(bn* src, bn* dst) {
    for(int i = 0; i < src->length; i++) {
        dst->digits[i] = src->digits[i];
    }
    dst->length = src->length;
}

// MARK: - Чтение выражения

void parse_expression(void) {
    char buffer[2500];
    fgets(buffer, sizeof(buffer), stdin);
    execute_expression(buffer);
}

// MARK: - Функция определения приоритета оператора

int priority(char* operator) {
    switch (operator[0]) {
        case '!':
            return 5;
        case '^':
            return 4;
        case '*':
            return 3;
        case '+':
            return 2;
        case '-':
            return 1;
        case '#':
            return -1;
        default:
            return -1;
    }
}

// MARK: - Вычисление выражения

void execute_expression(char* buffer) {
    char temp[2500], postfix[100][2500];
    char tokens[100][2500];
    size_t i = 0, flag = 0;
    
    while (buffer[i] != '\0') {
        if (buffer[i] == '\n') {
            flag = 1;
            temp[i] = buffer[i + 1];
        } else {
            temp[i] = buffer[i];
        }
        if (flag) {
            temp[i] = buffer[i + 1];
        }
        i++;
    }
    temp[i] = '\0';
    int num_count = string_to_tokens(temp, tokens);
    // нужно реализовать либо мат выражение, либо сумма от и до
    convert(tokens, num_count ,postfix);
    bn result = execute(postfix, num_count);
    bn_print(&result);
}

// MARK: - Проверка символа на оператор

int is_operator(char* c) {
    if ((c[0] == '+') || (c[0] == '-') || (c[0] == '*') || (c[0] == '^') || (c[0] == '!')) {
        return 1;
    }
    return 0;
}

// MARK: - Проверка на число

int is_number(char* c) {
    int i = 0;
    while (c[i] != '\0') {
        if (c[i] >= '0' && c[i] <= '9') {
            return 1;
        }
        i++;
    }
    return 0;
}

// MARK: - Push operator

void push(char* item) {
    stack[++top] = item[0];
}

// MARK: - Pop operator

char pop(void) {
    return stack[top--];
}

// MARK: - Convert from infix to postfix

void convert(char infix[][2500], int num_count, char postfix[][2500]) {
    int i, j = 0;
    char symbol[2500];
    stack[++top] = '#';
    
    for (i = 0; i < num_count; i++) {
        strcpy(symbol, infix[i]);
        
        if (is_operator(symbol) == 0) {
            strcpy(postfix[j], symbol);
            j++;
        } else {
            if (priority(symbol) > priority(&stack[top])) {
                push(symbol);
            } else {
                while (priority(symbol) < priority(&stack[top])) {
                    strncpy(postfix[j++], &stack[top--], 1);
                }
                push(symbol);
            }
        }
    }
    while (stack[top] != '#') {
        strncpy(postfix[j++], &stack[top--], 1);
    }
    char end_str = '\0';
    strcpy(postfix[j], &end_str);
}

// MARK: - Стуктура стека

struct Stack {
    char data[100][2500];
    int top;
};

// MARK: - Функция инициализации стека

void initializeStack(struct Stack *stack) {
    stack->top = -1;
}

// MARK: - Помещение элемента в стек

void push_stack(struct Stack *stack, const char *item) {
    stack->top++;
    strcpy(stack->data[stack->top], item);
}

// MARK: - Извлечение элемента из стека

char* pop_stack(struct Stack *stack) {
    return stack->data[stack->top--];
}

// MARK: - Вычисление выражения

bn execute(char postfix[][2500], int count) {
    struct Stack stack;
    initializeStack(&stack);
    
    bn result, operand1, operand2;;
    bn_init(&result);
    char token[2500], token1[2500], token2[2500];
    int i = 0, j = 0;
   
    while (i < count) {
       
        if (is_number(postfix[j])) {
            push_stack(&stack, postfix[j++]);
        } else {
            strcpy(token, postfix[j++]);
            
            if (is_operator(token)) {
               
                strcpy(token2, pop_stack(&stack));
                strcpy(token1, pop_stack(&stack));

                operand2 = bn_create(token2);
                operand1 = bn_create(token1);

                switch (token[0]) {
                    case '+':
                        result = bn_add(&operand1, &operand2);
                        break;
                    case '-':
                        result = bn_sub(&operand1, &operand2);
                        break;
                    case '*':
                        result = bn_mul(&operand1, &operand2);
                        break;
                    case '^':
                        result = bn_pow(&operand1, &operand2);
                        break;
                    case '!':
                        result = bn_fact(&operand2);
                        break;
                }
                char* result_str = bn_to_string(&result);
                push_stack(&stack, result_str);
            }
        }
        i++;
    }
    return result;
}

// MARK: - Добавление пробела перед оператором

void add_space(char *expression, char *result) {
    size_t len = strlen(expression);
    int resultIndex = 0;
    
    for (int i = 0; i < len; i++) {
        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '!' || expression[i] == '^') {
            result[resultIndex++] = ' ';
            result[resultIndex++] = expression[i];
            result[resultIndex++] = ' ';
        } else if (expression[i] != ' ' && expression[i] != '\n') {
            result[resultIndex++] = expression[i];
        }
    }
    result[resultIndex] = '\0';
}

// MARK: - Разделение строки на числа и операторы

int string_to_tokens(char *expression, char tokens[][2500]) {
    char spacedExpression[2 * MAX_DIGITS];
    int num_count = 0;
    add_space(expression, spacedExpression);
    
    char *token = strtok(spacedExpression, " \n");
    while (token != NULL) {
        if ((token[0] >= '0' && token[0] <= '9') || (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '!' || token[0] == '^')) {
            strcpy(tokens[num_count], token);
            num_count++;
        }
        token = strtok(NULL, " \n");
    }
    return num_count;
}

// MARK: - Функция преобразования большого числа в строку

char* bn_to_string(bn* num) {
    int buffer_size = 0;
    
    for (int i = 0; i < num->length; i++) {
        buffer_size += snprintf(NULL, 0, "%d", num->digits[i]);
    }
    char *result = (char *)malloc(buffer_size + 1);
    
    int position = 0;
    for (int i = 0; i < num->length; i++) {
        position += snprintf(result + position, buffer_size - position + 1, "%d", num->digits[i]);
    }
    
    for (int i = 0, j = position - 1; i < j; i++, j--) {
        char temp = result[i];
        result[i] = result[j];
        result[j] = temp;
    }
    return result;
}
