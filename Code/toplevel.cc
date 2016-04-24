#include "eval.hh"
#include "env.hh"
#include "read.hh"
#include "object.hh"
#include <stdio.h>
#include <string>
#include <stdexcept>


extern "C" FILE *yyin;

bool is_setq(Object l){
  if (!listp(l)) throw 1;
  if (null(l)) throw 1;
  if (!(car(l)->is_string() || car(l)->is_symbol())) throw 1;
  if (Object_to_string(car(l)) != "setq") throw 1;
  if (null(cdr(l))) throw 2; // renvoyer des erreurs pour rattraper les erreurs et informer l'utilisateur des erreurs qu'il a codé en faisant des erreurs.
  if (null(cddr(l))) throw 2;
  if (null(cdddr(l))) throw 0;
  throw 3;
}

void toplevel(){
  Environment env;
  Object meval;
  do {
    Object l = read_obj();
    try {is_setq(l);}
			catch (int e){
				switch (e) {
				case 1: break;
				case 2: cout << "too short" << endl;continue;break;
				case 3: cout << "too long" << endl;continue;break;
				case 0:
				  env.add_new_binding(Object_to_string(cadr(l)),eval(caddr(l),env));
				  cout << "SET" << endl;
				  continue;break;
				}
    }
	try {meval = eval(l,env);}
	catch (runtime_error e){
		continue;
		}
    cout << meval<< endl;
    } while (!feof(yyin));
}
