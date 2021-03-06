// BisonIntro.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"
extern FILE *yyin;
int yyparse();


int main(int argc,char **argv){
	FILE *dotFile;
	yyin = fopen(argv[1], "r");

	yyparse();

	dotFile = fopen("tree.dot", "w");
	PrintTree(dotFile, root, NULL);

	system("dot -Tgif tree.dot -o tree.gif");
	eval(root);
    return 0;
}

