#ifndef TREE_
#define TREE_
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "demo.tab.h"

typedef enum NodeTypes{ NT_IDENTIFIER, NT_NUMBER, 
						NT_EXPRESSION_PLUS, NT_EXPRESSION_MINUS,
						NT_EXPRESSION_DIV, NT_EXPRESSION_MULT, NT_EXPRESSION_UPLUS,
						NT_EXPRESSION_UMINUS, NT_EXPRESSION_ASSIGN, NT_COMPILEUNIT_STATEMENT,
						NT_COMPILEUNIT_FUNDEF,	NT_EXPRESSION_OR,NT_EXPRESSION_AND,NT_EXPRESSION_NOT,
						NT_EXPRESSION_GT,NT_EXPRESSION_GTE,NT_EXPRESSION_LT,NT_EXPRESSION_LTE,
						NT_EXPRESSION_EQUAL,NT_EXPRESSION_NEQUAL, NT_STATEMENT,NT_STATEMENTLIST, NT_STATEMENT_COMPOUND,
						NT_IFSTATEMENT, NT_WHILESTATEMENT, NT_EXPRESSION_FCALL, NT_FUNDEF,
						NT_ARGS_FIRST, NT_ARGS_FOLLOWING, NT_FARGS_FIRST, NT_FARGS_FOLLOWING, NT_STATEMENT_RETURN
} NodeType;
extern struct ptnode *root;
struct ptnode {
	NodeType type;
	int *value;
	char *label;
	char *text;
	int serialNumber;
	struct ptnode *left; 
	struct ptnode *middle;
	struct ptnode *right;
	struct ptnode *funBody = NULL;
};

struct ptnode *NewNode(NodeType type,
	struct ptnode *left, struct ptnode *right, struct ptnode *middle = NULL, char *text = NULL);

void PrintTree(FILE *fp,
	struct ptnode *current, struct ptnode *parent);

struct ptnode *getSymbol(char *varName);

int eval(struct ptnode* current);

#endif