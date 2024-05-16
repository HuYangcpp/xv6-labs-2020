#include "../kernel/types.h"

#include "user.h"

int main(int argc,char *argv[])
{
    if(argc!=2)//参数错误
    {
        fprintf(2,"usage: sleep <time>\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}