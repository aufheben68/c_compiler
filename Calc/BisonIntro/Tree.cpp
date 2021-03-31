#include "Tree.h"

int g_serialNumberCounter = 0;
const char *nodeTypeStrings[] = { "NT_IDENTIFIER", "NT_NUMBER",
								  "NT_EXPRESSION_PLUS", "NT_EXPRESSION_MINUS",
								  "NT_EXPRESSION_DIV", "NT_EXPRESSION_MULT", "NT_EXPRESSION_UPLUS",
								  "NT_EXPRESSION_UMINUS", "NT_EXPRESSION_ASSIGN", "NT_COMPILEUNIT_STATEMENT",
								  "NT_COMPILEUNIT_FUNDEF", "NT_EXPRESSION_OR","NT_EXPRESSION_AND","NT_EXPRESSION_NOT",
								  "NT_EXPRESSION_GT","NT_EXPRESSION_GTE","NT_EXPRESSION_LT","NT_EXPRESSION_LTE",
								  "NT_EXPRESSION_EQUAL","NT_EXPRESSION_NEQUAL", "NT_STATEMENT","NT_STATEMENTLIST",
								  "NT_STATEMENT_COMPOUND", "NT_IFSTATEMENT", "NT_WHILESTATEMENT", "NT_EXPRESSION_FCALL", "NT_FUNDEF",
								  "NT_ARGS_FIRST", "NT_ARGS_FOLLOWING", "NT_FARGS_FIRST", "NT_FARGS_FOLLOWING", "NT_STATEMENT_RETURN" };
struct ptnode *symbolTable[50] = { NULL };
struct ptnode *root;


struct ptnode *getSymbol(char *varName)
{
	struct ptnode *node = NULL;
	int i = 0;
	while (i < 50 && symbolTable[i] != NULL) {
		if (!strcmp(symbolTable[i]->text, varName))
			return symbolTable[i];
		i++;
	}

	if (i < 50) {
		symbolTable[i] = NewNode(NT_IDENTIFIER, NULL, NULL, NULL, varName);
		return symbolTable[i];
	}
	else
		return NULL;
}

struct ptnode *NewNode(NodeType type,
	struct ptnode *left, struct ptnode *right, struct ptnode *middle, char *text) {
	char buf[10];
	struct ptnode *newNode;

	newNode = (struct ptnode *)malloc(sizeof(struct ptnode));
	newNode->type = type;
	newNode->serialNumber = g_serialNumberCounter++;
	newNode->left = left;
	newNode->right = right;
	newNode->middle = middle;
	newNode->value = (int *)malloc(sizeof(int));

	newNode->text = NULL;
	if (type == NT_NUMBER || type == NT_IDENTIFIER) {
		newNode->text = strdup(text);
		if (type == NT_NUMBER) {
			*(newNode->value) = atoi(text);
		}
	}

	// "ADDITION_45"
	newNode->label = (char *)malloc(50);
	strcpy(newNode->label, nodeTypeStrings[type]);
	strcat(newNode->label, "_");
	itoa(newNode->serialNumber, buf, 10);
	strcat(newNode->label, buf);
	return newNode;
}

void mapActualToFormalArguments(struct ptnode *arg, struct ptnode *farg) {
	if (arg->left != NULL) {
		mapActualToFormalArguments(arg->left, farg->left);
	}
	else {
		*(farg->value) = *(arg->value);
	}
	if (arg->right != NULL) {
		mapActualToFormalArguments(arg->right, farg->right);
	}

}

void PrintTree(FILE *fp,
	struct ptnode *current, struct ptnode *parent) {

	// PREORDER
	if (parent != NULL) {
		fprintf(fp, "\"%s\"->\"%s\";\n", parent->label, current->label);
	}
	else {
		fprintf(fp, "digraph G{\n");
	}

	// VISIT
	if (current->left != NULL) {
		PrintTree(fp, current->left, current);
	}
	if (current->middle != NULL) {
		PrintTree(fp, current->middle, current);
	}
	if (current->right != NULL) {
		PrintTree(fp, current->right, current);
	}

	// POSTERDER
	if (parent == NULL) {
		fprintf(fp, "}");

		// Don't forget to close the file. Otherwise Graphviz will fail silently
		// and won't produce the .gif file
		fclose(fp);
	}
}

int eval(struct ptnode* current) {
	int denumerator;
	int result;
	static struct ptnode *fun;
	switch (current->type)
	{
	case NT_IDENTIFIER:
		if (current->value != NULL)
			return *(current->value);
		else {
			printf("%s not initialized.", current->text);
			exit(1);
		}
		break;
	case NT_NUMBER:
		return *(current->value);
		break;
	case NT_EXPRESSION_PLUS:
		*(current->value) = eval(current->left) + eval(current->right);
		return *(current->value);
		break;
	case NT_EXPRESSION_MINUS:
		*(current->value) = eval(current->left) - eval(current->right);
		return *(current->value);
		break;
	case NT_EXPRESSION_DIV:
		denumerator = eval(current->right);
		if (denumerator != 0) {
			*(current->value) = eval(current->left) / denumerator;
			return *(current->value);
		}
		else {
			printf("Division by zero");
			exit(1);
		}
		break;
	case NT_STATEMENT_RETURN:
		*(fun->value) = eval(current->left);
		return *(fun->value);
		break;
	case NT_EXPRESSION_MULT:
		*(current->value) = eval(current->left) * eval(current->right);
		return *(current->value);
		break;
	case NT_EXPRESSION_UPLUS:
		*(current->value) = eval(current->left);
		return *(current->value);
		break;
	case NT_EXPRESSION_UMINUS:
		*(current->value) = -eval(current->left);
		return *(current->value);
		break;
	case NT_EXPRESSION_ASSIGN:
		result = eval(current->right);
		*(current->left->value) = result;
		printf("%s = %d\n", current->left->text, result);
		break;
	case NT_COMPILEUNIT_STATEMENT:
		eval(current->left);
		if (current->right != NULL)
			eval(current->right);
		break;
	case NT_COMPILEUNIT_FUNDEF:
		eval(current->left);
		if (current->right != NULL)
			eval(current->right);
		break;
	case NT_EXPRESSION_OR:
		return eval(current->left) || eval(current->right);
		break;
	case NT_EXPRESSION_AND:
		return eval(current->left) && eval(current->right);
		break;
	case NT_EXPRESSION_NOT:
		return !eval(current->left);
		break;
	case NT_EXPRESSION_GT:
		return eval(current->left) > eval(current->right);
		break;
	case NT_EXPRESSION_GTE:
		return eval(current->left) >= eval(current->right);
		break;
	case NT_EXPRESSION_LT:
		return eval(current->left) < eval(current->right);
		break;
	case NT_EXPRESSION_LTE:
		return eval(current->left) <= eval(current->right);
		break;
	case NT_EXPRESSION_EQUAL:
		return eval(current->left) == eval(current->right);
		break;
	case NT_EXPRESSION_NEQUAL:
		return eval(current->left) != eval(current->right);
		break;
	case NT_STATEMENT:
		eval(current->left);
		break;
	case NT_STATEMENTLIST:
		eval(current->left);
		if (current->right != NULL)
			eval(current->right);
		break;
	case NT_STATEMENT_COMPOUND:
		if (current->left != NULL)
			eval(current->left);
		break;
	case NT_IFSTATEMENT:
		if (eval(current->left))
			eval(current->right);
		else if (current->middle != NULL)
			eval(current->middle);
		break;
	case NT_WHILESTATEMENT:
		while (eval(current->left))
			eval(current->right);
		break;
	case NT_EXPRESSION_FCALL:
		eval(current->right);
		struct ptnode *funID = getSymbol(current->left->text);
		fun = funID;
		mapActualToFormalArguments(current->right, funID->funBody->right);
		*(current->value) = eval(funID->funBody->middle);
		return *(fun->value);
		break;
	}
}