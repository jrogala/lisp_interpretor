#pragma once

#include "cell.hh"

typedef Cell* Object;

Object nil();
bool null(Object l);

Object cons(Object a, Object l);
Object car(Object l);
Object cdr(Object l);
Object empty();
bool is_empty(Object l);

Object number_to_Object(int n);
Object string_to_Object(string s);
Object symbol_to_Object(string s);

int Object_to_number(Object l);
string Object_to_string(Object l);
