#include<stdio.h>
int main()
{
//char a[21]={"%CE%9B"};
//char a[21]={"%E2%82%AC"};
//char a[21]={"%E1%8F%A7"};
char a[21]={"%E2%8E%8A"};
char *ptr;
int numbers[4]={0,0,0,0};
char bytes[4]={0,0,0,0};
int i,j,k;
int index;
unsigned int unicode;
unsigned char m;
int numberOfBytes;
ptr=a;
ptr++;
sscanf(ptr,"%2x",&j);
if(j>=128)
{
numberOfBytes=0;
k=128;
while(1)
{
if((j&k)!=k) break;
numberOfBytes++;
k=k/2;
}
printf("Number of bytes are : %d",numberOfBytes);
m=(unsigned char)j;
m=m<<(numberOfBytes+1);
m=m>>(numberOfBytes+1);
index=4-numberOfBytes;
bytes[index]=m;
numbers[index]=j;
index++;
i=2;
while(i<=numberOfBytes)
{
ptr+=3;
sscanf(ptr,"%2x",&j);
m=(unsigned char)j;
m=m<<2;
m=m>>2;
bytes[index]=m;
numbers[index]=j;
index++;
i++;
}
printf("Numbers : ");
for(int i=0;i<=3;i++) printf("%d ",numbers[i]);
printf("\n");
printf("Bytes : ");
for(int i=0;i<=3;i++) printf("%d ",bytes[i]);
printf("\n");
printf("Preparing the unicode part\n");
index=4-numberOfBytes;
unicode=bytes[index];
index++;
while(index<=3)
{
m=bytes[index];
m=m<<2;
unicode=unicode<<8;
unicode=unicode | m;
unicode=unicode>>2;
index++;
}
}
else
{
unicode=j;
}
printf("Unicode string : %d\n",unicode);
return 0;
}
