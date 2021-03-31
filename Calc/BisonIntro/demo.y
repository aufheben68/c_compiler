
%{
#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"
int yyerror(const char* message);
int yylex();
extern int yylineno;
%}

%error-verbose
%verbose
%locations

%union {
	struct ptnode *node;
}

%start compileUnit
%right '='
%nonassoc GT GTE LT LTE EQUAL NEQUAL
%left OR
%left AND
%left PLUS MINUS 
%left DIV MULT
%nonassoc NOT
%token <node> NUMBER IDENTIFIER
%token IF WHILE FUNCTION RETURN
%nonassoc IFPREC
%nonassoc ELSE
%type <node> compileUnit functionDefinition expression args fargs statement ifstatement whilestatement compoundStatement statementList
%%

compileUnit : compileUnit statement { root = $$ = NewNode(NT_COMPILEUNIT_STATEMENT,$1,$2); }
			| statement { root = $$ = NewNode(NT_COMPILEUNIT_STATEMENT,$1,NULL); }
			| functionDefinition { root = $$ = NewNode(NT_COMPILEUNIT_FUNDEF,$1,NULL); }
			| compileUnit functionDefinition { root = $$ = NewNode(NT_COMPILEUNIT_FUNDEF,$1,$2); }
			;

functionDefinition : FUNCTION IDENTIFIER '(' fargs ')' compoundStatement { $$ = NewNode(NT_FUNDEF,$2,$4,$6);
																		   $2->funBody = $$; }
				   ;

statement : expression ';' { $$ = NewNode(NT_STATEMENT,$1,NULL); }
		  | ifstatement { $$ = NewNode(NT_STATEMENT,$1,NULL); }
		  | whilestatement { $$ = NewNode(NT_STATEMENT,$1,NULL); }
		  | compoundStatement { $$ = NewNode(NT_STATEMENT,$1,NULL); }
		  | RETURN expression ';' { $$ = NewNode(NT_STATEMENT_RETURN,$2,NULL); }
		  ;

ifstatement : IF '(' expression ')' statement %prec IFPREC { $$ = NewNode(NT_IFSTATEMENT,$3,$5); }
			| IF '(' expression ')' statement ELSE statement { $$ = NewNode(NT_IFSTATEMENT,$3,$5,$7); }
			;

whilestatement : WHILE '(' expression ')' statement { $$ = NewNode(NT_WHILESTATEMENT,$3,$5); }
			   ;

compoundStatement : '{' '}' { $$ = NewNode(NT_STATEMENT_COMPOUND,NULL,NULL); }
				  | '{' statementList '}' { $$ = NewNode(NT_STATEMENT_COMPOUND,$2,NULL); }
				  ;

statementList : statementList statement { $$ = NewNode(NT_STATEMENTLIST,$1,$2); }
			  | statement { $$ = NewNode(NT_STATEMENTLIST,$1,NULL); }
			  ;

expression : NUMBER { $$ = $1; }
		   | IDENTIFIER 
		   | IDENTIFIER '('  ')' { $$ = NewNode(NT_EXPRESSION_FCALL,$1,NULL); }
		   | IDENTIFIER '(' args ')' { $$ = NewNode(NT_EXPRESSION_FCALL,$1,$3); }
		   | expression PLUS expression { $$ = NewNode(NT_EXPRESSION_PLUS,$1,$3); }
		   | expression MINUS expression { $$ = NewNode(NT_EXPRESSION_MINUS,$1,$3); }
		   | expression DIV expression { $$ = NewNode(NT_EXPRESSION_DIV,$1,$3); }
		   | expression MULT expression { $$ = NewNode(NT_EXPRESSION_MULT,$1,$3); }
		   | PLUS expression { $$ = NewNode(NT_EXPRESSION_UPLUS,$2,NULL); }
		   | MINUS expression { $$ = NewNode(NT_EXPRESSION_UMINUS,$2,NULL); }
		   | '(' expression ')' { $$ = $2; }
		   | IDENTIFIER '=' expression { $$ = NewNode(NT_EXPRESSION_ASSIGN,$1,$3); }
		   | expression OR expression { $$ = NewNode(NT_EXPRESSION_OR,$1,$3); }
		   | expression AND expression { $$ = NewNode(NT_EXPRESSION_AND,$1,$3); }
		   | NOT expression { $$ = NewNode(NT_EXPRESSION_NOT,$2,NULL); }
		   | expression GT expression { $$ = NewNode(NT_EXPRESSION_GT,$1,$3); }
		   | expression GTE expression { $$ = NewNode(NT_EXPRESSION_GTE,$1,$3); }
		   | expression LT expression { $$ = NewNode(NT_EXPRESSION_LT,$1,$3); }
		   | expression LTE expression { $$ = NewNode(NT_EXPRESSION_LTE,$1,$3); }
		   | expression EQUAL expression { $$ = NewNode(NT_EXPRESSION_EQUAL,$1,$3); }
		   | expression NEQUAL expression { $$ = NewNode(NT_EXPRESSION_NEQUAL,$1,$3); }
		   ;

args : expression { $$ = NewNode(NT_ARGS_FIRST,$1,NULL); }
	 | args ',' expression { $$ = NewNode(NT_ARGS_FOLLOWING,$1,$3); }
	 ;

fargs : IDENTIFIER { $$ = NewNode(NT_FARGS_FIRST,$1,NULL); }
	  | fargs ',' IDENTIFIER { $$ = NewNode(NT_FARGS_FOLLOWING,$1,$3); }
	  ;

%%

// callback
int yyerror(const char *message){
	printf("Line %d, %s\n",yylineno,message);
return 1;
}