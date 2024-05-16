#include "../kernel/types.h"
#include "user.h"

#define RD 0
#define WR 1

const int INT_LEN = sizeof(int);

//读第一个数用来取余，同时打印，因为是素数
int lpipe_first_data(int lpipe[2], int *dst)
{
  if (read(lpipe[RD], dst, sizeof(int)) == sizeof(int)) {
    printf("prime %d\n", *dst);
    return 0;
  }
  return -1;
}

//取余不为0的，往右边走
void transmit_data(int lpipe[2], int rpipe[2], int first)
{
  int data;
  // 从左管道读取数据
  while (read(lpipe[RD], &data, sizeof(int)) == sizeof(int)) {
    // 将无法整除的数据传递入右管道
    if (data % first)
      write(rpipe[WR], &data, sizeof(int));
  }
  close(lpipe[RD]);
  close(rpipe[WR]);
}

//子进程递归
void primes(int lpipe[2])
{
  close(lpipe[WR]);//关闭写通道
  int first;
  if (lpipe_first_data(lpipe, &first) == 0) {//正常读取一个元素
    int p[2];
    pipe(p); // 当前的管道
    transmit_data(lpipe, p, first);

    if (fork() == 0) {//孙子进程
      primes(p);    // 递归的思想，但这将在一个新的进程中调用
    } else {//子进程
      close(p[RD]);
      wait(0);
    }
  }
  exit(0);
}

int main(int argc, char const *argv[])
{
  int p[2];
  pipe(p);//管道

  for (int i = 2; i <= 35; ++i) //写入初始数据
    write(p[WR], &i, INT_LEN);//写入管道，先写入的小，那么先读到的就小

  if (fork() == 0) {
    primes(p);//子进程直接递归
  } else {
    close(p[WR]);
    close(p[RD]);//父进程
    wait(0);
  }

  exit(0);
}
