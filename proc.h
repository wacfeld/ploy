#ifndef PROC_H
#define PROC_H

#include <map>
#include <string>
#include <vector>

#include "repl.h"

class Sexp;

class Proc {
public:
  bool prim;
  Sexp *(*f)(Sexp *);
  // std::vector<std::string> formals; // TODO allow other types of formals
  Sexp *formals;
  std::map<std::string, Sexp*> env;
  Sexp *body;
};

extern std::map<std::string, Sexp*> global_env;
void bind(const std::string &name, Sexp *e);
void bind(const std::string &name, Sexp *(*f)(Sexp *));
void unbind(const std::string &name);

bool isproc(Sexp *e);

Sexp *call(const Proc &proc, Sexp *args);

#endif
