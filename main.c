#include <stdio.h>
#include <string.h>

#define MAX_DIGITS 2500
#define SUM '+'
#define SUB '-'
#define MUL '*'
#define EXP '^'
#define FAC '!'

typedef struct {
    short int digits[MAX_DIGITS];
    short int lenght;
    char sign; // если число отрицательное, то sign = -1, иначе sign = 1
} bn;

// MARK: - Init

void init_bn(bn*, char *);              // success
void print_bn(bn*);                     // success
void read_expression(void);

// MARK: - Operaions

void add_bn(bn*, bn*, bn*);             // success
void sub_bn(bn*, bn*, bn*);             // success
void mul_bn(bn*, bn*, bn*);             // success
void fact_bn(bn*, bn*);
void pow_bn(bn*, bn*, bn*);
void sum_bn_from_to(bn*, bn*, bn*);

// MARK: - Support

void clear_nulls(bn*);                  // success
void copy_bn(bn*, bn*);

int main(void) {
    
    bn num1, num2, result;
    result.lenght = MAX_DIGITS;
    memset(result.digits, 0x00, sizeof(short int) * MAX_DIGITS);
    
    char str1[MAX_DIGITS] = "2";
    char str2[MAX_DIGITS] = "420";
    
    init_bn(&num1, str1);
    init_bn(&num2, str2);
    
//    add_big_num(&result, &num1, &num2);
//        sub_big_num(&result, &num1, &num2);
//        mul_big_num(&result, &num1, &num2);
//    pow_big_num(&result, &num1, 3);
    print_bn(&num1);
    print_bn(&num2);
//    copy_big_num(&num1, &num2);
//    print_big_num(&num1);
//    print_big_num(&num2);
    print_bn(&result);
    
    return 0;
}

// MARK: - Инициализация большого числа

void init_bn(bn *num, char *str) {
    if (str[0] == '-') {
        num->sign = -1; str++;
    } else num->sign = 1;
    num->lenght = strlen(str);
    memset(num->digits, 0x00, sizeof(short int) * MAX_DIGITS);
    for (short int i = 0; i < num->lenght; i++) num->digits[num->lenght - i - 1] = str[i] - '0';
}

// MARK: - Вывод большого числа

void print_bn(bn *num) {
    if (num->sign == -1) printf("-");
    for (short int i = num->lenght - 1 ; i >= 0; i--) printf("%d", num->digits[i]);  // потому что идем справа налево <--
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
        
        if (first_num->lenght >= second_num->lenght) {
            sub = first_num->digits[i] - second_num->digits[i] - carry_flag;
        } else if (second_num->lenght > first_num->lenght) {
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
    
    for(short int i = 0; i < first_number->lenght; i++) {
        for (short int j = 0; j < second_num->lenght; j++) {
            current = first_number->digits[i] * second_num->digits[j];
            result->digits[i + j] += current;
        }
    }
    for(short int i = 0; i < result->lenght; i++) {
        result->digits[i + 1] +=  result->digits[i] / 10;
        result->digits[i] %= 10;
    }
    clear_nulls(result);
}

// MARK: - Факториал большого числа

void fact_bn(bn* result, bn* first_number) {
 
}

// MARK: - Возведение в степень большого числа

void pow_bn(bn* result, bn* number, bn* exp) {
    
    bn temp = *number;
    
    if (exp == 0) { // при нулевой степени возвращает 1; num ^ 0 = 1
        result->digits[0] = 1;
        result->lenght = 1;
    } else {
        
//        short int count = 0, current = 0;
        
        while (exp > 0) {
            
            mul_bn(&temp, &temp, number);
            exp--;
        }
        clear_nulls(&temp);
        print_bn(&temp);
    }
    result = &temp;
    clear_nulls(result);
}

// MARK: - Сумма всех чисел от большого числа до большого числа

void sum_bn_from_to(bn* result, bn* first_num, bn* second_num) {
    
}

// MARK: - Удаление нулей перед числом

void clear_nulls(bn * big_num) {
    while (big_num->digits[big_num->lenght - 1] == 0 && big_num->lenght > 1) big_num->lenght--;
}

// MARK: - Функция копирования структуры

void copy_bn(bn* number, bn* copy) {
    for(int i = 0; i < number->lenght; ++i) {
        copy->digits[i] = number->digits[i];
    }
    copy->sign = number->sign;
    copy->lenght = number->lenght;
}
