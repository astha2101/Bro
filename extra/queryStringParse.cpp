#include<stdio.h>
bool isHexChar(int w)
{
    if(w>=48 && w<=57) return true;
    if(w>='A' && w<='F') return true;
    if(w>='a' && w<='f') return true;
    return false;
}
int main()
{
    char e[101] = {"ABCD%41%44CD+%2bpqr%29"};
    char d[101];
    char *ptr;
    int i,m;
    ptr=e;
    i=0;
    while(*ptr!='\0')
    {
        if(*ptr=='+')
        {
            d[i]=' ';
            i++;
            ptr++;
            continue;
        }
        if(*ptr!='%')
        {
            d[i]=*ptr;
            i++;
            ptr++;
            continue;
        }
        ptr++;
        if(isHexChar(*ptr) && isHexChar(*(ptr+1)))
        {
            sscanf(ptr,"%2x",&m);
            d[i]=m;
            i++;
            ptr+=2;
        }
        else{
            i=0;
            break;
        }

    }
    d[i]=='\0';
    printf("Decoded string : %s",d);
    return 0;
}
