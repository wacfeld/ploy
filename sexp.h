#ifndef SEXP_H
#define SEXP_H

#include <string>

enum atype {NUMBER, SYMBOL};

class Atom {
  atype type;
  int num;
  std::string symb;
};

class Sexp {
  bool eol;
  bool atom;
  Atom a;

  Sexp *car;
  Sexp *cdr;
};

#endif
