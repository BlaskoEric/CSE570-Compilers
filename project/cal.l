/**************************************************************
* Name          : Eric Blasko
* Date          : 3/1/2019
* Title         : Lex/Yacc Term Project
* Description   : This file is the lex portion of this project.
*                 It provides the rules for the given grammar
*                 which allows for single variables from a-z,
*                 integers, and floating point values. This
*                 file also list rules for token values that 
*                 utilize the math.h library.
***************************************************************/


%{
    #include<stdio.h>
    #include<stdlib.h>
    #include<math.h>
    #include"cal.tab.h" 
    #define BASE 10
    int c;
    
%}

%%

" ";

[a-z] {
    c = yytext[0];
    yylval.var = c - 'a';
    return (LETTER);
}

([1-9][0-9]+|[0-9])?\.[0-9]+ {
    yylval.num = atof(yytext);
    return (DIGIT);
}

([1-9][0-9]+|[0-9]) {
    yylval.num = atof(yytext);
    return (DIGIT);
}

[-+()=/*\n^]    {return *yytext;}

[ \t] ;
.   yyerror("invalid char");

sin {return SIN;}
asin {return ASIN;}
sinh {return SINH;}
cos {return COS;}
acos {return ASIN;}
cosh {return COSH;}
cube {return CUBE;}
squre {return SQURE;}
ln {return LN;}
tan {return TAN;}
atan {return ATAN;}
tanh {return TANH;}
log {return LOG;}
sqrt {return SQRT;}
exp {return EXP;}
ceil {return CEIL;}
floor {return FLOOR;}
round {return ROUND;}
exit {return EXIT;}

