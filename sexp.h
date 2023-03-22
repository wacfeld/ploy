#ifndef SEXP_H
#define SEXP_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>

#include "lexer.h"
#include "proc.h"

enum atype {NUMBER, SYMBOL, BOOLEAN, PROCEDURE};

class Atom {
public:
  atype type;
  int num;
  std::string symb;
  bool boole;
  Proc proc;

  Atom() {}
  Atom(int n): type{NUMBER}, num{n} {}
  Atom(std::string s): type{SYMBOL}, symb{s} {}
  Atom(bool b): type{BOOLEAN}, boole{boole} {}
};

class Sexp {
public:
  bool atom;
  Atom a;

  Sexp *car;
  Sexp *cdr;

  Sexp(bool atom): atom{atom}, car{nullptr}, cdr{nullptr} {}

  ~Sexp() {
    delete car;
    delete cdr;
  }
};


Sexp *get_sexp(std::vector<Token> toks);
Sexp *get_list(std::vector<Token> toks);

int list_len(Sexp *e);
Sexp *eval(Sexp *e);

extern Sexp *the_empty_list;
  
#endif
