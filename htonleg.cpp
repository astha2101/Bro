#include<stdio.h>
#include<arpa/inet.h>

int main()
{
    int x,y;
    char *p;
    x=2147483647;
    p=(char *)&x;
    printf("Address %u\n",&x);
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;
    y=htonl(x);
    p=(char *)&y;
    printf("Address %u\n",&x);
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;
    printf("Address %u\n",p);
    printf("%d\n",*p);
    p++;


    return 0;
}
