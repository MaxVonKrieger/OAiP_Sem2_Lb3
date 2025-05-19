#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "task1.h"


int get_integer_part(float num) {
    FloatUnion fu = { .f = num };
    int exponent = fu.bits.exponent - 127;  // �������� ����������

    if (exponent < 0) return 0;  // ����� |num| < 1.0

    // ����� ��� ���������� ����� �����
    unsigned int mask = 0x7FFFFF >> (23 - exponent);
    int integer = (fu.bits.mantissa >> (23 - exponent)) | (1 << exponent);

    return (fu.bits.sign) ? -integer : integer;
}
