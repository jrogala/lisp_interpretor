#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>
#include "env.hh"

using namespace std;

Binding::Binding(string _name, Object _value):
  name(_name), value(_value) {}
string  Binding::get_name() const {
  return name;
}
Object  Binding::get_value() const {
  return value;
}

class No_Binding_Exception: public runtime_error {
private:
  string name;
public:
  No_Binding_Exception(string _name): runtime_error("No binding for name: " + _name) {
    name = _name;
  }
  virtual ~No_Binding_Exception() throw () {}
};

class Zipping_Exception: public runtime_error {
private:
  string message;
  Object lobjs;
public:
  Zipping_Exception(Object _lobjs, string _message): runtime_error("Zipping exception: " + _message) {
    message = _message;
    lobjs = _lobjs;
    clog << message << ": " << lobjs << endl;
  }
  virtual ~Zipping_Exception() throw () {}
};

Environment::Environment() {
  contents = vector<Binding>();
}

void Environment::add_new_binding(string name, Object value) {
  contents.push_back(Binding(name, value));
}
void Environment::extend_env(Object lpars, Object lvals) {
  if (null(lpars) && null(lvals)) return;
  if (null(lpars) && !null(lvals)) throw Zipping_Exception(lvals, "Too many values");
  if (!null(lpars) && null(lvals)) throw Zipping_Exception(lpars, "Too many parameters");
  add_new_binding(Object_to_string(car(lpars)), car(lvals));
  extend_env(cdr(lpars), cdr(lvals));
}
Object Environment::find_value(string name) {
  for (int i = contents.size() - 1;; i--) {
    if (i < 0) throw No_Binding_Exception(name);
    if (contents.at(i).get_name() == name) return contents.at(i).get_value();
  }
}

void Environment::print(ostream& s) {
  s << "\t| ";
  for (int i = contents.size() - 1; i >= 0; i--) {
    s << contents.at(i).get_name() << ": " << contents.at(i).get_value() << "; ";
  }
}

ostream& operator << (ostream& s, Environment& env) {
  env.print(s);
  return s;
}
