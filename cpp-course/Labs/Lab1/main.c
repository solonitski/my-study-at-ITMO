#include "return_codes.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    bool sign;
    int exponent;
    uint64_t mantissa;
} FloatingPoint;

void setConstants(char format, int *MANTISSA_SIZE, int *SHIFT,
                  FloatingPoint *NaN, FloatingPoint *POSITIVE_INF, FloatingPoint *NEGATIVE_INF,
                  FloatingPoint *POSITIVE_NULL, FloatingPoint *NEGATIVE_NULL) {
    if (format == 'f') {
        *MANTISSA_SIZE = 23;
        *SHIFT = 127;
        *NaN = (FloatingPoint){0, 0xFF, 0x7FFFFF};
        *POSITIVE_INF = (FloatingPoint){0, 0xFF, 0};
        *NEGATIVE_INF = (FloatingPoint){1, 0xFF, 0};
    } else {
        *MANTISSA_SIZE = 10;
        *SHIFT = 15;
        *NaN = (FloatingPoint){0, 0x1F, 0x3FF};
        *POSITIVE_INF = (FloatingPoint){0, 0x1F, 0};
        *NEGATIVE_INF = (FloatingPoint){1, 0x1F, 0};
    }
    *POSITIVE_NULL = (FloatingPoint){0, 0, 0};
    *NEGATIVE_NULL = (FloatingPoint){1, 0, 0};
}

int parseNumber(FloatingPoint *number, const char *str, char format) {
    char *endptr;
    uint32_t num = strtoul(str, &endptr, 16);
    if (endptr == str) {
        fprintf(stderr, "Unable to find a number.\n");
        return ERROR_ARGUMENTS_INVALID;
    } else if (*endptr != '\0' || strncmp(str, "0x", 2) != 0) {
        fprintf(stderr, "Invalid number format.\n");
        return ERROR_ARGUMENTS_INVALID;
    }

    if (format == 'f') {
        number->sign = (num >> 31) & 1;
        number->exponent = (int) ((num >> 23) & 0xFF);
        number->mantissa = num & 0x7FFFFF;
    } else {
        number->sign = (num >> 15) & 1;
        number->exponent = (int) ((num >> 10) & 0x1F);
        number->mantissa = num & 0x3FF;
    }

    return SUCCESS;
}

int typeOfNumber(FloatingPoint number, char format) {
    int SHIFT = format == 'f' ? 127 : 15;

    if (number.exponent == 0) {
        if (!number.mantissa) {
            return !number.sign ? 0 : 1;// 0 or -0
        } else {
            return 2;// denormalize
        }
    } else if (number.exponent == (2 * SHIFT + 1)) {
        if (number.mantissa) {
            return 3;// NaN
        } else {
            return number.sign ? 4 : 5;// -inf or +inf
        }
    }
    return 6;// default
}

int binary_number_length(uint64_t n) {
    int length = 0;
    if (n == 0) {
        return 1;
    }

    while (n > 0) {
        length++;
        n >>= 1;
    }

    return length;
}

void normalize(uint64_t *mantissa, int *exponent, char format) {
    int MANTISSA_SIZE = format == 'f' ? 23 : 10;

    int whereIsOne = binary_number_length(*mantissa);
    int countOfLeadingNulls = (MANTISSA_SIZE - whereIsOne) > 0 ? (MANTISSA_SIZE - whereIsOne) : 0;

    if ((*exponent - countOfLeadingNulls - 1) > 0) {
        *mantissa &= ((1 << (whereIsOne + countOfLeadingNulls - 1)) - 1);
        (*exponent) -= countOfLeadingNulls;
    } else {
        *mantissa <<= (*exponent > 0 ? (*exponent - 1) : 0);
        (*exponent) = 0;
    }
}

void printNormalizeNumber(FloatingPoint number, char format) {
    int SHIFT = format == 'f' ? 127 : 15;
    if (format == 'f') {
        printf("%s0x1.%06lx", number.sign ? "-" : "", (number.mantissa << 1));
    } else {
        printf("%s0x1.%03lx", number.sign ? "-" : "", (number.mantissa << 2));
    }
    number.exponent >= SHIFT ? printf("p+") : putchar('p');
    printf("%i\n", number.exponent - SHIFT);
}

void printDenormalizeNumber(FloatingPoint number, char format) {
    int MANTISSA_SIZE = format == 'f' ? 23 : 10;
    int whereIsOne = binary_number_length(number.mantissa);
    int countOfLeadingNulls = (MANTISSA_SIZE - whereIsOne) > 0 ? (MANTISSA_SIZE - whereIsOne) : 0;
    number.mantissa <<= (1 + countOfLeadingNulls);
    number.mantissa &= (~(1ULL << (whereIsOne + countOfLeadingNulls)));
    number.exponent -= countOfLeadingNulls;

    printNormalizeNumber(number, format);
}

void printFloatingPoint(FloatingPoint *number, char format) {
    switch (typeOfNumber(*number, format)) {
        case 0:
            format == 'f' ? printf("0x0.000000p+0\n") : printf("0x0.000p+0\n");
            return;
        case 1:
            format == 'f' ? printf("-0x0.000000p+0\n") : printf("-0x0.000p+0\n");
            return;
        case 2:
            printDenormalizeNumber(*number, format);
            return;
        case 3:
            printf("nan\n");
            return;
        case 4:
            printf("-inf\n");
            return;
        case 5:
            printf("inf\n");
            return;
        case 6:
            printNormalizeNumber(*number, format);
            return;
    }
}

uint32_t roundMantissa(FloatingPoint number, uint32_t lostPart, int lostPartSize, char typeOfRound) {
    if (lostPart) {
        switch (typeOfRound) {
            case '0':
                return number.mantissa;
            case '1':
                if (lostPart > (1 << (lostPartSize - 1)) || (lostPart == 1 && lostPartSize == 1)) {
                    return number.mantissa + 1;
                } else if (lostPart == (1 << (lostPartSize - 1))) {
                    return (number.mantissa & 1) == 0 ? number.mantissa : number.mantissa + 1;
                }
                return number.mantissa;
            case '2':
                return number.sign ? number.mantissa : number.mantissa + 1;
            case '3':
                return number.sign ? number.mantissa + 1 : number.mantissa;
            default:
                return number.mantissa;
        }
    }

    return number.mantissa;
}

uint32_t takeLostPart(uint64_t mantissa, int lostPartSize) {
    return mantissa & ((1ULL << lostPartSize) - 1);
}

void applyMantissaRounding(FloatingPoint *number, int lostPartSize, int shift, char typeOfRound) {
    if (lostPartSize > 0) {
        uint32_t lostPart = takeLostPart(number->mantissa, lostPartSize);
        number->mantissa >>= lostPartSize;
        number->mantissa = roundMantissa(*number, lostPart, lostPartSize, typeOfRound);
        number->exponent += shift;
    }
}

bool addHiddenBit(FloatingPoint *number, char format) {
    int MANTISSA_SIZE = format == 'f' ? 23 : 10;
    if (typeOfNumber(*number, format) == 6) {
        number->mantissa += (1 << MANTISSA_SIZE);
        return true;
    }
    return false;
}

void alignExponents(FloatingPoint *x, FloatingPoint *y) {
    int diff = x->exponent - y->exponent;
    if (diff > 0) {
        applyMantissaRounding(y, diff, diff, '0');
    } else {
        applyMantissaRounding(x, -diff, -diff, '0');
    }
}

FloatingPoint handleExponentOverflow(FloatingPoint result, int SHIFT, int MANTISSA_SIZE,
                                     FloatingPoint NEGATIVE_INF, FloatingPoint POSITIVE_INF,
                                     FloatingPoint NEGATIVE_NULL, FloatingPoint POSITIVE_NULL) {
    if (result.exponent >= (SHIFT * 2 + 1)) {
        if (result.sign) {
            return NEGATIVE_INF;
        } else {
            return POSITIVE_INF;
        }
    } else if (result.exponent < 0) {
        if (result.exponent >= -(MANTISSA_SIZE - 1) && result.mantissa) {
            result.mantissa >>= (-result.exponent);
            result.exponent = 0;
        } else {
            if (result.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        }
    }
    return result;
}

FloatingPoint addition(FloatingPoint x, FloatingPoint y, char typeOfRound, char format) {
    int typeOfX = typeOfNumber(x, format);
    int typeOfY = typeOfNumber(y, format);

    int MANTISSA_SIZE, SHIFT;
    FloatingPoint NaN, POSITIVE_INF, NEGATIVE_INF, POSITIVE_NULL, NEGATIVE_NULL;
    setConstants(format, &MANTISSA_SIZE, &SHIFT, &NaN, &POSITIVE_INF, &NEGATIVE_INF, &POSITIVE_NULL, &NEGATIVE_NULL);

    if (typeOfX == 3 || typeOfY == 3 || typeOfX == 4 && typeOfY == 5 || typeOfX == 5 && typeOfY == 4) {
        return NaN;
    } else if (typeOfX == 1 || typeOfY == 1) {
        if (typeOfX == 1 && typeOfY == 1) {
            return NEGATIVE_NULL;
        } else if (typeOfX == 1) {
            return y;
        } else {
            return x;
        }
    } else if (typeOfX == 0 || typeOfY == 0) {
        if (typeOfX == 0) {
            return y;
        } else {
            return x;
        }
    } else if (typeOfX == 4 || typeOfY == 4) {
        return NEGATIVE_INF;
    } else if (typeOfX == 5 || typeOfY == 5) {
        return POSITIVE_INF;
    }

    bool isNeedNormalize = addHiddenBit(&x, format) | addHiddenBit(&y, format);

    alignExponents(&x, &y);

    FloatingPoint result;
    result.exponent = y.exponent;
    if (x.sign == y.sign) {
        result.sign = x.sign;
        result.mantissa = x.mantissa + y.mantissa;
    } else {
        result.sign = x.mantissa >= y.mantissa ? x.sign : y.sign;
        result.mantissa = x.mantissa >= y.mantissa ? x.mantissa - y.mantissa : y.mantissa - x.mantissa;
    }

    if (!result.mantissa) {
        return POSITIVE_NULL;
    }

    int length_mantissa = binary_number_length(result.mantissa);
    int countOfLeadingNulls = (MANTISSA_SIZE - length_mantissa) > 0 ? (MANTISSA_SIZE - length_mantissa) : 0;
    length_mantissa = binary_number_length(result.mantissa) + countOfLeadingNulls;

    applyMantissaRounding(&result, (length_mantissa - (MANTISSA_SIZE + 1)), isNeedNormalize, typeOfRound);

    if (isNeedNormalize) {
        normalize(&result.mantissa, &result.exponent, format);
    }

    result = handleExponentOverflow(result, SHIFT,
                                    MANTISSA_SIZE, NEGATIVE_INF, POSITIVE_INF, NEGATIVE_NULL, POSITIVE_NULL);

    return result;
}

FloatingPoint multiplication(FloatingPoint x, FloatingPoint y, char typeOfRound, char format) {
    int typeOfX = typeOfNumber(x, format);
    int typeOfY = typeOfNumber(y, format);

    int MANTISSA_SIZE, SHIFT;
    FloatingPoint NaN, POSITIVE_INF, NEGATIVE_INF, POSITIVE_NULL, NEGATIVE_NULL;
    setConstants(format, &MANTISSA_SIZE, &SHIFT, &NaN, &POSITIVE_INF, &NEGATIVE_INF, &POSITIVE_NULL, &NEGATIVE_NULL);

    if (typeOfX == 3 || typeOfY == 3) {
        return NaN;
    } else if (typeOfX == 0 || typeOfY == 0) {
        if (typeOfX == 4 || typeOfY == 4 || typeOfX == 5 || typeOfY == 5) {
            return NaN;
        } else if (typeOfX == 1 || typeOfY == 1) {
            return NEGATIVE_NULL;
        } else if (typeOfX == 0) {
            if (y.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        } else {
            if (x.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        }
    } else if (typeOfX == 1 || typeOfY == 1) {
        if (typeOfX == 4 || typeOfY == 4 || typeOfX == 5 || typeOfY == 5) {
            return NaN;
        } else if (typeOfX == 1) {
            if (!y.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        } else {
            if (!x.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        }
    } else if (typeOfX == 4 || typeOfY == 4) {
        if (typeOfX == 5 || typeOfY == 5) {
            return NaN;
        } else if (typeOfX == 4) {
            if (y.sign) {
                return POSITIVE_INF;
            } else {
                return NEGATIVE_INF;
            }
        } else {
            if (x.sign) {
                return POSITIVE_INF;
            } else {
                return NEGATIVE_INF;
            }
        }
    } else if (typeOfX == 5 || typeOfY == 5) {
        if (typeOfX == 5) {
            if (!y.sign) {
                return POSITIVE_INF;
            } else {
                return NEGATIVE_INF;
            }
        } else {
            if (!x.sign) {
                return POSITIVE_INF;
            } else {
                return NEGATIVE_INF;
            }
        }
    } else if (typeOfX == 2 && typeOfY == 2) {
        if (x.sign ^ y.sign) {
            return NEGATIVE_NULL;
        } else {
            return POSITIVE_NULL;
        }
    }

    bool isNeedNormalize = addHiddenBit(&x, format) | addHiddenBit(&y, format);

    FloatingPoint result;
    result.sign = x.sign ^ y.sign;
    result.exponent = x.exponent + y.exponent - SHIFT;
    result.mantissa = (uint64_t) x.mantissa * y.mantissa;

    int mantissa_length = binary_number_length(result.mantissa);
    applyMantissaRounding(&result, mantissa_length - MANTISSA_SIZE - 1, 0, typeOfRound);

    if (isNeedNormalize) {
        normalize(&result.mantissa, &result.exponent, format);
        result.exponent++;
    }

    result = handleExponentOverflow(result, SHIFT, MANTISSA_SIZE,
                                    NEGATIVE_INF, POSITIVE_INF, NEGATIVE_NULL, POSITIVE_NULL);

    return result;
}

FloatingPoint division(FloatingPoint x, FloatingPoint y, char typeOfRound, char format) {
    int typeOfX = typeOfNumber(x, format);
    int typeOfY = typeOfNumber(y, format);

    int MANTISSA_SIZE, SHIFT;
    FloatingPoint NaN, POSITIVE_INF, NEGATIVE_INF, POSITIVE_NULL, NEGATIVE_NULL;
    setConstants(format, &MANTISSA_SIZE, &SHIFT, &NaN, &POSITIVE_INF, &NEGATIVE_INF, &POSITIVE_NULL, &NEGATIVE_NULL);

    if (typeOfX == 3 || typeOfY == 3 || (typeOfX == 1 && typeOfY == 1) || (typeOfX == 5 && typeOfY == 4) ||
        (typeOfX == 4 && typeOfY == 4) || (typeOfX == 5 && typeOfY == 5) || (typeOfX == 4 && typeOfY == 5) ||
        (typeOfX == 0 && typeOfY == 0) || (typeOfX == 0 && typeOfY == 1) || (typeOfX == 1 && typeOfY == 0)) {
        return NaN;
    } else if (typeOfX == 0 || typeOfY == 0) {
        if (typeOfX != 0) {
            if (x.sign ^ y.sign) {
                return NEGATIVE_INF;
            } else {
                return POSITIVE_INF;
            }
        } else {
            if (x.sign ^ y.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        }
    } else if (typeOfX == 1 || typeOfY == 1) {
        if (typeOfY == 4 || typeOfY == 5 || typeOfX == 1) {
            if (x.sign ^ y.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        } else {
            if (x.sign ^ y.sign) {
                return NEGATIVE_INF;
            } else {
                return POSITIVE_INF;
            }
        }
    } else if (typeOfX == 4 || typeOfY == 4) {
        if (typeOfY == 4) {
            if (x.sign ^ y.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        } else {
            if (x.sign ^ y.sign) {
                return NEGATIVE_INF;
            } else {
                return POSITIVE_INF;
            }
        }
    } else if (typeOfX == 5 || typeOfY == 5) {
        if (typeOfY == 5) {
            if (x.sign ^ y.sign) {
                return NEGATIVE_NULL;
            } else {
                return POSITIVE_NULL;
            }
        } else {
            if (x.sign ^ y.sign) {
                return NEGATIVE_INF;
            } else {
                return POSITIVE_INF;
            }
        }
    }

    bool isDenormalizeProduct = addHiddenBit(&x, format) & addHiddenBit(&y, format);

    FloatingPoint result;
    result.sign = x.sign ^ y.sign;
    result.exponent = x.exponent - y.exponent + SHIFT;
    result.mantissa = (x.mantissa << MANTISSA_SIZE) / y.mantissa;

    uint32_t lostPart = (x.mantissa << MANTISSA_SIZE) % y.mantissa;
    int lostPartSize = binary_number_length(y.mantissa);
    result.mantissa = roundMantissa(result, lostPart, lostPartSize, typeOfRound);

    if (isDenormalizeProduct) {
        int mantissa_size = binary_number_length(result.mantissa);
        normalize(&result.mantissa, &result.exponent, format);
        result.exponent -= (mantissa_size < (MANTISSA_SIZE + 1)) ? MANTISSA_SIZE - mantissa_size + 1 : 0;
        result.mantissa <<= (mantissa_size < (MANTISSA_SIZE + 1)) ? MANTISSA_SIZE - mantissa_size + 1 : 0;
    } else {
        int whereIsOne = binary_number_length(result.mantissa);
        int countOfLeadingNulls = (lostPartSize - whereIsOne) > 0 ? (lostPartSize - whereIsOne) : 0;
        result.mantissa <<= (1 + countOfLeadingNulls);
        result.mantissa &= (~(1ULL << (whereIsOne + countOfLeadingNulls)));
        result.exponent -= (1 + countOfLeadingNulls);
        result.mantissa += lostPart;
    }

    result = handleExponentOverflow(result, SHIFT, MANTISSA_SIZE,
                                    NEGATIVE_INF, POSITIVE_INF, NEGATIVE_NULL, POSITIVE_NULL);

    return result;
}

typedef struct
{
    const char *format;
    const char *rounding;
    const char *number1;
    const char *operation;
    const char *number2;
} Arguments;

Arguments parseArguments(int argc, const char *argv[]) {
    Arguments args;
    args.format = argv[1];
    args.rounding = argv[2];
    args.number1 = argv[3];

    if (argc == 6) {
        args.operation = argv[4];
        args.number2 = argv[5];
    }

    return args;
}

int main(int argc, const char *argv[]) {
    if (argc != 4 && argc != 6) {
        fprintf(stderr, "Invalid arguments. Expected 3 (or 5) arguments.\n");
        return ERROR_ARGUMENTS_INVALID;
    }

    Arguments args = parseArguments(argc, argv);

    char format = args.format[0];
    if (strlen(args.format) != 1 || (format != 'h' && format != 'f')) {
        fprintf(stderr, "Unsupported floating-point number format.\n");
        return ERROR_ARGUMENTS_INVALID;
    }

    char rounding = args.rounding[0];
    if (strlen(args.rounding) != 1 || (rounding != '0' && rounding != '1' && rounding != '2' && rounding != '3')) {
        fprintf(stderr, "Unsupported type of rounding.\n");
        return ERROR_ARGUMENTS_INVALID;
    }

    int errorCode;
    FloatingPoint number1, number2, result;
    errorCode = parseNumber(&number1, args.number1, format);
    if (errorCode) {
        return errorCode;
    }

    if (argc == 4) {
        printFloatingPoint(&number1, format);
        return SUCCESS;
    }

    errorCode = parseNumber(&number2, args.number2, format);
    if (errorCode) {
        return errorCode;
    }

    char operation = args.operation[0];
    if (strlen(args.operation) != 1) {
        fprintf(stderr, "Unsupported operation.\n");
        return ERROR_ARGUMENTS_INVALID;
    }

    switch (operation) {
        case '+':
            result = addition(number1, number2, rounding, format);
            printFloatingPoint(&result, format);
            return SUCCESS;
        case '-':
            number2.sign = !number2.sign;
            result = addition(number1, number2, rounding, format);
            printFloatingPoint(&result, format);
            return SUCCESS;
        case '*':
            result = multiplication(number1, number2, rounding, format);
            printFloatingPoint(&result, format);
            return SUCCESS;
        case '/':
            result = division(number1, number2, rounding, format);
            printFloatingPoint(&result, format);
            return SUCCESS;
        default:
            fprintf(stderr, "Unsupported operation.\n");
            return ERROR_ARGUMENTS_INVALID;
    }
}