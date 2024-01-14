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

// MARK: - Expression

void parse_expression(void);
bn perform_operation(char, bn*, bn*);
void execute_expression(char*, bn*);

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
    char buffer[2500];
    bn result, num1, num2;
    
    num1 = bn_create("1231");
    num2 = bn_create("4241");
    
    result = bn_sub(&num1, &num2);
    bn_print(&result);
    
//    fgets(buffer, sizeof(buffer), stdin);
//    parse_expression();
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
    int pos = 0;
    
    while (buffer[pos] != '\0') {
        if (buffer[pos] != '!') {
            getchar();
        }
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
            result = bn_add(first_num, second_num);
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

// MARK: - Вычисление выражения

void execute_expresssion(char* expression, bn* result) { }
