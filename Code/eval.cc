#include <stdexcept>
#include <string>
#include <cassert>
#include "eval.hh"
#include "object.hh"

using namespace std;

bool numberp(Object l) {
  return l -> is_number();
}

bool stringp(Object l) {
  return l -> is_string();
}

bool symbolp(Object l) {
  return l -> is_symbol();
}

bool listp(Object l) {
  return l -> is_pair();
}

Object cadr(Object l) {
  return car(cdr(l));
}

Object cddr(Object l) {
  return cdr(cdr(l));
}

Object caddr(Object l) {
  return car(cddr(l));
}

Object cdddr(Object l) {
  return cdr(cddr(l));
}

Object cadddr(Object l) {
  return car(cdddr(l));
}

class Evaluation_Exception: public runtime_error {
private:
  Object obj;
  Environment env;
  string message;
public:
  Evaluation_Exception(Object _obj, Environment _env, string _message):
    runtime_error("Evaluation error:" + _message) {
    obj = _obj;
    env = _env;
    message = _message;
  }
  virtual ~Evaluation_Exception() throw () {}
};

Object eval(Object l, Environment env);
Object apply(Object f, Object lvals, Environment env);
Object eval_list(Object largs, Environment env);

Object eval(Object l, Environment env) {
  clog << "\teval: " << l << env << endl;

  if (null(l)) return l;
  if (numberp(l)) return l;
  if (stringp(l)) return l;
  if (symbolp(l)) return env.find_value(Object_to_string(l));
  assert(listp(l));
  Object f = car(l);
  if (symbolp(f)) {
    if (Object_to_string(f) == "quote") return cadr(l);
    if (Object_to_string(f) == "if") {
      Object test_part = cadr(l);
      Object then_part = caddr(l);
      Object else_part = cadddr(l);
      Object test_value = eval(test_part, env);
      if (null(test_value)) return eval(else_part, env);
      return eval(then_part, env);
    }
    if (Object_to_string(f) == "lambda") return l;
    if (Object_to_string(f) == "cond") return apply(f, cdr(l), env);
  }
  // It is a function applied to arguments
  Object vals = eval_list(cdr(l), env);
  return apply(f, vals, env);
}

Object eval_list(Object largs, Environment env) {
  if (null(largs)) return largs;
  return cons(eval(car(largs), env), eval_list(cdr(largs), env));
}


Object do_plus(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a + b);
}

Object do_minus(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a - b);
}

Object do_times(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a * b);
}

Object do_division(Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  return number_to_Object(a / b);
}

Object test_equal (Object lvals) {
  int a = Object_to_number(car(lvals));
  int b = Object_to_number(cadr(lvals));
  if (a == b) {
    return (symbol_to_Object("t"));
  }
  else {
    return(nil());
  }
}


Object apply(Object f, Object lvals, Environment env) {
  clog << "\tapply: " << f << " " << lvals << env << endl;

  if (null(f)) throw Evaluation_Exception(f, env, "Cannot apply nil");
  if (numberp(f)) throw Evaluation_Exception(f, env, "Cannot apply a number");
  if (stringp(f))  throw Evaluation_Exception(f, env, "Cannot apply a string");
  if (symbolp(f)) {
    if (Object_to_string(f) == "+") return do_plus(lvals);
    if (Object_to_string(f) == "-") return do_minus(lvals);
    if (Object_to_string(f) == "*") return do_times(lvals);
    if (Object_to_string(f) == "/") return do_division(lvals);
    if (Object_to_string(f) == "=") return test_equal(lvals);
    if (Object_to_string(f) == "car") return car(lvals);
    if (Object_to_string(f) == "cdr") return cdr(lvals);
    if (Object_to_string(f) == "quote") return lvals;
    if (Object_to_string(f) == "cond") {
      Object clause;
      Object condition;
      Object result;
      int b = 1;
      do {
        clause = car(lvals);
        condition = car(clause);
        b = !null(eval(condition, env));
        if (b) {
          result = eval(cadr(clause), env);
        }
      }
      while(!b);
      return result;
    }
    Object new_f = env.find_value(Object_to_string(f));
    return apply(new_f, lvals, env);
  }
  assert(listp(f));
  if (Object_to_string(car(f)) == "lambda") {
    Object lpars = cadr(f);
    Object body = caddr(f);
    Environment new_env = env;
    new_env.extend_env(lpars, lvals);
    return eval(body, new_env);
  }
  throw Evaluation_Exception(f, env, "Cannot apply a list");
  assert(false);
}
