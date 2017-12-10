# XOCC

xo语言编译器

## Hello World

```
begin:
    a := 10;
    if (a >= 0) then:
        a := 0;
    else:
        a := -1;
end;
```
<hr/>

```
$ make clean && make xocc && ./xocc test.xo
```

## xo语言EBNF表示

```
<Program> ::= "begin", ":", <Statements>, "end", ";";
<Statements> ::= <Statement>{;<Statement>};
<Statement> ::= <Assignment> | <Condition>;
<Assignment> ::= <Identifier>, ":=", <Expression>, ";";
<Condition> ::= "if", "(", <Boolean>, ")", "then", ":"
                    <Statements>, ";"
                ["else", ":", <Statements>, ";"];
<Boolean> ::= <Expression>, {">" | ">=" | "=" | "<=" | "<" | "!="}, <Expression>
<Expression> ::= <Item>{("+", <Item>) | ("-", <Item>)};
<Item> ::= <Factor>{("*", <Factor>) | ("/", <Factor>)};
<Factor> ::= <Identifier> | <Digit> | ("(", <Expression>, ")");
<Digit> ::= 0 | <Unsigned> | <Signed>;
<Unsigned> ::= "1" | "2" | "3" |
               "4" | "5" | "6" |
               "7" | "8" | "9" ;
<Signed> ::= "-", <Unsigned>;
<Identifier> = <Letter> | {<Letter> | <Unsigned>};
<Letter> :: = "a" | "b" | "c" | "d" | "e" | "f" | "g" |
              "h" | "i" | "j" | "k" | "l" | "m" | "n" |
              "o" | "p" | "q" | "r" | "s" | "t" | "u" |
              "v" | "w" | "x" | "y" | "z"             ;
```

## ToDo

+ [ ] 语义分析
