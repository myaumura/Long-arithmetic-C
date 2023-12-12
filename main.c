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
} big_num;

void init_big_num(big_num *num, char *str);
void print_big_num(big_num *num);
void add_big_num(big_num *result, big_num *a, big_num *b);
void sub_big_num(big_num*, big_num*, big_num*);
void mul_big_num(big_num*, big_num*, big_num*);
void factorial_big_num(big_num*, big_num*);
void pow_big_num(big_num*, big_num*, int);
int compare_signs_big_nums(big_num *num1, big_num *num2);
int compare_lenght(big_num *num1, big_num *num2);

int main(void) {
    
    big_num num1, num2, result;
    result.lenght = MAX_DIGITS;
    // Зануление результата
    memset(result.digits, 0x00, sizeof(short int) * MAX_DIGITS);
    
    char str1[MAX_DIGITS] = "10";
    char str2[MAX_DIGITS] = "12";
    
    init_big_num(&num1, str1);
    init_big_num(&num2, str2);
    
//    add_big_num(&result, &num1, &num2);
//        sub_big_num(&result, &num1, &num2);
//        mul_big_num(&result, &num1, &num2);
    pow_big_num(&result, &num1, 5);
    print_big_num(&num1);
    print_big_num(&num2);
    //    printf("Sum: ");
    print_big_num(&result);
    return 0;
}

// MARK: - Инициализация большого числа

void init_big_num(big_num *num, char *str) {
    // Определяем наличие знака
    if (str[0] == '-') {
        num->sign = -1;
        str++;
    } else num->sign = 1;
    // Получаем размер числа
    num->lenght = strlen(str);
    // Зануляем массив
    memset(num->digits, 0x00, sizeof(short int) * MAX_DIGITS);
    // Заполняем массив цифрами из входного массива
    for (short int i = 0; i < num->lenght; i++) {
        num->digits[num->lenght - i - 1] = str[i] - '0'; // num->digits[i] как альтернатива
    }
}

// num->digits == (*num).digits

// MARK: - Вывод большого числа

void print_big_num(big_num *num) {
    if (num->sign == -1) {
        printf("-");
    }
    // потому что идем справа налево
    for (short int i = num->lenght - 1 ; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
    printf("\n");
}

// MARK: - Сложение больших чисел

void add_big_num(big_num *result, big_num *first_num, big_num *second_num) {
    // Флаг для переноса числа
    short int carry_flag = 0,sum = 0;
    
    for (int i = 0; i < MAX_DIGITS; i++) {
        sum = first_num->digits[i] + second_num->digits[i] + carry_flag;
        carry_flag = sum / 10;
        result->digits[i] = sum % 10;
    }
    while (result->digits[result->lenght - 1] == 0 && result->lenght > 1) result->lenght--;// Убираем все нули перед числом
}

// MARK: - Вычитание больших чисел

void sub_big_num(big_num * result, big_num *first_num, big_num *second_num) {
    
    short int carry_flag = 0;
    
    for (short int i = 0; i < MAX_DIGITS; i++) {
    
        short int sub = 0;
        
        if (first_num->lenght >= second_num->lenght) {
            sub = first_num->digits[i] - second_num->digits[i] - carry_flag;
        } else if (second_num->lenght > first_num->lenght) {
            sub = second_num->digits[i] - first_num->digits[i] - carry_flag;
        } // нужно пофиксить случай если длина одинаковая а второе число больше
        
        if (sub < 0) {
            sub += 10;
            carry_flag = 1;
        } else {
            carry_flag = 0;
        }
        result->digits[i] = sub;
    }
    while (result->digits[result->lenght - 1] == 0 && result->lenght > 1) result->lenght--;
}

// MARK: - Умножение больших чисел

void mul_big_num(big_num * result, big_num * first_number, big_num * second_num) {
    
    short int current = 0;
    
    for (short int i = 0; i < first_number->lenght; i++) {
        for (short int j = 0; j < second_num->lenght; j++) {
            current = first_number->digits[i] * second_num->digits[j];
            result->digits[i + j] += current;
        }
    }
    
    for (short int i = 0; i < result->lenght; i++) {
        result->digits[i + 1] +=  result->digits[i] / 10;
        result->digits[i] %= 10;
    }
    
    while (result->digits[result->lenght - 1] == 0 && result->lenght > 1) result->lenght--;
}

// MARK: - Факториал большого числа

void factorial_big_num(big_num* result, big_num* first_number) {
 
}

// MARK: - Возведение в степень большого числа

void pow_big_num(big_num* result, big_num* first_number, int exp) {
    
    short int count = 0, current = 0;
    
    while (count < exp) {
        
        for (short int i = 0; i < first_number->lenght; i++) {
            for (short int j = 0; j < first_number->lenght; j++) {
                current = first_number->digits[i] * first_number->digits[j];
                result->digits[i + j] += current;
            }
        }
        
        for (short int i = 0; i < result->lenght; i++) {
            result->digits[i + 1] +=  result->digits[i] / 10;
            result->digits[i] %= 10;
        }
    }

    while (result->digits[result->lenght - 1] == 0 && result->lenght > 1) result->lenght--;
}


