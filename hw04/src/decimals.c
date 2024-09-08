#include <stdio.h>
#include <string.h>

static int normalize(int number, int upper)
{
    int lower = upper / 10;
    while (number < lower)
        number *= 10;
    while (number > upper)
        number /= 10;
    return number;
}

int decimals_to_base(int decimals)
{
    int base = 1;
    while (decimals --> 0)
        base *= 10;
    return base;
}

int load_decimal(const char *string, int decimals)
{
    int base = decimals_to_base(decimals);
    if (!strchr(string, '.')) {
        int result;
        sscanf(string, "%i", &result);
        return result * base;
    }

    int large;
    int small;
    sscanf(string, "%i.%i", &large, &small);

    return large * base + normalize(small, base);
}
