/*
* Copyright 2016 sprocket
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License, or (at your option)
* any later version.
*/

#include <stdio.h>

#include "ElasticPL.h"
#include "../miner.h"

/*****************************************************************************
ElasticPL Token List

Format:  Str, Len, Type, Exp, Prec

Str:	Token String
Len:	String Length Used For "memcmp"
Type:	Enumerated Token Type
Exp:	Enumerated Num Of Expressions To Link To Node
Inputs: Number Of Required Inputs To Operator / Function
Prec:	(Precedence) Determines Parsing Order
******************************************************************************/
struct EPL_TOKEN_LIST epl_token[] = {
	{ "<eof>",						5,	TOKEN_EOF,			EXP_NONE,		0,	0 },
	{ "{",							1,	TOKEN_BLOCK_BEGIN,	EXP_STATEMENT,	2,	1 },
	{ "}",							1,	TOKEN_BLOCK_END,	EXP_STATEMENT,	2,	1 },
	{ "(",							1,	TOKEN_OPEN_PAREN,	EXP_NONE,		0,	1 },
	{ ")",							1,	TOKEN_CLOSE_PAREN,	EXP_NONE,		0,	1 },
	{ ";",							1,	TOKEN_END_STATEMENT,EXP_NONE,		0,	2 },
	{ "verify",						6,	TOKEN_VERIFY,		EXP_STATEMENT,	1,	2 },
	{ "repeat",						6,	TOKEN_REPEAT,		EXP_STATEMENT,	2,	2 },
	{ "if",							2,	TOKEN_IF,			EXP_STATEMENT,	2,	2 },
	{ "else",						4,	TOKEN_ELSE,			EXP_STATEMENT,	2,	2 },

	{ "m[",							2,	TOKEN_VAR_BEGIN,	EXP_EXPRESSION,	1,	4 },
	{ "]",							1,	TOKEN_VAR_END,		EXP_EXPRESSION,	1,	4 },

	{ "*",							1,	TOKEN_MUL,			EXP_EXPRESSION,	2,	14 },	// Multiplicative
	{ "/",							1,	TOKEN_DIV,			EXP_EXPRESSION,	2,	14 },	// Multiplicative
	{ "%",							1,	TOKEN_MOD,			EXP_EXPRESSION,	2,	14 },	// Multiplicative

	{ "+",							1,	TOKEN_ADD,			EXP_EXPRESSION,	2,	13 },	// Additive
	{ "-",							1,	TOKEN_SUB,			EXP_EXPRESSION,	2,	13 },	// Additive
	{ "-",							1,	TOKEN_NEG,			EXP_EXPRESSION,	1,	15 },	// Additive

	{ "<<<",						3,	TOKEN_LROT,			EXP_EXPRESSION,	2,	12 },	// Shift
	{ "<<",							2,	TOKEN_LSHIFT,		EXP_EXPRESSION,	2,	12 },	// Shift
	{ ">>>",						3,	TOKEN_RROT,			EXP_EXPRESSION,	2,	12 },	// Shift
	{ ">>",							2,	TOKEN_RSHIFT,		EXP_EXPRESSION,	2,	12 },	// Shift

	{ "<=",							2,	TOKEN_LE,			EXP_EXPRESSION,	2,	11 },	// Relational
	{ ">=",							2,	TOKEN_GE,			EXP_EXPRESSION,	2,	11 },	// Relational
	{ "<",							1,	TOKEN_LT,			EXP_EXPRESSION,	2,	11 },	// Relational
	{ ">",							1,	TOKEN_GT,			EXP_EXPRESSION,	2,	11 },	// Relational

	{ "==",							2,	TOKEN_EQ,			EXP_EXPRESSION,	2,	10 },	// Equality
	{ "!=",							2,	TOKEN_NE,			EXP_EXPRESSION,	2,	10 },	// Equality

	{ "&&",							2,	TOKEN_AND,			EXP_EXPRESSION,	2,	6 },	// Logical AND
	{ "||",							2,	TOKEN_OR,			EXP_EXPRESSION,	2,	5 },	// Logical OR

	{ "&",							1,	TOKEN_BITWISE_AND,	EXP_EXPRESSION,	2,	9 },	// Bitwise AND
	{ "and",						3,	TOKEN_BITWISE_AND,	EXP_EXPRESSION,	2,	9 },	// Bitwise AND
	{ "^",							1,	TOKEN_BITWISE_XOR,	EXP_EXPRESSION,	2,	8 },	// Bitwise XOR
	{ "xor",						3,	TOKEN_BITWISE_XOR,	EXP_EXPRESSION,	2,	8 },	// Bitwise XOR
	{ "|",							1,	TOKEN_BITWISE_OR,	EXP_EXPRESSION,	2,	7 },	// Bitwise OR
	{ "or",							2,	TOKEN_BITWISE_OR,	EXP_EXPRESSION,	2,	7 },	// Bitwise OR

	{ "=",							1,	TOKEN_ASSIGN,		EXP_STATEMENT,	2,	3 },	// Assignment

	{ "~",							1,	TOKEN_COMPL,		EXP_EXPRESSION,	1,	15 },	// Unary Operator
	{ "!",							1,	TOKEN_NOT,			EXP_EXPRESSION,	1,	15 },	// Unary Operator
	{ "true",						4,	TOKEN_TRUE,			EXP_EXPRESSION,	0,	15 },	// Unary Operator
	{ "false",						5,	TOKEN_FALSE,		EXP_EXPRESSION,	0,	15 },	// Unary Operator

	{ "sha256",						6,	TOKEN_SHA256,		EXP_FUNCTION,	2,	60 },	// Built In Functions
	{ "sha512",						6,	TOKEN_SHA512,		EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "whirlpool",					9,	TOKEN_WHIRLPOOL,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "md5",						3,	TOKEN_MD5,			EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp192k1privtopub",			18,	TOKEN_SECP192K_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp192k1pointadd",			17,	TOKEN_SECP192K_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp192k1pointsub",			17,	TOKEN_SECP192K_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp192k1pointscalarmult",	24,	TOKEN_SECP192K_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp192k1pointnegate",		20,	TOKEN_SECP192K_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "secp192r1privtopub",			18,	TOKEN_SECP192R_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp192r1pointadd",			17,	TOKEN_SECP192R_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp192r1pointsub",			17,	TOKEN_SECP192R_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp192r1pointscalarmult",	24,	TOKEN_SECP192R_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp192r1pointnegate",		20,	TOKEN_SECP192R_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "secp224k1privtopub",			18,	TOKEN_SECP224K_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp224k1pointadd",			17,	TOKEN_SECP224K_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp224k1pointsub",			17,	TOKEN_SECP224K_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp224k1pointscalarmult",	24,	TOKEN_SECP224K_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp224k1pointnegate",		20,	TOKEN_SECP224K_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "secp224r1privtopub",			18,	TOKEN_SECP224R_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp224r1pointadd",			17,	TOKEN_SECP224R_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp224r1pointsub",			17,	TOKEN_SECP224R_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp224r1pointscalarmult",	24,	TOKEN_SECP224R_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp224r1pointnegate",		20,	TOKEN_SECP224R_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "secp256k1privtopub",			18,	TOKEN_SECP256K_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp256k1pointadd",			17,	TOKEN_SECP256K_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp256k1pointsub",			17,	TOKEN_SECP256K_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp256k1pointscalarmult",	24,	TOKEN_SECP256K_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp256k1pointnegate",		20,	TOKEN_SECP256K_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "secp256r1privtopub",			18,	TOKEN_SECP256R_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp256r1pointadd",			17,	TOKEN_SECP256R_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp256r1pointsub",			17,	TOKEN_SECP256R_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp256r1pointscalarmult",	24,	TOKEN_SECP256R_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp256r1pointnegate",		20,	TOKEN_SECP256R_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "secp384r1privtopub",			18,	TOKEN_SECP384R_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "secp384r1pointadd",			17,	TOKEN_SECP384R_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp384r1pointsub",			17,	TOKEN_SECP384R_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp384r1pointscalarmult",	24,	TOKEN_SECP384R_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "secp384r1pointnegate",		20,	TOKEN_SECP384R_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "prime192v1privtopub",		18,	TOKEN_PRM192V1_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "prime192v1pointadd",			17,	TOKEN_PRM192V1_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v1pointsub",			17,	TOKEN_PRM192V1_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v1pointscalarmult",	24,	TOKEN_PRM192V1_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v1pointnegate",		20,	TOKEN_PRM192V1_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "prime192v2privtopub",		18,	TOKEN_PRM192V2_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "prime192v2pointadd",			17,	TOKEN_PRM192V2_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v2pointsub",			17,	TOKEN_PRM192V2_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v2pointscalarmult",	24,	TOKEN_PRM192V2_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v2pointnegate",		20,	TOKEN_PRM192V2_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "prime192v3privtopub",		18,	TOKEN_PRM192V3_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "prime192v3pointadd",			17,	TOKEN_PRM192V3_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v3pointsub",			17,	TOKEN_PRM192V3_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v3pointscalarmult",	24,	TOKEN_PRM192V3_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime192v3pointnegate",		20,	TOKEN_PRM192V3_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "prime256v1privtopub",		18,	TOKEN_PRM256V1_PTP,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "prime256v1pointadd",			17,	TOKEN_PRM256V1_PA,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime256v1pointsub",			17,	TOKEN_PRM256V1_PS,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime256v1pointscalarmult",	24,	TOKEN_PRM256V1_PSM,	EXP_FUNCTION,	5,	60 },	// Built In functions
	{ "prime256v1pointnegate",		20,	TOKEN_PRM256V1_PN,	EXP_FUNCTION,	3,	60 },	// Built In functions
	{ "tiger",						5,	TOKEN_TIGER,		EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "ripemd160",					9,	TOKEN_RIPEMD160,	EXP_FUNCTION,	2,	60 },	// Built In functions
	{ "ripemd128",					9,	TOKEN_RIPEMD128,	EXP_FUNCTION,	2,	60 }	// Built In functions
};

extern bool init_token_list(SOURCE_TOKEN_LIST *token_list, size_t size) {
	token_list->token = malloc(size * sizeof(SOURCE_TOKEN));
	token_list->num = 0;
	token_list->size = size;

	if (!token_list->token)
		return false;
	else
		return true;
}

static bool add_token(SOURCE_TOKEN_LIST *token_list, int token_id, char *literal, int line_num) {
	char *str;

	// Increase Token List Size If Needed
	if (token_list->num == token_list->size) {
		token_list->size += 256;
		token_list->token = (SOURCE_TOKEN *)realloc(token_list->token, token_list->size * sizeof(SOURCE_TOKEN));

		if (!token_list->token)
			return false;
	}

	// EPL Tokens
	if (token_id >= 0) {

		// Determine If '-' Is Binary Or Unary
		if (epl_token[token_id].type == TOKEN_SUB) {
			if (token_list->num == 0 || token_list->token[token_list->num - 1].exp != EXP_EXPRESSION || token_list->token[token_list->num - 1].inputs > 1) {
				token_id++;
			}
		}

		token_list->token[token_list->num].type = epl_token[token_id].type;
		token_list->token[token_list->num].exp = epl_token[token_id].exp;
		token_list->token[token_list->num].literal = NULL;
		token_list->token[token_list->num].inputs = epl_token[token_id].inputs;
		token_list->token[token_list->num].prec = epl_token[token_id].prec;
	}
	// Literals
	else if (literal != NULL) {
		str = calloc(1, strlen(literal) + 1);

		if (!str) return false;

		strcpy(str, literal);
		token_list->token[token_list->num].literal = str;
		token_list->token[token_list->num].type = TOKEN_LITERAL;
		token_list->token[token_list->num].exp = EXP_EXPRESSION;
		token_list->token[token_list->num].inputs = 0;
		token_list->token[token_list->num].prec = -1;
	}
	// Error
	else {
		return false;
	}

	token_list->token[token_list->num].token_id = token_id;
	token_list->token[token_list->num].line_num = line_num;
	token_list->num++;

	return true;
}

extern void delete_token_list(SOURCE_TOKEN_LIST *token_list) {
	int i;

	for (i = 0; i < token_list->num; i++) {
		if (token_list->token[i].literal)
			free(token_list->token[i].literal);
	}

	free(token_list->token);
	token_list->token = NULL;
	token_list->num = 0;
	token_list->size = 0;
}

static int validate_token_list(SOURCE_TOKEN_LIST *token_list) {
	int i, j, len;
	char c;

	for (i = 0; i < token_list->num; i++) {

		// Validate That All Literals Are Numeric
		if (token_list->token[i].type == TOKEN_LITERAL) {

			len = strlen(token_list->token[i].literal);

			for (j = 0; j < len; j++) {
				c = token_list->token[i].literal[j];
				if (!(c >= '0' && c <= '9') && c != '-')
					return i;
			}
		}
	}

	return -1;
}

extern bool get_token_list(char *str, SOURCE_TOKEN_LIST *token_list) {
	char c, literal[100];
	int i, idx, len, token_id, line_num, token_list_sz, literal_idx;

	token_list_sz = sizeof(epl_token) / sizeof(epl_token[0]);

	len = strlen(str);

	idx = 0;
	line_num = 1;
	literal_idx = 0;
	memset(literal, 0, 100);

	while (idx < len) {
		token_id = -1;
		c = str[idx];

		// Remove Whitespace
		if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {

			if (literal_idx > 0) {
				add_token(token_list, -1, literal, line_num);
				literal_idx = 0;
				memset(literal, 0, 100);
			}

			// Increment Line Number Counter
			if (c == '\n') {
				line_num++;
			}

			idx++;
			continue;
		}

		// Check For EPL Token
		for (i = 0; i < token_list_sz; i++) {

			if (memcmp(&str[idx], epl_token[i].str, epl_token[i].len) == 0) {
				token_id = i;
				break;
			}

		}

		if (token_id >= 0) {

			if (literal_idx > 0) {
				add_token(token_list, -1, literal, line_num);
				literal_idx = 0;
				memset(literal, 0, 100);
			}

			add_token(token_list, token_id, NULL, line_num);
			idx += epl_token[token_id].len;
		}
		else {
			literal[literal_idx] = c;
			literal_idx++;
			idx++;
		}
	}

	//	dump_token_list(token_list);

	idx = validate_token_list(token_list);

	if (idx >= 0) {
		applog(LOG_ERR, "Syntax Error - Line: %d  Invalid Operator \"%s\"", token_list->token[idx].line_num, token_list->token[idx].literal);
		return false;
	}

	return true;
}


// Temporary - For Debugging Only
static void dump_token_list(SOURCE_TOKEN_LIST *token_list)
{
	int i;

	fprintf(stdout, "\nNum\tLine\tToken\tToken ID\n");
	printf("--------------------------------\n");
	for (i = 0; i < token_list->num; i++) {
		if (token_list->token[i].type == TOKEN_LITERAL)
			fprintf(stdout, "%d:\t%d\t\"%s\"\t%d\n", i, token_list->token[i].line_num, token_list->token[i].literal, token_list->token[i].type);
		else
			fprintf(stdout, "%d:\t%d\t%s\t%d\n", i, token_list->token[i].line_num, epl_token[token_list->token[i].token_id].str, token_list->token[i].type);
	}
}