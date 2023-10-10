#include <stdio.h>

int i = 0x12345678;

int main(void)
{
    printf("The logical address of i is 0x%08x",(unsigned int)(long)&i);
    fflush(stdout);
    while(i);
    return 0;
}
