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
} bn;

// MARK: - Expression

void read_expression(void);

// MARK: - Init

void create_bn(bn*, char*);             // success
void init_bn(bn*);                      // success
void print_bn(bn*);                     // success
void read_expression(void);

// MARK: - Operaions

void add_bn(bn*, bn*, bn*);             // success
void sub_bn(bn*, bn*, bn*);             // success
void mul_bn(bn*, bn*, bn*);             // success

void fact_bn(bn*, bn*);                 // success
void pow_bn(bn*, bn*, bn*);             // success
void sum_bn_from_to(bn*, bn*, bn*);

// MARK: - Support

int  bn_is_zero(bn*);                   // success
void bn_dec(bn*);                       // success
void bn_inc(bn*);                       // success
int  bn_cmp(bn*, bn*);                  // success
void clear_nulls(bn*);                  // success
void copy_bn(bn*, bn*);                 // success

// MARK: - Main

int main(void) {
    
    char str1[MAX_DIGITS] = "2", str2[MAX_DIGITS] = "100";
    bn num1, num2, result;
    
    create_bn(&num1, str1);
    create_bn(&num2, str2);
    init_bn(&result);
//    fact_bn(&result, &num1);
//    pow_bn(&result, &num1, &num2);
//    bn_dec(&num1);
    print_bn(&result);
    return EXIT_SUCCESS;
}

// MARK: - Инициализация большого числа

void init_bn(bn * num) {
    memset(num->digits, 0x00, sizeof(int) * MAX_DIGITS);
    num->length = 2500;
}

// MARK: - Создание большого числа

void create_bn(bn *num, char *str) {
    num->length = strlen(str);
    memset(num->digits, 0x00, sizeof(int) * MAX_DIGITS);
    for (short int i = 0; i < num->length; i++) num->digits[num->length - i - 1] = str[i] - '0';
}

// MARK: - Вывод большого числа

void print_bn(bn *num) {
    for (short int i = num->length - 1 ; i >= 0; i--) printf("%d", num->digits[i]);  // потому что идем справа налево <--
    printf("\n");
}

// MARK: - Сложение больших чисел

void add_bn(bn *result, bn *first_num, bn *second_num) {
    short int carry_flag = 0,sum = 0;
    
    for (int i = 0; i < MAX_DIGITS; i++) {
        sum = first_num->digits[i] + second_num->digits[i] + carry_flag;
        carry_flag = sum / 10;
        result->digits[i] = sum % 10;
    }
    clear_nulls(result);
}

// MARK: - Вычитание больших чисел

void sub_bn(bn * result, bn *first_num, bn *second_num) {
    short int carry_flag = 0;
    
    for(short int i = 0; i < MAX_DIGITS; i++) {
        short int sub = 0;
        
        if (first_num->length >= second_num->length) {
            sub = first_num->digits[i] - second_num->digits[i] - carry_flag;
        } else if (second_num->length > first_num->length) {
            sub = second_num->digits[i] - first_num->digits[i] - carry_flag;
        }
        
        if (sub < 0) {
            sub += 10;
            carry_flag = 1;
        } else {
            carry_flag = 0;
        }
        result->digits[i] = sub;
    }
    clear_nulls(result);
}

// MARK: - Умножение больших чисел

void mul_bn(bn * result, bn * first_number, bn * second_num) {
    
    short int current = 0;
    
    for(short int i = 0; i < first_number->length; i++) {
        for (short int j = 0; j < second_num->length; j++) {
            current = first_number->digits[i] * second_num->digits[j];
            result->digits[i + j] += current;
        }
    }
    for(short int i = 0; i < result->length - 1; i++) {
        result->digits[i + 1] +=  result->digits[i] / 10;
        result->digits[i] %= 10;
    }
    clear_nulls(result);
}

// MARK: - Факториал большого числа

void fact_bn(bn* result, bn* num) {
    
    bn temp;
    copy_bn(num, &temp);
    bn_dec(num);
    
    while (!bn_is_zero(num)) {
        mul_bn(result, &temp, num);
        bn_dec(num);
        copy_bn(result, &temp);
        init_bn(result);
    }
    copy_bn(&temp, result);
}

// MARK: - Возведение в степень большого числа

void pow_bn(bn* result, bn* num, bn* exp) {
    
    if (bn_is_zero(exp)) {
        result->digits[0] = 1;
        result->length = 1;
    } else {
        bn temp;
        init_bn(&temp);
        copy_bn(num, &temp);
        bn_dec(exp);
        
        while (!bn_is_zero(exp)) {
            mul_bn(result, &temp, num);
            bn_dec(exp);
            copy_bn(result, &temp);
            init_bn(result);
        }
        copy_bn(&temp, result);
    }
}

// MARK: - Сумма всех чисел от и до

void sum_bn_from_to(bn* result, bn* first_num, bn* second_num) {
    
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
    clear_nulls(num);
}

// MARK: - Увеличение числа на единицу

void bn_inc(bn* num) {
    int res, temp;
    
    for (int i = 0; i < num->length; i++) {
        temp = num->digits[i];
        res = temp + 1;
        num->digits[i] = res;

        if (res > temp) break;
    }
}

// MARK: - Сравнение больлих чисел

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

void clear_nulls(bn* num) {
    while (num->digits[num->length - 1] == 0 && num->length > 1) num->length--;
}

// MARK: - Функция копирования структуры

void copy_bn(bn* src, bn* dst) {
    for(int i = 0; i < src->length; i++) {
        dst->digits[i] = src->digits[i];
    }
    dst->length = src->length;
}

// MARK: - Чтение выражения

void read_expression(void) {
    
}
