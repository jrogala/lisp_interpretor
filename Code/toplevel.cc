#include "eval.hh"
#include "env.hh"
#include "read.hh"
#include "object.hh"
#include <stdio.h>


extern "C" FILE *yyin;

bool is_setq(Object l){
  if (!listp(l)) return false;
  if (null(l)) return false;
  if (!(car(l)->is_string() || car(l)->is_symbol())) return false;
  if (Object_to_string(car(l)) != "setq") return false;
  if (null(cdr(l))) return false; // renvoyer des erreurs pour rattraper les erreurs et informer l'utilisateur des erreurs qu'il a codé en faisant des erreurs.
  if (null(cddr(l))) return false;
  if (null(cdddr(l))) return true;
  return false;
}

void toplevel(){
  Environment env;
  do {
    Object l = read_obj();

    if (is_setq(l)) {
      env.add_new_binding(Object_to_string(cadr(l)),eval(caddr(l),env));
      cout << "SET" << endl;
      continue;
    }
    cout << eval(l,env) << endl;
    } while (!feof(yyin));
}
