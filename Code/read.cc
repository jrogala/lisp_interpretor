#include <stdio.h>
#include "object.hh"

extern Object just_read;
extern "C" int yyparse();

Object read_obj(){
  cout << "Lisp? " << flush;
  yyparse();
  Object l = just_read;
  return l;
}
