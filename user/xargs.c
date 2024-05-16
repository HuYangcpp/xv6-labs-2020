//每次读取一个字符，直到读完一行
//将标准输入0中的字符一个一个的读取，直到遇到换行符或者读取结束。
//将读取结果进行处理，加到argv数组里，fork一个子线程，用exec执行它
#include "../kernel/types.h"
#include "user.h"
#include "../kernel/stat.h"
#include "../kernel/param.h"

int main(int argc,char *argv[])
{
    int bufP=-1;
    int i,readLen;
    char buf[512];//缓冲区
    char* new_argv[MAXARG];
    for(i=1;i<argc;++i)
    {
        new_argv[i-1]=argv[i];//跳过程序名称
    }
    while(1)
    {
        bufP=-1;
        do{
            bufP++;//0为标准输入
            readLen=read(0,&buf[bufP],sizeof(char));//返回读取字节数
        }while(readLen>0&&buf[bufP]!='\n');//>0就没到末尾，不等于\n就没有换行
        if(readLen==0&&bufP==0)//说明没有读到任何标准输入
        {
            break;
        }

        buf[bufP]='\0';//切换到子进程去读
        //参数放入exe数组
        new_argv[argc-1]=buf;
        //0标志命令行参数的结束
        new_argv[argc]=0;

        if(fork()==0)
        {
            exec(new_argv[0],new_argv);
            exit(0);
        }
        else 
        {
            wait(0);
        }
    
    }
    exit(0);
}