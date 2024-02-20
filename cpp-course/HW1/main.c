#include <stdint.h>
#include <stdio.h>

#define MERSENNE_PRIME ((uint32_t)0x7FFFFFFF)

uint32_t factorial(uint16_t n)
{
	uint32_t result = 1;

	for (uint32_t i = 2; i <= n; ++i)
	{
		result = ((uint64_t)result * i) % MERSENNE_PRIME;
	}

	return result;
}

uint8_t number_length(uint32_t n)
{
	if (n == 0)
	{
		return 1;
	}

	uint8_t length = 0;
	while (n != 0)
	{
		length++;
		n /= 10;
	}

	return length;
}

uint8_t max_length(uint16_t n_start, uint16_t n_end)
{
	uint32_t temp_factorial = factorial(n_start);
	uint8_t maximum = number_length(temp_factorial);

	for (uint16_t i = n_start + 1; i <= n_end; ++i)
	{
		temp_factorial = ((uint64_t)temp_factorial * i) % MERSENNE_PRIME;
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
	printf("+");
	for (uint8_t i = 0; i < first_column_length + 2; ++i)
	{
		printf("-");
	}
	printf("+");
	for (uint8_t i = 0; i < second_column_length + 2; ++i)
	{
		printf("-");
	}
	printf("+\n");
}

void row_with_numbers(uint8_t first_column_len, uint8_t second_column_len, uint16_t n, uint32_t n_fact, int8_t align)
{
	uint8_t n_length = number_length(n);
	uint8_t n_factorial_length = number_length(n_fact);

	printf("|");
	for (uint8_t i = 0; i < first_column_len + 2; ++i)
	{
		if (align == -1)
		{
			if (i == 1)
			{
				printf("%i", n);
				i += (n_length - 1);
				continue;
			}
		}
		else if (align == 0)
		{
			if (i == (((first_column_len + 2) / 2) - ((n_length - first_column_len % 2) / 2)))
			{
				printf("%i", n);
				i += (n_length - 1);
				continue;
			}
		}
		else if (align == 1)
		{
			if (i == (first_column_len - n_length + 1))
			{
				printf("%i", n);
				i += (n_length - 1);
				continue;
			}
		}
		printf(" ");
	}
	printf("|");
	for (uint8_t i = 0; i < second_column_len + 2; ++i)
	{
		if (align == -1)
		{
			if (i == 1)
			{
				printf("%i", n_fact);
				i += (n_factorial_length - 1);
				continue;
			}
		}
		else if (align == 0)
		{
			if (i == (((second_column_len + 2) / 2) - (n_factorial_length - second_column_len % 2) / 2))
			{
				printf("%i", n_fact);
				i += (n_factorial_length - 1);
				continue;
			}
		}
		else if (align == 1)
		{
			if (i == (second_column_len - n_factorial_length + 1))
			{
				printf("%i", n_fact);
				i += (n_factorial_length - 1);
				continue;
			}
		}
		printf(" ");
	}
	printf("|\n");
}

void first_part_of_table(uint8_t first_column_length, uint8_t second_column_length, int8_t align)
{
	// first row
	row_without_numbers(first_column_length, second_column_length);

	// second row
	printf("|");
	for (uint8_t i = 0; i < first_column_length + 2; ++i)
	{
		if (align == -1)
		{
			if (i == 1)
			{
				printf("n");
				continue;
			}
		}
		else if (align == 0)
		{
			if (i == ((first_column_length + 2) / 2))
			{
				printf("n");
				continue;
			}
		}
		else if (align == 1)
		{
			if (i == first_column_length)
			{
				printf("n");
				continue;
			}
		}
		printf(" ");
	}
	printf("|");
	for (uint8_t i = 0; i < second_column_length + 2; ++i)
	{
		if (align == -1)
		{
			if (i == 1)
			{
				printf("n!");
				i++;
				continue;
			}
		}
		else if (align == 0)
		{
			if (i == ((second_column_length + 2 + 1) / 2 - 1))
			{
				printf("n!");
				i++;
				continue;
			}
		}
		else if (align == 1)
		{
			if (i == second_column_length - 1)
			{
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

void table_of_factorials_first_case(uint16_t n_start, uint16_t n_end, int8_t align)
{
	uint8_t first_column_length = number_length(n_end);
	uint8_t second_column_length = max_length(n_start, n_end);

	if (second_column_length == 1)
	{
		second_column_length++;
	}

	first_part_of_table(first_column_length, second_column_length, align);

	uint32_t n_fact = factorial(n_start);
	for (uint32_t i = n_start; i <= n_end; ++i)
	{
		row_with_numbers(first_column_length, second_column_length, i, n_fact, align);
		n_fact = ((uint64_t)n_fact * (i + 1)) % MERSENNE_PRIME;
	}

	row_without_numbers(first_column_length, second_column_length);
}

void table_of_factorials_second_case(uint16_t n_start, uint16_t n_end, int8_t align)
{
	uint8_t first_column_length = 5;	  // because length(65535) = 5
	uint8_t second_column_length = 10;	  // because length(65535! % MERSENNE_PRIME) = 10

	first_part_of_table(first_column_length, second_column_length, align);

	//printf("1");
	uint32_t n_fact = factorial(n_start);
	//printf("2");
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

	row_without_numbers(first_column_length, second_column_length);
}

int main()
{
	int32_t n_start, n_end;
	int8_t align;

	scanf("%i %i %hhd", &n_start, &n_end, &align);
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

	if (n_start <= n_end)
	{
		table_of_factorials_first_case(n_start, n_end, align);
	}
	else
	{
		table_of_factorials_second_case(n_start, n_end, align);
	}

	return 0;
}