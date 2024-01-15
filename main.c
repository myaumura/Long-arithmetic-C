#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DIGITS 2500

// MARK: - Stack

char stack[25];
int top = -1;

// MARK: - Stack operators

void push(char);
char pop(void);
int priority(char);
int is_operator(char);
void convert(char[], char[]);
void push_int(int);
void itoa(int, char[]);
void reverse(char[]);
char pop_int(void);
int evaluate(char*);
void string_to_tokens(char *expression, char(*)[], char[]);
void add_space(char *expression, char *result);

// MARK: - Структура большого числа
typedef struct {
    int digits[MAX_DIGITS];
    short int length;
    short int sign;
} bn;

// MARK: - Expression

void parse_expression(void);
bn perform_operation(char, bn*, bn*);
void execute_expression(char*);
bn evaluate_expression(char*);


bn execute_bn(char*);

int is_digit(char);


// MARK: - Init

bn bn_create(char*);             // success
void bn_init(bn*);               // success
void bn_print(bn*);              // success

// MARK: - Operaions

bn bn_add(bn*, bn*);             // success
bn bn_sub(bn*, bn*);             // success
bn bn_mul(bn*, bn*);             // success
void bn_div_by_2(bn*);           // success
bn bn_fact(bn*);                 // success
bn bn_pow(bn*, bn*);             // success
bn bn_sum_from_to(bn*, bn*);

// MARK: - Support

int  bn_is_zero(bn*);            // success
void bn_dec(bn*);                // success
void bn_inc(bn*);                // success
int  bn_cmp(bn*, bn*);           // success
void bn_clear_nulls(bn*);        // success
void bn_copy(bn*, bn*);          // success

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

int priority(char operator) {
    switch (operator) {
        case '!':
            return 3;
        case '^':
            return 2;
        case '*':
            return 1;
        case '+':
            return 0;
        case '-':
            return 0;
        default:
            return -1;
    }
}

// MARK: - Операции

bn perform_operation(char operator, bn* first_num, bn* second_num) {
    bn result;
    bn_init(&result);
    
    switch (operator) {
        case '+':
            result = bn_add(first_num, second_num);
            break;
        case '-':
            result = bn_sub(first_num, second_num);
            break;
        case '*':
            result = bn_mul(first_num, second_num);
            break;
        case '^':
            result = bn_pow(first_num, second_num);
            break;
        case '!':
            result = bn_fact(first_num);
            break;
        default:
            break;
    }
    return result;
}

// MARK: - execute bn

bn execute_bn(char* str) {
    bn num;
    bn_init(&num);
    
    return num;
}

// MARK: - Вычисление выражения

void execute_expression(char* buffer) {
    
    char temp[2500], postfix[2500];
    char numbers[MAX_DIGITS][MAX_DIGITS];
    char operators[MAX_DIGITS];
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
    string_to_tokens(temp, numbers, operators);
    //    convert(temp, postfix);
    //    printf("%s\n", postfix);
    //    int result = evaluate(postfix);
    //    printf("%d", result);
    
}

// MARK: - Проверка символа на оператор

int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '^' || c == '!');
}

// MARK: - Проверка цифры

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

// MARK: - Push operator

void push(char item) {
    stack[++top] = item;
}

// MARK: - Pop operator

char pop(void) {
    return stack[top--];
}

// MARK: - Convert from infix to postfix

void convert(char infix[], char postfix[]) {
    int i, symbol, j = 0;
    stack[++top] = '#';
    
    for (i = 0; i < strlen(infix); i++) {
        symbol = infix[i];
        
        if (is_operator(symbol) == 0) {
            postfix[j] = symbol;
            j++;
        } else {
            if (symbol == '(') {
                push(symbol);
            } else {
                if (symbol == ')') {
                    while (stack[top] != '(') {
                        postfix[j] = pop();
                        j++;
                    }
                    pop(); // pop out ( bracket
                } else {
                    if (priority(symbol) > priority(stack[top])) {
                        push(symbol);
                    } else {
                        while (priority(symbol) <= priority(stack[top])) {
                            postfix[j] = pop();
                            j++;
                        }
                        push(symbol);
                    }
                }
            }
        }
    }
    
    while (stack[top] != '#') {
        postfix[j] = pop();
        j++;
    }
    postfix[j] = '\0';
}

// MARK: - Int stack

int stack_int[25];
int top_int = -1;

// MARK: - Push int

void push_int(int item) {
    stack_int[++top_int] = item;
}

// MARK: - Pop int

char pop_int(void) {
    return stack_int[top_int--];
}

int evaluate(char* postfix) {
    char ch;
    int i = 0, operand1, operand2;
    
    while ((ch = postfix[i++]) != '\0') {
        
        if (isdigit(ch)) {
            push_int(ch - '0');
        } else {
            operand2 = pop_int();
            operand1 = pop_int();
            
            switch (ch) {
                case '+':
                    push_int(operand1 + operand2);
                    break;
                default:
                    break;
            }
        }
        
    }
    return stack_int[top_int];
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

void string_to_tokens(char *expression, char numbers[][MAX_DIGITS], char operators[]) {
    
    char spacedExpression[2 * MAX_DIGITS];
    add_space(expression, spacedExpression);
    
    // Инициализируем счетчики
    int num_count = 0;
    int op_count = 0;
    
    // Разбиваем выражение на числа и операторы
    char *token = strtok(spacedExpression, " \n");
    while (token != NULL) {
        if (token[0] >= '0' && token[0] <= '9') {
            // Если токен - число, добавляем его в массив чисел
            strcpy(numbers[num_count], token);
            num_count++;
        } else if (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '!' || token[0] == '^') {
            // Если токен - оператор, добавляем его в массив операторов
            operators[op_count] = token[0];
            op_count++;
        }
        token = strtok(NULL, " \n");
    }
}
