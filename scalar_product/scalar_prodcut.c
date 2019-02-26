#include <stdio.h>

int arr1[] = { 1, 2, 3, 4, 5 };
int arr2[] = { 6, 7, 8, 9, 10 };

int scalar_product(const int arr1[], const int arr2[], size_t len)
{
    int sum = 0;
    size_t i = 0;
    for (i = 0; i < len; ++i)
    {
        sum += arr1[i] * arr2[i];
    }
    return sum;
}

int main(void)
{
    printf("scalar product of { 1, 2, 3, 4, 5 } and { 6, 7, 8, 9, 10 } is %d\n",
        scalar_product(arr1, arr2, sizeof(arr1) / sizeof(arr1[0])));
    
    return 0;
}
