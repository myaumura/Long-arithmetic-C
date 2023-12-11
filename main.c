#include <stdio.h>
#include <string.h>
#define MAX_DIGITS 2500

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
int compare_signs_big_nums(big_num *num1, big_num *num2);
int compare_lenght(big_num *num1, big_num *num2);

int main(void) {
    
    big_num num1, num2, result;
    result.lenght = MAX_DIGITS;
    // Зануление результата
    memset(result.digits, 0x00, sizeof(short int) * MAX_DIGITS);
    
    char str1[MAX_DIGITS] = "-100";
    char str2[MAX_DIGITS] = "-10000";
    
    init_big_num(&num1, str1);
    init_big_num(&num2, str2);
    
    add_big_num(&result, &num1, &num2);
    //    sub_big_num(&result, &num1, &num2);
    //    mul_big_num(&result, &num1, &num2);
    print_big_num(&num1);
    print_big_num(&num2);
    //    printf("Sum: ");
    print_big_num(&result);
    return 0;
}

// MARK: Инициализация большого числа

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
    for (int i = 0; i < num->lenght; i++) {
        num->digits[num->lenght - i - 1] = str[i] - '0'; // num->digits[i] как альтернатива
    }
}

// num->digits == (*num).digits

// MARK: Вывод большого числа

void print_big_num(big_num *num) {
    if (num->sign == -1) {
        printf("-");
    }
    // потому что идем справа налево
    for (int i = num->lenght - 1 ; i >= 0; i--) {
        printf("%d", num->digits[i]);
    }
    printf("\n");
}

// MARK: Сложение больших чисел

void add_big_num(big_num *result, big_num *first_num, big_num *second_num) {
    
    // Флаг для переноса числа
    short int carry_flag = 0,sum = 0;
    
    for (int i = 0; i < MAX_DIGITS; i++) {
        
        switch (compare_signs_big_nums(first_num, second_num)) {
            case 1: // если первое число больше второго
            if (first_num->digits[i] == 0 && second_num->digits[i] > 0) {
                sum = first_num->digits[i] * first_num->sign + second_num->digits[i] * second_num->sign + carry_flag + 10;
                carry_flag = -1;
            } else {
                sum = first_num->digits[i] * first_num->sign + second_num->digits[i] * second_num->sign + carry_flag;
            }
                break;
            case -1: // если первое число меньше второго
            sum = first_num->digits[i] * first_num->sign + second_num->digits[i] * second_num->sign + carry_flag;
                break;
            case 0: // знаки равны
            sum = first_num->digits[i] * first_num->sign + second_num->digits[i] * second_num->sign + carry_flag;
                break;
        }
        // Проверка суммы на отрицательное число
        if (sum < 0) {
            sum *= -1;
            result->sign = -1;
        }
        carry_flag = sum / 10;
        result->digits[i] = sum % 10;
    }
    while (result->digits[result->lenght - 1] == 0 && result->lenght > 1) {
        result->lenght--;
    } // Убираем все нули перед числом
}

// MARK: Вычитание больших чисел

void sub_big_num(big_num * result, big_num *first_num, big_num *second_num) {
    
    short int carry_flag = 0;
    
    for (int i = 0; i < MAX_DIGITS; i++) {
        int sub = first_num->digits[i] - second_num->digits[i] - carry_flag; // базовый случай, если первое число больше второго
        // int sub = second_num->digits[i] - first_num->digits[i] - carry_flag; // случай, когда второе число меньше первого
        // нужно учитывать знаки, например если второе отрицательное, то должна происходить сумма и знак меняться
        if (sub < 0) {
            sub += 10;
            carry_flag = 1;
        } else {
            carry_flag = 0;
        }
        result->digits[i] = sub;
    }
    while (result->digits[result->lenght - 1] == 0 && result->lenght > 1) {
        result->lenght--;
    }
}

// MARK: Умножение больших чисел

void mul_big_num(big_num * result, big_num * first_number, big_num * second_num) {
    int carry_flag = 0;
    for(int i = 0; i < MAX_DIGITS; i++) {
        for(int j = 0; j < MAX_DIGITS; j++){
            int mul = first_number->digits[i] * second_num->digits[j] + carry_flag;
            result->digits[i] = mul % 10;
            carry_flag = mul / 10;}
    }
    while (result->digits[result->lenght - 1] == 0 && result->lenght > 1) {
        result->lenght--;
    }
}

// Сравнение двух знаков
int compare_signs_big_nums(big_num *num1, big_num *num2) {
    if (num1->sign > num2->sign) return 1;
    else if(num1->sign < num2->sign) return -1;
    else return 0;
}

int compare_lenght(big_num *num1, big_num *num2) {
    if (num1->lenght >= num2->lenght) {
        return 1;
    } else return 0;
}

