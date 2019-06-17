/****************************************************************************
* Name              : Eric Blasko
* Date              : 3/1/2019
* Title             : Lex/Yacc Term Project
* Description       : This file is the Yacc portion of this project. The
*                     given grammar is for a simple scientific calculator
*                     which can perform calculations and save values to 
*                     variables. The first section of this file, gives 
*                     declarations such as functions, token declarations, 
*                     and type declarations. The sectond section is the 
*                     grammar. The starting point is the list which can
*                     start a grammar or present and error. Stat determines
*                     if the grammar is a expression or a variable being 
*                     saved. For each grammar production, the stack values
*                     are saved to a register so that the value can be used
*                     again for the next calculation. up to 26 variables may
*                     also be assigned values. If a variable has been decalared
*                     the program will print all variables and there values
*                     after each production. The final section of this file
*                     are some C functions which are used which the program
*                     runs
******************************************************************************/
%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<math.h>
    #define YYTYPE double
    void print();
    double regsSym[26];
    double regsVal[26];
    int varCount;
    double ans;
    int yylex();
    int yyerror(char *s);
    int yywrap();
%}

%start list
%union{
    double num; 
    char var;
}

%token DIGIT
%token LETTER
%left '|'
%left '&'
%left '+' '-'
%left '*' '/' '%'
%token SIN ASIN SINH COS ACOS COSH TAN ATAN TANH LOG LN SQRT SQURE CUBE EXIT
%token EXP CEIL FLOOR ROUND  
%type <num>expr number DIGIT
%type <var>LETTER

%%

list:   /* empty */
    |   list stat '\n'      {print();}
    |   list error '\n'     {yyerrok;};

stat:   expr                {printf("%f\n", $1);}
    |   LETTER '=' expr     {regsVal[$1] = $3;
                             regsSym[$1] = $1 + 97;
                             varCount = varCount + 1;};

expr:   expr '+' expr       {$$ = ans = $1 + $3;} 
    |   expr '-' expr       {$$ = ans = $1 - $3;}
    |   expr '*' expr       {$$ = ans = $1 * $3;}
    |   '(' expr ')'        {$$ = ans = $2;}
    |   '-' expr            {$$ = ans = ans - $2;}
    |   '+' expr            {$$ = ans = ans + $2;}
    |   '*' expr            {$$ = ans = ans * $2;}
    |   '/' expr            {if($2==0){
                                    printf("Divide By Zero");
                                }
                                else {$$ = ans = ans / $2;}}
    |   SIN '(' expr ')'    {$$ = ans = sin($3*3.141/180);}
    |   ASIN '(' expr ')'   {$$ = ans = asin($3*3.141/180);}
    |   SINH '(' expr ')'   {$$ = ans = sinh($3*3.141/180);}
    |   COS '(' expr ')'    {$$ = ans = cos($3*3.141/180);}
    |   ACOS '(' expr ')'   {$$ = ans = acos($3*3.141/180);}
    |   COSH '(' expr ')'   {$$ = ans = cosh($3*3.141/180);}
    |   TAN '(' expr ')'    {$$ = ans = tan($3*3.141/180);}
    |   ATAN '(' expr ')'   {$$ = ans = atan($3*3.141/180);}
    |   TANH '(' expr ')'   {$$ = ans = tanh($3*3.141/180);}
    |   EXP '(' expr ')'    {$$ = ans = exp($3);}
    |   CEIL '(' expr ')'   {$$ = ans = ceil($3);}
    |   FLOOR '(' expr ')'  {$$ = ans = floor($3);}
    |   ROUND '(' expr ')'  {$$ = ans = round($3);}
    |   LOG '(' expr ')'    {$$ = ans = log($3);}
    |   LN '(' expr ')'     {$$ = ans = log($3);}
    |   SQRT '(' expr ')'   {$$ = ans = sqrt($3);}
    |   SQURE '(' expr ')'  {$$ = ans = $3 * $3;}
    |   CUBE '(' expr ')'   {$$ = ans = $3 * $3 * $3;}
    |   expr '/' expr       {if($3==0){
                                    printf("Divide By Zero");
                                }
                                else {$$=$1/$3;}} 
    |   LETTER              {$$ = regsVal[$1];}
    |   number;
    |   EXIT                {exit(0);};

number: DIGIT               {$$ = $1;}
        

%%


//begins the parse
int main()
{
    return yyparse();
}

//needed for error handling
int yyerror(char *s)
{
    fprintf(stderr, "%s\n", s );
    return 1;
}

//prints all values of the variables
void print()
{
    if(varCount > 0)
    {
        int i;
        printf("=========Variables============\n");
        for(i=0;i<26;i++)
        {
            if(regsVal[i] != 0)
            { 
                printf("n[%c] = [%f] \n",(char)regsSym[i], regsVal[i]);
            }
        }  
        printf("==============================\n");
    }
}

//required function
int yywrap() {return 1;}

                
