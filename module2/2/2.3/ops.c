#include <math.h>
#include "ops.h"

double op_add(int argc, double argv[]) {
    double sum = 0;
    for (int i = 0; i < argc; i++) 
        sum += argv[i];
    return sum;
}

double op_sub(int argc, double argv[]) {
    if (argc < 2) return 0;
    double result = argv[0];
    for (int i = 1; i < argc; i++)
        result -= argv[i];
    return result;
}

double op_mul(int argc, double argv[]) {
    double result = 1;
    for (int i = 0; i < argc; i++)
        result *= argv[i];
    return result;
}

double op_div(int argc, double argv[]) {
    if (argc < 2 || argv[1] == 0.0)
        return NAN;
    double result = argv[0];
    for (int i = 1; i < argc; i++) {
        if (argv[i] == 0.0) return NAN;
        result /= argv[i];
    }
    return result;
}

double op_pow(int argc, double argv[]) {
    if (argc < 2) return 0;
    return pow(argv[0], argv[1]);
}

double op_sqrt(int argc, double argv[]) {
    (void)argc;
    if (argv[0] < 0.0) return NAN;
    return sqrt(argv[0]);
}