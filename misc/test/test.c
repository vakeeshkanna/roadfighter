

#include<stdio.h>
extern void testfunc();
extern int a;
int main()
{
    testfunc();
    printf("Global a = %d\n",a);
    return 0;
}
