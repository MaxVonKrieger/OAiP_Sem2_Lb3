#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "func_general.h"
#include <malloc.h>
#include <conio.h>
#include <time.h>
#include "task1.h"
#include "task2.h"

int main(int argc, const char* argv[])
{
    setlocale(LC_ALL, "RU");
    int i = 0, n = 0, vibor = 0, c = 0;
    float k = 0;

    while (1)
    {
        DrawMenu();
        printf_s("�������� ����� ����: ");
        vibor = intchar();

        switch (vibor)
        {
        case 1:
            printf("������� ����� ���� float: ");
            k = floatchar();
            n = get_integer_part(k);
            printf("����� ����� ����� %f ����� %d\n", k, n);
            break;

        case 2:
            workerStruct(argv[1]);
            break;

        case 3:
            printf("���������� ������ ���������... \n");
            return 0;

        default:
            printf_s("�������� �����. ������� ����� �� 1 �� 3.\n");
            break;
        }
    }

    return 0;
}