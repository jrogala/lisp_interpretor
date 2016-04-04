#include "object.hh"
#include "env.hh"
#include "eval.hh"
#include <stdio.h>

extern Object just_read;
extern "C" int yyparse();
extern "C" FILE *yyin;

using namespace std;

int main() {
  Object a =  symbol_to_Object("a");
  Object b =  symbol_to_Object("b");
  Object one = number_to_Object(1);
  Object two = number_to_Object(2);

  Environment env;
  env.add_new_binding(Object_to_string(a), one);
  env.add_new_binding(Object_to_string(a), two);

  do {
    cout << "Lisp? " << flush;
    yyparse();
    Object l = just_read;
    cout << eval(l, env) << endl;
  } while (!feof(yyin));
}
