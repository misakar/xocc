#include "xo.h"

/*  ___________________________________
 *  |  WORD  TYPENUM |  WORD  TYPENUM  |
 *  |________________|_________________|
 *  |  begin   1     |  :       17     |
 *  |  if      2     |  :=      18     |
 *  |  then    3     |  <       19     |
 *  |  while   4     |  !=      20     |
 *  |  do      5     |  <=      21     |
 *  |  end     6     |  >       22     |
 *  |  else    7     |                 |
 *  |  l(l|d)* 10    |  >=      23     |
 *  |  +-dd*   11    |  =       24     |
 *  |  +       13    |  ;       25     |
 *  |  -       14    |  (       26     |
 *  |  *       15    |  )       27     |
 *  |  /       16    |  #       30     |
 *  ------------------------------------
 */

void xosyntax(WORD* srcwords)
{
  /* 语法分析程序 */
  int cursor = 0;  // 游标计数器
  if (srcwords[cursor].typenum == 1)  // begin
  {
    cursor++;
    if (srcwords[cursor].typenum == 17)  // :
    {
      cursor++;
      // 调用语句串分析程序
      Statements(srcwords, &cursor);
    }
    else
    { fprintf(stderr, "[xosyntax:':'] Error\n"); }
  }
  else
  { fprintf(stderr, "[xosyntax:'begin'] Error\n"); }


  if (srcwords[cursor].typenum == 6)  // end
  {
    cursor++;
    if (srcwords[cursor].typenum == 25)  // ;
    {
      // 程序语法正确
      printf("[xosyntax:25] Success\n");
    }
    else
    { fprintf(stderr, "[xosyntax:';'] Error\n"); }
  }
  else
  { fprintf(stderr, "[xosyntax:'end'] Error\n"); }
}

void Statements(WORD* srcwords, int* cursor)
{
  /* 语句串分析程序 */
  // 调用语句分析程序
  Statement(srcwords, cursor);
  while (srcwords[*cursor].typenum == 25)  // ;
  {
    (*cursor)++;
    // 调用语句分析程序
    Statement(srcwords, cursor);
  }
}

void Statement(WORD* srcwords, int* cursor)
{
  /* 语句分析程序 */
  // cursor++;
  if (srcwords[*cursor].typenum == 10)  // l(l|d)*
  {
    Assignment(srcwords, cursor);  // 赋值语句
  }
  else if (srcwords[*cursor].typenum == 2)  // if
  {
    Condition(srcwords, cursor);  // 条件语句
  }
  else if (srcwords[*cursor].typenum == 6)  // end
  {
    // end
  }
}

void Assignment(WORD* srcwords, int* cursor)
{
  /* 赋值语句分析程序 */
  if (srcwords[*cursor].typenum == 10)  // l(l|d)*
  {
    (*cursor)++;
    if (srcwords[*cursor].typenum == 18)  // :=
    {
      (*cursor)++;
      Expression(srcwords, cursor);
      if (srcwords[*cursor].typenum == 25)  // ;
      {
         // 语法程序正确
      }
      else
      { fprintf(stderr, "[Assignment:';'] Error\n"); }
    }
    else
    { fprintf(stderr, "[Assignment:':='] Error\n"); }
  }
  else
  { fprintf(stderr, "[Assignment:'l(l|d)*'] Error\n"); }
}

void Condition(WORD* srcwords, int* cursor)
{
  /* 条件语句分析程序 */
  if (srcwords[*cursor].typenum == 2)  // if
  {
    (*cursor)++;
    if (srcwords[*cursor].typenum == 26)  // (
    {
      (*cursor)++;
      Boolean(srcwords, cursor);
      if (srcwords[*cursor].typenum == 27)  // )
      {
        (*cursor)++;
        if (srcwords[*cursor].typenum == 3)  // then
        {
          (*cursor)++;
          if (srcwords[*cursor].typenum == 17)  // :
          {
            (*cursor)++;
            ConditionStatements(srcwords, cursor);
          }
          else
          { fprintf(stderr, "[Condition:':'] Error\n"); }
        }
        else
        { fprintf(stderr, "[Condition:'then'] Error\n"); }
       }
       else
       { fprintf(stderr, "[Condition:')'] Error\n"); }
    }
    else
    { fprintf(stderr, "[Condition:'('] Error\n"); }
  }
  else
  { fprintf(stderr, "[Condition:'if'] Error\n"); }
}

void ConditionStatements(WORD* srcwords, int* cursor)
{
  /* 条件语句分析程序 */
  Statements(srcwords, cursor);
  // else分句处理
  if (srcwords[*cursor].typenum == 7)  // else
  {
    (*cursor)++;
    if (srcwords[*cursor].typenum == 2)  // if
    {
      Condition(srcwords, cursor);
    }
    else if (srcwords[*cursor].typenum == 17)  // :
    {
      (*cursor)++;
      Statements(srcwords, cursor);
    }
    else
    { fprintf(stderr, "[ConditionStatement:'if, :'] Error\n"); }
  }
  else
  {
    // 无else分句, 继续分析
  }
}

void Boolean(WORD* srcwords, int* cursor)
{
  /* if语句判断式分析程序 */
  Expression(srcwords, cursor);
  if ((srcwords[*cursor].typenum == 22) ||  // >
      (srcwords[*cursor].typenum == 23) ||  // >=
      (srcwords[*cursor].typenum == 24) ||  // =
      (srcwords[*cursor].typenum == 19) ||  // <
      (srcwords[*cursor].typenum == 21) ||  // <=
      (srcwords[*cursor].typenum == 20))    // !=
  {
    (*cursor)++;
    Expression(srcwords, cursor);
  }
  else
  { fprintf(stderr, "[Boolean] Error\n"); }
}

void Expression(WORD* srcwords, int* cursor)
{
  /* 表达式分析程序 */
  Item(srcwords, cursor);
  while ((srcwords[*cursor].typenum == 13) ||  // +
         (srcwords[*cursor].typenum == 14))  // -
  {
    (*cursor)++;
    Item(srcwords, cursor);
  }
}

void Item(WORD* srcwords, int* cursor)
{
  /* 项分析程序 */
  Factor(srcwords, cursor);
  while ((srcwords[*cursor].typenum == 15) ||  // *
         (srcwords[*cursor].typenum == 16))  // /
  {
    (*cursor)++;
    Factor(srcwords, cursor);
  }
}

void Factor(WORD* srcwords, int* cursor)
{
  /* 因子分析程序 */
  if ((srcwords[*cursor].typenum == 10)  ||  // l(l|d)*
      (srcwords[*cursor].typenum == 11))  // d
  {
    (*cursor)++;
  }
  else if (srcwords[*cursor].typenum == 26)  // (
  {
    (*cursor)++;
    Expression(srcwords, cursor);
    if (srcwords[*cursor].typenum == 27)  // )
    {
      (*cursor)++;
    }
    else
    { fprintf(stderr, "[Factor:')'] Error\n"); }
  }
  else
  { fprintf(stderr, "[Factor:'('] Error\n"); }
}
