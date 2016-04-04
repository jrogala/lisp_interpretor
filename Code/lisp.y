%{
#include <cstdio>
#include <iostream>
#include <string>
#include "object.hh"
  
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" int yylineno;
extern "C" char *yytext;

void yyerror(char const *msg);

// #define test(s, x) {clog << "Bison: " << #s << ": " << x << endl;}
 #define test(s, x) {}

Object reverse(Object l) {
  Object m = nil();
  for (Object p = l; !null(p); p = cdr(p)) {
    m = cons(car(p), m);
  }
  return m;
}

Object just_read;

%}

/* Declarations */

%token <number_value> Token_number
%token <string_value> Token_symbol
%token <string_value> Token_string
%token Token_lpar Token_rpar 
%token Token_nil
%token Token_quote
%token Token_blank

/* %type <listObjectp_value> list_expr */
%type <Object_value> expr
%type <Object_value> list_expr
%type <Object_value> main

%union {
  Object	Object_value;
  int		number_value;
  char		*string_value;
}

%start main             /* the entry point */

%% 
/* Grammar rules */

/* 
http://tldp.org/HOWTO/Lex-YACC-HOWTO-6.html
6.2 Recursion: 'right is wrong'
*/

main: 
list_rpar expr          {$$ = $2; clog << "Read: " << $2 << endl; just_read = $$; YYACCEPT;}
;

expr:
Token_number            {test(number, $1); $$ = number_to_Object($1);}
| Token_nil             {test(nil, ""); $$ = nil();}
| Token_symbol		{test(symbol, $1); $$ = symbol_to_Object($1);}
| Token_string          {test(string, $1); $$ = string_to_Object($1);}
| Token_quote expr      {test(quote, ""); $$ = cons(string_to_Object("quote"), $2);}
| Token_lpar list_expr Token_rpar	{$$ = reverse($2);} // Right order is back! :-)
;

list_expr:
%empty			{$$ = nil();}
| list_expr expr 	{$$ = cons($2, $1); } // Watch it: reverse order... :-(
;

list_rpar:
%empty		{}
| list_rpar Token_rpar	{}
;

%%
/* Additional functions */

void yyerror(char const *s) {
  printf("yyerror: %s\n", s);
}

/*

	int yyerror(const char *s) {
	cerr << "ERROR: " << s << " at symbol \"" << yytext << "\"" 
	<< "on line " << yylineno << endl;
	exit(1);
	}

*/
