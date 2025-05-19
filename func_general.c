#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int strDlin(const char* str)
{
    int length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

void centrirovText(const char* text)
{
    int consoleWidth = 80;
    int textLength = strDlin(text);
    int padding = (consoleWidth - textLength) / 2;
    for (int i = 0; i < padding; i++)
    {
        printf_s(" ");
    }
    printf("%s", text);
    for (int i = 0; i < padding; i++)
    {
        printf_s(" ");
    }
    printf_s("\n");
}

void DrawMenu()
{
    centrirovText(" ____________________________________________________ ");
    centrirovText("|                                                    |");
    centrirovText("|                        МЕНЮ                        |");
    centrirovText("|____________________________________________________|");
    centrirovText("|                                                    |");
    centrirovText("| 1. Найти целую часть числа с плавающей точкой      |");
    centrirovText("| 2. Структура Работник                              |");
    centrirovText("| 3. Выход из программы                              |");
    centrirovText("|____________________________________________________|");
}

void DrawSubMenu()
{
    centrirovText(" _____________________________ ");
    centrirovText("|                             |");
    centrirovText("|         СПОСОБ ВВОДА        |");
    centrirovText("|_____________________________|");
    centrirovText("|                             |");
    centrirovText("| 1. Случайный ввод           |");
    centrirovText("| 2. Ручной ввод              |");
    centrirovText("|_____________________________|");
}

float floatchar() {
    char k;
    int i = 0;
    int has_decimal_point = 0;  // Флаг наличия точки/запятой
    int decimal_places = 0;     // Количество цифр после точки
    int is_negative = 0;        // Флаг отрицательного числа
    float result = 0.0f;        // Результат
    float decimal_multiplier = 1.0f; // Множитель для дробной части

    // Первый символ (знак или цифра)
    k = _getch();

    // Обработка знака
    if (k == '+' || k == '-') {
        printf("%c", k);
        is_negative = (k == '-');
        k = _getch();  // Считываем следующий символ
    }
    else {
        printf("+");  // Если знак не введён, подразумеваем '+'
    }

    // Основной цикл ввода
    while (1) {
        // Цифры
        if (k >= '0' && k <= '9') {
            printf("%c", k);
            if (!has_decimal_point) {
                result = result * 10 + (k - '0');
            }
            else {
                decimal_places++;
                decimal_multiplier *= 0.1f;
                result += (k - '0') * decimal_multiplier;
            }
        }
        // Точка или запятая (только одна)
        else if ((k == '.' || k == ',') && !has_decimal_point) {
            printf(".");
            has_decimal_point = 1;
        }
        // Enter (завершение ввода, если есть хотя бы одна цифра)
        else if (k == '\r' && i >= 1) {
            printf("\n");
            break;
        }
        // Backspace (удаление последнего символа)
        else if (k == '\b' && i >= 1) {
            printf("\b \b");
            if (has_decimal_point && decimal_places > 0) {
                decimal_places--;
                decimal_multiplier /= 0.1f;
                // Вычисляем обратно целую часть и обновляем результат
                // (Это сложно сделать точно, поэтому лучше ограничить Backspace)
            }
            else {
                result = (int)(result / 10);  // Грубое удаление последней цифры
            }
            i--;
            continue;
        }

        i++;
        k = _getch();
    }

    return is_negative ? -result : result;
}

int intchar()
{
    char k;
    int l = 0, i = 0;
    char* mas = (char*)malloc(1);
    if (mas == NULL)
    {
        printf("Ошибка выделения памяти при вводе\n");
        return 1;
    }

    while (1) {
        k = _getch();
        if (k >= '0' && k <= '9')
        {
            printf("%c", k);
            mas[i] = k;
            i++;
            char* temp = (char*)realloc(mas, i + 1);
            if (temp == NULL)
            {
                printf("Ошибка перераспределения памяти при вводе\n");
                free(mas);
                return 1;
            }
            mas = temp;
            mas[i] = '\0';
        }
        else if (k == '\r' && i != 0)
        {
            printf("\n");
            break;
        }
        else if (k == '\b')
        {
            if (i > 0)
            {
                i--;
                mas[i] = '\0';
                printf("\b \b");
            }
        }
    }

    for (int j = 0; j < i; j++)
    {
        l = l * 10 + (mas[j] - '0');
    }
    free(mas);
    return l;
}