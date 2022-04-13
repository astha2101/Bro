#include<stdio.h>
#include<sys/stat.h>

int main()
{
    int x;
    struct stat s;

    x=stat("/practice",&s);   // returns 0 if path is found
    printf("%d\n",x);
    printf("%d\n",s.st_mode & S_IFDIR); // returns some +ve number if the given path is a directory
    x=stat("/practice/project",&s);
    printf("%d\n",x);
    printf("%d\n",s.st_mode & S_IFDIR);
    x=stat("/practice/project/Bro/bro.cpp",&s);
    printf("%d\n",x);
    printf("%d\n",s.st_mode & S_IFDIR); // returns 0 if given path is not a directory
    x=stat("/home",&s);  // returns -1 if path not found
    printf("%d\n",x);
    return 0;
}
