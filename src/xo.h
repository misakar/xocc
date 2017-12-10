#ifndef XOCC_H_
#define XOCC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _KEY_WORD_END "_KEY_WORD_END"

typedef struct
{
  /* WORD类型 */
  int typenum;  // 种别码
  char* word;   // 单词
} WORD;

char* input;  /* 文件输入缓冲区 */
char* token;  /* 缓冲区用于将字母依据规则组合成句子 */
int p_input;  /* input缓冲区游标 */
int p_token;  /* token缓冲区游标 */
char ch;
/* 词法分析程序计数器 */
int scaner_cnt;
/* xo语言包含的关键字 */
extern char* keywords[];
/* 由源程序字母组成的句子, 默认typenum=-1, word="error" */
extern WORD srcword;
/* 函数原型声明 */
WORD* scaner(WORD*, char*, char*, WORD*, int);
char m_getch(char *);
int letter(char);
int digit(char);
void concat(char, char *);
void clear_token(char *);
int get_token_typenum(char *);
long str_to_int(char *);
void xosyntax(WORD*);
void Statements(WORD*, int*);
void Statement(WORD*, int*);
void Item(WORD*, int*);
void Factor(WORD*, int*);
void Boolean(WORD*, int*);
void Expression(WORD*, int*);
void Condition(WORD*, int*);
void ConditionStatements(WORD*, int*);
void Assignment(WORD*, int*);

#endif
