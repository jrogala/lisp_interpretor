#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "object.hh"

using namespace std;

class Binding {
private:
  string name;
  Object value;
public:
  Binding(string _name, Object _value);
  string get_name() const;
  Object get_value() const;
};

class Environment {
private:
  vector<Binding> contents;
public:
  Environment();
  void add_new_binding(string name, Object value);
  void extend_env(Object lpars, Object lvals);
  Object find_value(string name);
  void print(ostream& s);
};

ostream& operator << (ostream& s, Environment& env);
