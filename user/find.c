//查找目录树下具有特定名称的所有文件
#include "../kernel/types.h"
#include "user.h"
#include "../kernel/stat.h"
#include "../kernel/fs.h"

void find(char *path,const char* filename)
{
   char buf[512],*p; //用于存储文件路径的缓冲区和指针
   int fd;           // 文件描述符
   struct dirent de; //目录结构体
   struct stat st;   //文件状态结构体
   //打开指定路径文件
   if((fd=open(path,0))<0)//fd存储了当前path下所有文件项
   {
     fprintf(2,"find: cannot open %s\n",path);
     return;
   }
   //获取文件状态
    if(fstat(fd,&st)<0)
    {
        fprintf(2,"find:cannot fstat %s\n",path);
        close(fd);//关闭文件描述符
        return;
    }
   //如果参数错误
    if(st.type!=T_DIR)
    {
        fprintf(2,"usage: find <DIRECTORY> <filename>\n");
        //不是目录就报错
        return;
    }
   //如果路径过长
if(strlen(path)+1+DIRSIZ+1>sizeof buf)//路径+/+文件名+\0
{
    fprintf(2,"find: path too long\n");
    return;
}
   strcpy(buf,path);//将路径复制到缓冲区
   p =buf+strlen(buf);//将p指向路径字符串末尾
   *p++='/';//路径末尾加上斜杠用于连接文件名
   while(read(fd,&de,sizeof de)==sizeof de)//循环分别读取path下的目录项
   {
    if(de.inum==0)
    continue;//空目录跳过
    memmove(p,de.name,DIRSIZ);//文件名链接到路径末尾
    p[DIRSIZ]=0;//添加字符串结束符
    if(stat(buf,&st)<0)
    {
        fprintf(2,"find:cannot stat %s\n",buf);
        //获取文件状态失败报错
        continue;
    }
    //不要在“.”和“..”中递归
    if(st.type==T_DIR&&strcmp(p,".")!=0&&strcmp(p,"..")!=0)
    {
        find(buf,filename);//递归搜索子目录
    }//buf复制了路径
    else if(strcmp(filename,p)==0)//strcmp是判断是否相等
    printf("%s\n",buf);
    //找到了输出路径
   }
   close(fd);//关闭文件描述符
}

int main(int argc,char *argv[])
{//argc为c语言中参数数量即argv[]数组大小，*argv[]为传进来的参数组成的数组
if(argc!=3)
{
    fprintf(2,"usage: find <directory><filename>\n");
    exit(1);
}
find(argv[1],argv[2]);//argv[0]为程序名称，后为1路径，2文件名
exit(0);
}
