#include <stdio.h>

int is_prime(unsigned long n)
{
    unsigned long limit = n / 2;
    unsigned long i;
    int res = 1;
    for (i = 2; i < limit; ++i)
    {
        if (n % i == 0)
        {
            res = 0;
            break;
        }
    }
    
    return res;
}

int main(void)
{
    unsigned long n;
    scanf("%lu", &n);
    is_prime(n) ? puts("yes") : puts("no");
    
    return 0;
}
