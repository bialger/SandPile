#ifndef BASIC_FUNCTIONS_HPP
#define BASIC_FUNCTIONS_HPP

#include <cstdint>


#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include <Windows.h>
#else
#define STD_OUTPUT_HANDLE 0
#define HANDLE int
int GetStdHandle(int a);
int SetConsoleTextAttribute(int a, int b);
#endif


/** \n The IntFromString function takes a string representing an integer literal
 * and converts it to an int64_t value. It supports different number bases,
 * including binary (0b prefix), octal (0o prefix), and hexadecimal (x prefix).
 * The function handles potential conversion errors by checking the validity of
 * the converted result and the presence of any overflow or underflow. If any
 * error is detected, the function returns -1. Additionally, the function checks
 * if the converted value exceeds a specified limit and returns -1 if the limit
 * is surpassed. Otherwise, it returns the successfully converted int64_t value.
 */

int64_t IntFromString(char* int_literal, int64_t limit, int8_t base);

/** \n The i64toa function converts an integer to a string in a specified base.
 * Full analogy to the C function _i64toa in Windows. */

char* i64toa(int64_t value, char* buffer, int32_t base);

bool IsWindows();

/** \n The code validates the validity of a file_ based on rules specific to
 * Windows. The file_ should consist of alphanumeric characters, backslashes,
 * periods, hyphens, and spaces. Additionally, the code checks for consecutive
 * slashes in the file_, which is also considered invalid. */

bool IsValidFilename(char* pre_filename);

/** \n The code implementation of the PrintHelp function displays a detailed help
 * page for the SandPile, explaining its usage and the various
 * command-line options available. The information is formatted in a structured
 * manner, making it easy to read and understand. */

void PrintHelp();

#endif // BASIC_FUNCTIONS_HPP