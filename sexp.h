#ifndef SEXP_H
#define SEXP_H

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>

#include "lexer.h"
#include "proc.h"
#include "repl.h"

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
  Atom(bool b): type{BOOLEAN}, boole{b} {}
};

extern std::vector<Sexp*> allocated;
void cleanup();

class Sexp {
public:
  bool atom;
  Atom a;

  Sexp *car;
  Sexp *cdr;

  Sexp(bool atom): atom{atom}, car{nullptr}, cdr{nullptr} {allocated.push_back(this);}

};

Sexp *get_sexp(std::vector<Token> toks);
Sexp *get_list(std::vector<Token> toks);

Sexp *index(Sexp *e, int i);
bool isempty(Sexp *e);
bool islist(Sexp *e);
bool issymbol(Sexp *e);
int list_len(Sexp *e);
bool isnum(Sexp *e);

Sexp *make_bool(bool truth);
Sexp *make_num(int n);
bool eval_truth(Sexp *e);
// Sexp *eval(Sexp *e);
Sexp *eval(Sexp *e, const std::map<std::string, Sexp*> &env);
Sexp *eval_list(Sexp *e, const std::map<std::string, Sexp*> &env);

extern Sexp *the_empty_list;
  
#endif
