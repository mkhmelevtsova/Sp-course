#include "pch.h"

int main()
{
	printf(" _______________________ _______________________ _______________________ _______________________\n");
	printf("| Data type \t\t| Size(byte) \t\t| Min value \t\t| Max value \t\t|\n");
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| char \t\t\t| %hhd \t\t\t| %hhd \t\t\t| %hhd \t\t\t|\n", sizeof(char), CHAR_MIN, CHAR_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| signed char \t\t| %hhd \t\t\t| %hhd \t\t\t| %hhd \t\t\t|\n", sizeof(signed char), SCHAR_MIN, SCHAR_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| unsigned char \t| %hhd \t\t\t| %hhd \t\t\t| %hhu \t\t\t|\n", sizeof(unsigned char), 0, UCHAR_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| short \t\t| %hhd \t\t\t| %hd \t\t| %hd \t\t|\n", sizeof(short), SHRT_MIN, SHRT_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| unsigned short \t| %hhd \t\t\t| %hhd \t\t\t| %hu \t\t|\n", sizeof(unsigned short), 0, USHRT_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| int \t\t\t| %hhd \t\t\t| %d \t\t| %d \t\t|\n", sizeof(int), INT_MIN, INT_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| unsigned int \t\t| %hhd \t\t\t| %hhd \t\t\t| %u \t\t|\n", sizeof(unsigned int), 0, UINT_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| long int \t\t| %hhd \t\t\t| %ld \t\t| %ld \t\t|\n", sizeof(long int), LONG_MIN, LONG_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| unsigned long \t| %hhd \t\t\t| %hhd \t\t\t| %lu \t\t|\n", sizeof(unsigned long), 0, ULONG_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| long long int \t| %hhd \t\t\t| %lld \t| %lld \t|\n", sizeof(long long int), LLONG_MIN, LLONG_MAX);
	printf(" ----------------------- ----------------------- ----------------------- -----------------------\n");
	printf("| float \t\t| %hhd \t\t\t| %e \t\t| %e \t\t|\n", sizeof(float), FLT_MIN, FLT_MAX);
	printf(" _______________________ _______________________ _______________________ _______________________\n");
	printf("| double \t\t| %hhd \t\t\t| %e \t| %e \t|\n", sizeof(double), DBL_MIN, DBL_MAX);
	printf(" _______________________ _______________________ _______________________ _______________________\n");
	printf("| long double \t\t| %hhd \t\t\t| %e \t| %e \t|\n", sizeof(unsigned long long int), LDBL_MIN, LDBL_MAX);
	printf(" _______________________ _______________________ _______________________ _______________________\n");
}