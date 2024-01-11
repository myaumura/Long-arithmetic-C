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
    short int digits[MAX_DIGITS];
    short int length;
} bn;

// MARK: - Init

void create_bn(bn*, char *);            // success
void init_bn(bn*);
void print_bn(bn*);                     // success
void read_expression(void);

// MARK: - Operaions

void add_bn(bn*, bn*, bn*);             // success
void sub_bn(bn*, bn*, bn*);             // success
void mul_bn(bn*, bn*, bn*);             // success

void fact_bn(bn*, bn*);
void pow_bn(bn*, bn*, short int);       // success
void sum_bn_from_to(bn*, bn*, bn*);

// MARK: - Support

int bn_is_zero(bn*);
void bn_dec(bn*);
void clear_nulls(bn*);                  // success
void copy_bn(bn*, bn*);

// MARK: - Main

int main(void) {
    
    char str1[MAX_DIGITS] = "10", str2[MAX_DIGITS] = "3131";
    bn num1, num2, result;
    
    create_bn(&num1, str1);
    create_bn(&num2, str2);
    init_bn(&result);
//    mul_bn(&result, &num1, &num2);
    pow_bn(&result, &num1, 0);
//    bn_dec(&num1);
//    copy_bn(&num1, &result);
//    fact_bn(&result, &num1);
    print_bn(&result);
    return EXIT_SUCCESS;
}

// MARK: - Инициализация большого числа

void init_bn(bn * bn) {
//    memset(bn->digits, 0, sizeof(bn->digits) * MAX_DIGITS);
    for (int i = 0; i < MAX_DIGITS; i++) {
        bn->digits[i] = 0;
    }
    bn->length = 2500;
}

// MARK: - Создание большого числа

void create_bn(bn *num, char *str) {
    num->length = strlen(str);
    memset(num->digits, 0x00, sizeof(short int) * MAX_DIGITS);
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

void fact_bn(bn* result, bn* number) {
    
    bn temp;
    copy_bn(number, &temp);
    bn_dec(number);
    
    while (!bn_is_zero(number)) {
        mul_bn(result, &temp, number);
        bn_dec(number);
        copy_bn(result, &temp);
        init_bn(result);
    }
    copy_bn(&temp, result);
}

// MARK: - Возведение в степень большого числа

void pow_bn(bn* result, bn* number, short int exp) {
    
    if (exp == 0) {
        result->digits[0] = 1;
        result->length = 1;
    } else {
        bn temp;
        init_bn(&temp);
        copy_bn(number, &temp);
        
        for (int i = 0; i < exp; i++) {
            mul_bn(result, &temp, number);
            exp--;
            copy_bn(result, &temp);
            init_bn(result);
        }
        copy_bn(&temp, result);
    }
}

// MARK: - Сумма всех чисел от большого числа до большого числа

void sum_bn_from_to(bn* result, bn* first_num, bn* second_num) {
    
}

// MARK: - Проверка числа на ноль

int bn_is_zero(bn* n) {

    for (int i = 0; i < n->length; i++) {
    if (n->digits[i]) {
      return 0;
    }
  }
  return 1;
}

// MARK: - Уменьшение числа на единицу

void bn_dec(bn *num) {
      for (int i = 0; i < num->length; i++) {
        int tmp = num->digits[i];
        int res = tmp - 1;
          num->digits[i] = res;

        if (!(res > tmp))
        {
          break;
        }
      }
}

// MARK: - Удаление нулей перед числом

void clear_nulls(bn * big_num) {
    while (big_num->digits[big_num->length - 1] == 0 && big_num->length > 1) big_num->length--;
}

// MARK: - Функция копирования структуры

void copy_bn(bn* number, bn* copy) {
    for(int i = 0; i < number->length; i++) {
        copy->digits[i] = number->digits[i];
    }
    copy->length = number->length;
}

