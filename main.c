#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 2500
#define SUM '+'
#define SUB '-'
#define MUL '*'
#define EXP '^'
#define FAC '!'

typedef struct {
    int digits[MAX_DIGITS];
    short int length;
    short int sign;
} bn;

// MARK: - Expression

void read_expression(void);

// MARK: - Init

void bn_create(bn*, char*);             // success
void bn_init(bn*);                      // success
void bn_print(bn*);                     // success

// MARK: - Operaions

void bn_add(bn*, bn*, bn*);             // success
void bn_sub(bn*, bn*, bn*);             // success
void bn_mul(bn*, bn*, bn*);             // success
void bn_div_by_2(bn*);
void bn_fact(bn*, bn*);                 // success
void bn_pow(bn*, bn*, bn*);             // success
void bn_sum_from_to(bn*, bn*, bn*);

// MARK: - Support

int  bn_is_zero(bn*);                   // success
void bn_dec(bn*);                       // success
void bn_inc(bn*);                       // success
int  bn_cmp(bn*, bn*);                  // success
void bn_clear_nulls(bn*);                  // success
void bn_copy(bn*, bn*);                 // success

// MARK: - Main

int main(void) {
    
    char str1[MAX_DIGITS] = "101", str2[MAX_DIGITS] = "101";
    bn num1, num2, result;
    
    bn_create(&num1, str1);
    bn_create(&num2, str2);
    bn_init(&result);
    
    /* test examples
     bn_sub(&result, &num1, &num2);
     bn_pow(&result, &num1, &num2);
     bn_fact(&result, &num1);
     bn_mul(&result, &num1, &num2);
     bn_pow(&result, &num1, &num2);
     bn_sum_from_to(&result, &num1, &num2);
     */
    
    bn_print(&result);
    return EXIT_SUCCESS;
}

// MARK: - Инициализация большого числа

void bn_init(bn * num) {
    memset(num->digits, 0x00, sizeof(int) * MAX_DIGITS);
    num->length = 2500;
}

// MARK: - Создание большого числа

void bn_create(bn *num, char *str) {
    if (str[0] == '-') {
        num->sign = -1;
        str++;
    } else num->sign = 1;
    num->length = strlen(str);
    memset(num->digits, 0x00, sizeof(int) * MAX_DIGITS);
    for (short int i = 0; i < num->length; i++) num->digits[num->length - i - 1] = str[i] - '0';
}

// MARK: - Вывод большого числа

void bn_print(bn *num) {
    for (short int i = num->length - 1 ; i >= 0; i--) printf("%d", num->digits[i]);  // потому что идем справа налево <--
    printf("\n");
}

// MARK: - Сложение больших чисел

void bn_add(bn *result, bn *first_num, bn *second_num) {
    short int carry_flag = 0,sum = 0;
    
    for (int i = 0; i < MAX_DIGITS; i++) {
        sum = first_num->digits[i] + second_num->digits[i] + carry_flag;
        carry_flag = sum / 10;
        result->digits[i] = sum % 10;
    }
    bn_clear_nulls(result);
}

// MARK: - Вычитание больших чисел

void bn_sub(bn* result, bn *first_num, bn *second_num) {
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
                    
                case -1:
                    sub = second_num->digits[i] - first_num->digits[i] - carry_flag;
            }
        }
        
        if (sub < 0) {
            sub += 10;
            carry_flag = 1;
        } else {
            carry_flag = 0;
        }
        result->digits[i] = sub;
    }
    bn_clear_nulls(result);

    //    size_t res, temp1, temp2, borrow = 0;
    //
    //    for (int i = 0; i < MAX_DIGITS; i++) {
    //        temp1 = (size_t)first_num->digits[i] + (SIZE_MAX + 1); /* + number_base */
    //        temp2 = (size_t)second_num->digits[i] + borrow;
    //        res = temp1 - temp2;
    //        result->digits[i] = res % SIZE_MAX; /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
    //        borrow = (res <= SIZE_MAX);
    //    }
    //    bn_clear_nulls(result);
}

// MARK: - Умножение больших чисел

void bn_mul(bn* result, bn* first_number, bn* second_num) {
    bn temp1, temp2;
    bn_copy(first_number, &temp1);
    bn_copy(second_num, &temp2);
    bn_init(result);
    
    short int current = 0;
    
    for(short int i = 0; i < temp1.length; i++) {
        for (short int j = 0; j < temp2.length; j++) {
            current = temp1.digits[i] * temp2.digits[j];
            result->digits[i + j] += current;
        }
    }
    for(short int i = 0; i < result->length - 1; i++) {
        result->digits[i + 1] +=  result->digits[i] / 10;
        result->digits[i] %= 10;
    }
    bn_clear_nulls(result);
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

void bn_fact(bn* result, bn* num) {
    bn temp;
    bn_copy(num, &temp);
    bn_dec(num);
    
    while (!bn_is_zero(num)) {
        bn_mul(result, &temp, num);
        bn_dec(num);
        bn_copy(result, &temp);
        bn_init(result);
    }
    bn_copy(&temp, result);
}

// MARK: - Возведение в степень большого числа

void bn_pow(bn* result, bn* num, bn* exp) {
    if (bn_is_zero(exp)) {
        result->digits[0] = 1;
        result->length = 1;
    } else if ((num->length == 1) && (num->digits[0] == 1)) {
        bn_copy(num, result);
    } else {
        bn temp;
        bn_init(&temp);
        bn_copy(num, &temp);
        bn_dec(exp);
        
        while (!bn_is_zero(exp)) {
            bn_mul(result, &temp, num);
            bn_dec(exp);
            bn_copy(result, &temp);
            bn_init(result);
        }
        bn_copy(&temp, result);
    }
}

// MARK: - Сумма всех чисел от и до

void bn_sum_from_to(bn* result, bn* first_num, bn* second_num) {
    bn temp;
    bn_init(&temp);
    
    bn_copy(second_num, result);
    bn_add(result, result, first_num);
    
    if (first_num->sign == -1) {
        bn_add(&temp, second_num, first_num);
    } else {
        bn_sub(&temp, second_num, first_num);
    }
    
    bn_inc(&temp);
    bn_mul(result, result, &temp);
    bn_div_by_2(result);
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

void read_expression(void) {
    
}
