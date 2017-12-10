#include "xo.h"

/*                    1        2     3        4      5      6      7                 */
char* keywords[] = {"begin", "if", "then", "while", "do", "end", "else", _KEY_WORD_END};
WORD srcword = {.typenum=-1, .word="error"};

int main(int argc, char** argv)
{
  /* xo语言包含的关键字 */
  FILE* fp;  /* 源代码文件指针 */
  if ((fp = fopen(argv[1], "rt")) == NULL)
  {
    /* 可读模式打开源文件 */
    fprintf(stderr, "<Error> cannot open file %s\n", argv[1]);
    exit(1);
  }
  /* 根据文件指针偏移获取文件大小(以确定文件输入缓冲区大小) */
  fseek(fp, 0, SEEK_END);
  long lsize = ftell(fp);
  /* 将文件指针重新指向文件头 */
  rewind(fp);
  /* malloc内存给缓冲区 */
  input = (char*)malloc(sizeof(char)*lsize);
  token = (char*)malloc(sizeof(char)*255);
  /* 存储词法分析得到的单词符号 */
  WORD* srcwords = (WORD*)malloc(sizeof(WORD)*lsize);
  for (int i = 0; i < sizeof(char)*lsize; i++)
  {
    srcwords[i].typenum = -1;
    srcwords[i].word= "error";
  }

  if (input == NULL || token == NULL)
  {
    fprintf(stderr, "<Error> memory malloc error\n");
    exit(1);
  }
  if (fread(input, 1, lsize, fp) != lsize)
  {
    fprintf(stderr, "<Error> file reading error\n");
    exit(1);
  }
  /* 关闭文件 */
  fclose(fp);
  /* 初始化缓冲区游标 */
  p_input = 0;
  p_token = 0;
  scaner_cnt = 0;
  printf("--------------- lexical analysis start ----------------\n");
  while (p_input < strlen(input)-1)
  {
    /* 遍历input缓冲区, 调用scaner扫描并依据规则识别token对(word:typenum) */
    scaner(&srcword, input, token, srcwords, scaner_cnt);
    scaner_cnt++;
  }
  /* 词法分析结束, 释放缓冲区空间 */
  free(input);
  free(token);

  /*************************************************************/
  for (int i = 0; srcwords[i].typenum != -1; i++)
  {
    printf("[%d: %s] ", srcwords[i].typenum, srcwords[i].word);
  }
  printf("--------------- lexical analysis end ----------------\n");
  /*************************************************************/

  /* 调用语法分析程序 */
  printf("--------------- syntax analysis start ----------------\n");
  xosyntax(srcwords);
  printf("--------------- syntax analysis end ----------------\n");

  /* 语法分析结束, 释放堆内存 */
  for (int i = 0; srcwords[i].typenum != -1; i++)
  {
    // printf("[%d: %s]\n", srcwords[i].typenum, srcwords[i].word);
    free(srcwords[i].word);
  }
  free(srcwords);

  return 0;
}
