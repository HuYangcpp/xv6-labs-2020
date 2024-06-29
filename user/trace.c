#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);//命令行参数数量是否少于 3 或者第二个参数（掩码）是否不在数字范围内就打印使用参数
  }

  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);//如果 trace 调用失败，打印错误信息并退出。
  }
  
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }//从第三个参数开始，将所有参数复制到新的参数列表 nargv 中。
  exec(nargv[0], nargv);
  exit(0);
}
