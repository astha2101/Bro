#include<stdio.h>
int main()
{
    int i;
    char hh[11];
    sprintf(hh,"%x",65);
    printf("%s\n",hh);
    sscanf(hh,"%2x",&i);
    printf("%d\n",i);
    return 0;
}
