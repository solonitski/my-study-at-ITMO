#include <stdio.h>
#include <stdint.h>

uint32_t factorial(uint16_t n) {
    uint32_t mersenne_prime = 0x7FFFFFFF;
    uint32_t result = 1;

    for (uint16_t i = 2; i <= n; ++i) {
        result = ((uint64_t) result * i) % mersenne_prime;
    }

    return result;
}

uint8_t number_length(uint32_t n) {
    if (n == 0) {
        return 1;
    }

    uint8_t length = 0;
    while (n != 0) {
        length++;
        n /= 10;
    }

    return length;
}

uint8_t max_length(uint16_t n_start, uint16_t n_end) {
    uint8_t maximum = 0;

    for (uint16_t i = n_start; i <= n_end; ++i) {
        uint8_t temp_length = number_length(factorial(i));
        if (temp_length > maximum) {
            maximum = temp_length;
        }
        if (maximum == 10) {
            break;
        }
    }

    return maximum;
}

void row_without_numbers(uint8_t first_column_length, uint8_t second_column_length) {
    printf("+");
    for (uint8_t i = 0; i < first_column_length + 2; ++i) {
        printf("-");
    }
    printf("+");
    for (uint8_t i = 0; i < second_column_length + 2; ++i) {
        printf("-");
    }
    printf("+\n");
}

void row_with_numbers(uint8_t first_column_length, uint8_t second_column_length, uint16_t n, int8_t align) {
    uint32_t n_factorial = factorial(n);
    uint8_t n_length = number_length(n);
    uint8_t n_factorial_length = number_length(n_factorial);

    printf("|");
    for (uint8_t i = 0; i < first_column_length + 2; ++i) {
        if (align == -1) {
            if (i == 1) {
                printf("%i", n);
                i += (n_length - 1);
                continue;
            }
        } else if (align == 0) {
            if (i == (((first_column_length + 2) / 2) - ((n_length - first_column_length % 2) / 2))) {
                printf("%i", n);
                i += (n_length - 1);
                continue;
            }
        } else if (align == 1) {
            if (i == (first_column_length - n_length + 1)) {
                printf("%i", n);
                i += (n_length - 1);
                continue;
            }
        }
        printf(" ");
    }
    printf("|");
    for (uint8_t i = 0; i < second_column_length + 2; ++i) {
        if (align == -1) {
            if (i == 1) {
                printf("%i", n_factorial);
                i += (n_factorial_length - 1);
                continue;
            }
        } else if (align == 0) {
            if (i == (((second_column_length + 2) / 2) - (n_factorial_length - second_column_length % 2) / 2)) {
                printf("%i", n_factorial);
                i += (n_factorial_length - 1);
                continue;
            }
        } else if (align == 1) {
            if (i == (second_column_length - n_factorial_length + 1)) {
                printf("%i", n_factorial);
                i += (n_factorial_length - 1);
                continue;
            }
        }
        printf(" ");
    }
    printf("|\n");
}

void first_part_of_table(uint8_t first_column_length, uint8_t second_column_length, int8_t align) {
    // first row
    row_without_numbers(first_column_length, second_column_length);

    // second row
    printf("|");
    for (uint8_t i = 0; i < first_column_length + 2; ++i) {
        if (align == -1) {
            if (i == 1) {
                printf("n");
                continue;
            }
        } else if (align == 0) {
            if (i == ((first_column_length + 2) / 2)) {
                printf("n");
                continue;
            }
        } else if (align == 1) {
            if (i == first_column_length) {
                printf("n");
                continue;
            }
        }
        printf(" ");
    }
    printf("|");
    for (uint8_t i = 0; i < second_column_length + 2; ++i) {
        if (align == -1) {
            if (i == 1) {
                printf("n!");
                i++;
                continue;
            }
        } else if (align == 0) {
            if (i == ((second_column_length + 2 + 1) / 2 - 1)) {
                printf("n!");
                i++;
                continue;
            }
        } else if (align == 1) {
            if (i == second_column_length - 1) {
                printf("n!");
                i++;
                continue;
            }
        }
        printf(" ");
    }
    printf("|\n");

    // third row
    row_without_numbers(first_column_length, second_column_length);
}

void table_of_factorials(uint16_t n_start, uint16_t n_end, int8_t align) {
    uint8_t first_column_length = number_length(n_end);
    uint8_t second_column_length = max_length(n_start, n_end);

    if (second_column_length == 1) {
        second_column_length++;
    }

    first_part_of_table(first_column_length, second_column_length, align);
    for (uint16_t i = n_start; i <= n_end; ++i) {
        row_with_numbers(first_column_length, second_column_length, i, align);
    }
    row_without_numbers(first_column_length, second_column_length);
}

int main(void) {
    int32_t n_start, n_end;
    int8_t align;

    scanf("%i %i %hhd", &n_start, &n_end, &align);
    if (n_start < 0 || n_end < 0) {
        fprintf(stderr, "You have entered incorrect data. Factorial cannot be negative.");
        return 1;
    }
    if (-1 > align || align > 1) {
        fprintf(stderr, "This type of alignment is not supported.");
        return 1;
    }


    if (n_start > n_end) {
        table_of_factorials(n_end % 0x10000, n_start % 0x10000, align);
    } else {
        table_of_factorials(n_start % 0x10000, n_end % 0x10000, align);
    }

    return 0;
}
