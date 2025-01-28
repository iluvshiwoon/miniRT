#include "../../../include/parsing.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// Attention a gerer les caracteres qu'il ne faut pas
double absolut_value(const char *s) 
{
    double result;
    int decimal_found;
    double decimal_divisor;

    result = 0.0;
    decimal_found = 0;
    decimal_divisor = 10.0;
    while (*s) 
    {
        if (isdigit((unsigned char)*s)) 
        {
            if (!decimal_found) {
                result = result * 10.0 + (*s - '0');
            } else {
                result += (*s - '0') / decimal_divisor;
                decimal_divisor *= 10.0;
            }
        } 
        else if (*s == '.' && !decimal_found)
            decimal_found = 1;
        else
            return (result);
        s++;
    }
    return (result);
}

double ft_atoi_double(char *s) 
{
    if (s == NULL) {
        fprintf(stderr, "La chaîne est NULL\n");
        exit(EXIT_FAILURE);
    }
    double result;
    double sign = 1.0;
    while (isspace((unsigned char)*s)) {
        s++;
    }
    if (*s == '-') {
        sign = -1.0;
        s++;
    } else if (*s == '+') {
        s++;
    }
    result = absolut_value(s);
    return (sign * result);
}


// int main() {
//     const char *test1 = "-123.456";
//     const char *test2 = "42";
//     const char *test3 = "0.001";

//     printf("%f\n", atoi_double(test1));  
//     printf("%f\n", atoi_double(test2));  
//     printf("%f\n", atoi_double(test3));  

//     return 0;
// }
