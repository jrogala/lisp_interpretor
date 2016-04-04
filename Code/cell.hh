#pragma once

#include <iostream>
#include <string>

using namespace std;

class Cell {
private:
  enum cell_sort {NUMBER, STRING, SYMBOL, PAIR};
  cell_sort sort;

  struct cell_pair {
    Cell *item;
    Cell *next;
  };

  union cell_value {
    int as_number;
    char *as_string;
    char *as_symbol;
    cell_pair as_pair;
  };

  cell_value value;
  static Cell cell_nil;
  void check();

  cell_sort get_sort() const;

public:
  Cell();

  bool is_number() const;
  bool is_string() const;
  bool is_symbol() const;
  bool is_pair() const;

  int to_number() const;
  string to_string() const;
  string to_symbol() const;
  Cell *to_pair_item() const;
  Cell *to_pair_next() const;

  static Cell *nil();

  void make_cell_number(int a);
  void make_cell_string(string s);
  void make_cell_symbol(string s);
  void make_cell_pair(Cell* p, Cell* q);
};

ostream& operator << (ostream& s, const Cell *p);

