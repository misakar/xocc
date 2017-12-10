/***
 *  xo语言的词法分析程序
 *  ````````````````````
 */
#include "xo.h"

char m_getch(char* input)
{
  /* 从input缓冲区中读取(下)一个字符 */
  ch = *(input + p_input);
  p_input++;
  return ch;
}

int letter(char ch)
{
  /* 判断ch是不是字母(不区分大小写)*/
  if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {  return 1; }
  else
    {  return 0; }
}

int digit(char ch)
{
  /* 判断ch是不是数字字符 */
  if (ch >= '0' && ch <= '9')
    { return 1; }
  else
    { return 0; }
}

void concat(char ch, char* token)
{
  /*
   * 在token缓冲区中将字符ch拼接成字符串,
   * (结尾自动添加'\0')
   */
  token[p_token] = ch;
  p_token++;
  token[p_token] = '\0';
}

void clear_token(char* token)
{
  /* 清空token缓冲区, 初始化p_token指针 */
  memset(token, 0, sizeof(token));
  p_token = 0;
}

int get_token_typenum(char* token)
{
  /*
   * 判断一个符合l(l|d)*的token是自定义标识符还是语言关键字
   * 并返回该token的typenum
   */
  int i = 0;
  while (strcmp(keywords[i], _KEY_WORD_END))
  {
    if (!strcmp(keywords[i], token))
    {
      return i+1;
    }
    i++;
  }
  return 10;
}

long str_to_int(char* str)
{
  long res = 0, pow = 0;
  for(int i = 0; i < strlen(str); i++)
  {
    pow = 1;
    for(int j = i; j < strlen(str)-1; j++)
      { pow *= 10; }
    res += (str[i]-'0')*pow;
  }
  return res;
}

WORD* scaner(WORD* srcword, char* input, char* token, WORD* srcwords, int scaner_cnt)
{
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
  char ch = m_getch(input);  // 从input缓冲区中读取一个字符
  while ((ch == ' ') || (ch == '\b') || (ch == '\r') || (ch == '\n'))
  {
    ch = m_getch(input);
  }
  if (letter(ch))  // 开头为字母
  {
    while (letter(ch) || digit(ch))
    {
      // 依据正规式l(l|d)*组合保留字或用户自定义标识符
      concat(ch, token);
      ch = m_getch(input);
    }
    p_input--;  // 跳出while循环的判断会使p_input指针+1(下同)
    srcword->word = token;
    srcword->typenum = get_token_typenum(token);
  }
  else if (digit(ch))  // 开头是数字
  {
    while (digit(ch))
    {
      // 依据正规式d*组合数字
      concat(ch, token);
      ch = m_getch(input);
    }
    p_input--;
    srcword->word = token;
    srcword->typenum = 11;
  }
  else switch(ch)  // 其他的switch...case查表
  {
    case '=':
        srcword->word = "=";
        srcword->typenum = 24;
        break;
    case '+':
        srcword->word = "+";
        srcword->typenum = 13;
        break;
    case '-':
        ch = m_getch(input);
        if (digit(ch)) // 负数
        {
          srcword->word = "-";
          strcat(srcword->word, "1111");
          srcword->typenum = 11;
        }
        else
        {
          srcword->word = "-";
          srcword->typenum = 14;
        }
        break;
    case '*':
        srcword->word = "*";
        srcword->typenum = 15;
        break;
    case '/':
        srcword->word = "/";
        srcword->typenum = 16;
        break;
    case ':': ch = m_getch(input);
              if (ch == '=')
              {
                srcword->word = ":=";
                srcword->typenum = 18;
              }
              else if (ch == 10 || ch == ' ')
              {
                srcword->word = ":";
                srcword->typenum = 17;
              }
              else
              {
                srcword->word = "error";
                srcword->typenum = -1;
              }
        break;
    case '<': ch = m_getch(input);
              if (ch == '=')
              {
                srcword->word = "<=";
                srcword->typenum = 21;
              }
              else if (letter(ch) || digit(ch) || (ch == ' '))
              {
                srcword->word = "<";
                srcword->typenum = 19;
              }
              else
              {
                srcword->word = "error";
                srcword->typenum = -1;
              }
        break;
    case '>': ch = m_getch(input);
              if (ch == '=')
              {
                srcword->word = ">=";
                srcword->typenum = 23;
                break;
              }
              else if (letter(ch) || digit(ch) || (ch == ' '))
              {
                srcword->word = ">";
                srcword->typenum = 22;
              }
              else
              {
                srcword->word = "error";
                srcword->typenum = -1;
              }
        break;
    case '!': ch = m_getch(input);
              if (ch == '=')
              {
                srcword->word = "!=";
                srcword->typenum = 20;
              }
              else
              {
                srcword->word = "error";
                srcword->typenum = -1;
              }
        break;
    case ';':
        srcword->word = ";";
        srcword->typenum = 25;
        break;
    case '(':
        srcword->word = "(";
        srcword->typenum = 26;
        break;
    case ')':
        srcword->word = ")";
        srcword->typenum = 27;
        break;
    case '#':
        srcword->word = "#";
        srcword->typenum = 30;
        break;
    default:
        srcword->word = "error";
        srcword->typenum = -1;
  }
  srcwords[scaner_cnt].typenum = srcword->typenum;
  srcwords[scaner_cnt].word = (char *)malloc(sizeof(*(srcword->word)));
  strcpy(srcwords[scaner_cnt].word, srcword->word);
  clear_token(token);  // 清除用于本次识别的token缓冲区
  return srcwords;
}
