#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define MERSENNE_PRIME ((uint32_t)0x7FFFFFFF)

uint32_t factorial(uint16_t n)
{
	uint32_t result = 1;

	for (uint16_t i = 1; i < n; ++i)
	{
		result = ((uint64_t)result * (i + 1)) % MERSENNE_PRIME;
	}

	return result;
}

uint8_t number_length(uint32_t n)
{
	return (uint8_t)log10(n + !n) + 1;
}

uint8_t max_length(uint16_t n_start, uint16_t n_end)
{
	uint32_t temp_factorial = factorial(n_start);
	uint8_t maximum = number_length(temp_factorial);

	for (uint16_t i = n_start; i < n_end; ++i)
	{
		temp_factorial = ((uint64_t)temp_factorial * (i + 1)) % MERSENNE_PRIME;
		uint8_t temp_length = number_length(temp_factorial);
		if (temp_length > maximum)
		{
			maximum = temp_length;
		}
		if (maximum == 10)
		{
			break;
		}
	}

	return maximum;
}

void row_without_numbers(uint8_t first_column_length, uint8_t second_column_length)
{
	putchar('+');
	for (uint8_t i = 0; i < first_column_length + 2; ++i)
	{
		putchar('-');
	}
	putchar('+');
	for (uint8_t i = 0; i < second_column_length + 2; ++i)
	{
		putchar('-');
	}
	printf("+\n");
}

void print_aligned_number(uint8_t column_len, uint32_t number, int8_t align)
{
	uint8_t spaces_before, spaces_after;
	uint8_t number_len = number_length(number);

	switch (align)
	{
	case -1:
		spaces_before = 1;
		spaces_after = column_len - number_len - 1;
		break;
	case 0:
		spaces_before = (column_len >> 1) - ((number_len - (column_len & 1)) >> 1);
		spaces_after = column_len - number_len - spaces_before;
		break;
	case 1:
		spaces_before = column_len - number_len - 1;
		spaces_after = 1;
		break;
	default:
		return;
	}

	printf("%*c%d%*c", spaces_before, ' ', number, spaces_after, ' ');
}

void row_with_numbers(uint8_t first_column_len, uint8_t second_column_len, uint16_t n, uint32_t n_fact, int8_t align)
{
	putchar('|');
	print_aligned_number(first_column_len + 2, n, align);
	putchar('|');
	print_aligned_number(second_column_len + 2, n_fact, align);
	printf("|\n");
}

void first_part_of_table(uint8_t first_column_length, uint8_t second_column_length, int8_t align)
{
	uint8_t spaces_before_first, spaces_after_first;
	uint8_t spaces_before_second, spaces_after_second;

	// first row
	row_without_numbers(first_column_length, second_column_length);

	// second row
	switch (align)
	{
	case -1:
		spaces_before_first = 1;
		spaces_after_first = first_column_length;

		spaces_before_second = 1;
		spaces_after_second = second_column_length - 1;
		break;
	case 0:
		spaces_before_first = (first_column_length >> 1) + 1;
		spaces_after_first = first_column_length - spaces_before_first + 1;

		spaces_before_second = (second_column_length + 1) >> 1;
		spaces_after_second = second_column_length - spaces_before_second;
		break;
	case 1:
		spaces_before_first = first_column_length;
		spaces_after_first = 1;

		spaces_before_second = second_column_length - 1;
		spaces_after_second = 1;
		break;
	default:
		return;
	}

	printf("%c%*c%c%*c%c", '|', spaces_before_first, ' ', 'n', spaces_after_first, ' ', '|');
	printf("%*c%c%c%*c%c%c", spaces_before_second, ' ', 'n', '!', spaces_after_second, ' ', '|', '\n');

	// third row
	row_without_numbers(first_column_length, second_column_length);
}

void table_of_factorials(uint16_t n_start, uint16_t n_end, int8_t align)
{
	uint8_t first_column_length, second_column_length;
	uint32_t n_fact;

	if (n_start <= n_end)
	{
		first_column_length = number_length(n_end);
		second_column_length = max_length(n_start, n_end);
		second_column_length += !(second_column_length - 1);

		first_part_of_table(first_column_length, second_column_length, align);

		n_fact = factorial(n_start);
		for (uint32_t i = n_start; i <= n_end; ++i)
		{
			row_with_numbers(first_column_length, second_column_length, i, n_fact, align);
			n_fact = ((uint64_t)n_fact * (i + 1)) % MERSENNE_PRIME;
		}
	}
	else
	{
		first_column_length = 5;	  // because length(65535) = 5
		second_column_length = 10;	  // because length(65535! % MERSENNE_PRIME) = 10

		first_part_of_table(first_column_length, second_column_length, align);

		n_fact = factorial(n_start);
		for (uint32_t i = n_start; i <= UINT16_MAX; ++i)
		{
			row_with_numbers(first_column_length, second_column_length, i, n_fact, align);
			n_fact = ((uint64_t)n_fact * (i + 1)) % MERSENNE_PRIME;
		}
		n_fact = factorial(0);
		for (uint32_t i = 0; i <= n_end; ++i)
		{
			row_with_numbers(first_column_length, second_column_length, i, n_fact, align);
			n_fact = ((uint64_t)n_fact * (i + 1)) % MERSENNE_PRIME;
		}
	}

	row_without_numbers(first_column_length, second_column_length);
}

int main()
{
	int32_t n_start, n_end;
	int8_t align;

	if (scanf("%i %i %hhd", &n_start, &n_end, &align) != 3)
	{
		fprintf(stderr, "Error occurred during data input");
		return 1;
	}

	if (n_start < 0 || n_end < 0)
	{
		fprintf(stderr, "You have entered incorrect data. Factorial cannot be negative.");
		return 1;
	}
	if (-1 > align || align > 1)
	{
		fprintf(stderr, "This type of alignment is not supported.");
		return 1;
	}

	table_of_factorials(n_start, n_end, align);

	return 0;
}
