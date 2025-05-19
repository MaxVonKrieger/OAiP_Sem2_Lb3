#pragma once

typedef union {
    float f;
    struct {
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;
    } bits;
} FloatUnion;

int get_integer_part(float num);