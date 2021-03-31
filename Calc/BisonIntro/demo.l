%option noyywrap yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include "demo.tab.h"
#include "Tree.h"
%}

%%
"function" { return FUNCTION; }
"return" { return RETURN; }
"+" { return PLUS; } 
"-" { return MINUS; } 
"/" { return DIV; } 
"*" { return MULT; } 
"||" { return OR; }
"&&" { return AND; }
"!" { return NOT; }
"==" { return EQUAL; }
"!=" { return NEQUAL; }
">" { return GT; }
"<" { return LT; }
">=" { return GTE; }
"<=" { return LTE; }

";" { return ';';}
"(" { return '('; }
")" { return ')'; }
"{" { return '{'; }
"}" { return '}'; }
"," { return ','; }

"=" { return '='; }

"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }


0|[1-9][0-9]* {	yylval.node = NewNode(NT_NUMBER,NULL,NULL,NULL,yytext);
				return NUMBER; }

[a-zA-Z][0-9a-zA-Z_]*	{	yylval.node = getSymbol(yytext);
							return IDENTIFIER; }

[ \t\n\r]+ ;
 

%%
