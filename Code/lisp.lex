%{
#define YY_DECL extern "C" int yylex()
	
#include <cstdio>
#include <iostream>
#include <string>
#include <cassert>
#include "object.hh"
using namespace std;
 
#include "lisp_yacc.h"

// #define trace(s) {clog << "Flex: " << #s << ": " << yytext << endl;}
#define trace(s) {}

char *erase_quotes(char *s) {
  assert(s[0] == '\"');
  s[strlen(s) - 1] = '\0';
  s[0] = '\0';
  return s+1;
}

%}

%option noyywrap

BLANK [ \t\n]
LINE [^\n]*\n
ALPHA [a-zA-Z]
OPERATION [-+*/.=_?><]
NUM [0-9]
SYMBOL ({OPERATION}|({ALPHA}({ALPHA}|{NUM})*))
NUMBER (-?{NUM}+)
LPAR \(
RPAR \)
QUOTE \'
NIL (nil)|(NIL)|(Nil)
STRING \"[^\"]*\"
COMMENT ;{LINE}

%%

{BLANK}		{trace(BLANK);}
{NIL}		{trace(NIL); return Token_nil;}
{SYMBOL}	{trace(SYMBOL); yylval.string_value = strdup(yytext); return Token_symbol;}
{NUMBER} 	{trace(NUMBER); yylval.number_value = atoi(yytext); return Token_number;}
{QUOTE}		{trace(QUOTE); return Token_quote;}
{LPAR}		{trace(LPAR); return Token_lpar;}
{RPAR}		{trace(RPAR); return Token_rpar;}
{STRING}	{trace(STRING); yylval.string_value = erase_quotes(strdup(yytext)); return Token_string;} 
{COMMENT}	{trace(COMMENT);}

%%
